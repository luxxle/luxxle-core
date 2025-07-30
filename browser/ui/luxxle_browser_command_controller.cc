/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/luxxle_browser_command_controller.h"

#include <optional>

#include "base/containers/fixed_flat_set.h"
#include "base/feature_list.h"
#include "base/types/to_address.h"
#include "luxxle/app/luxxle_command_ids.h"
#include "luxxle/browser/ai_chat/ai_chat_utils.h"
#include "luxxle/browser/profiles/profile_util.h"
#include "luxxle/browser/ui/luxxle_pages.h"
#include "luxxle/browser/ui/browser_commands.h"
#include "luxxle/browser/ui/sidebar/sidebar_utils.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/components/ai_chat/core/browser/utils.h"
#include "luxxle/components/ai_chat/core/common/features.h"
#include "luxxle/components/ai_chat/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/luxxle_wayback_machine/buildflags/buildflags.h"
#include "luxxle/components/commander/common/buildflags/buildflags.h"
#include "luxxle/components/commands/common/features.h"
#include "luxxle/components/playlist/common/buildflags/buildflags.h"
#include "luxxle/components/speedreader/common/buildflags/buildflags.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/send_tab_to_self/send_tab_to_self_util.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_window/public/browser_window_features.h"
#include "chrome/browser/ui/tabs/tab_change_type.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "components/sync/base/command_line_switches.h"
#include "content/public/browser/web_contents.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "luxxle/components/speedreader/common/features.h"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
#include "luxxle/browser/playlist/playlist_service_factory.h"
#include "luxxle/components/playlist/common/features.h"
#endif

#if BUILDFLAG(ENABLE_COMMANDER)
#include "luxxle/browser/ui/commander/commander_service.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "luxxle/browser/tor/tor_profile_service_factory.h"
#endif

namespace {

bool IsLuxxleCommands(int id) {
  return id >= IDC_LUXXLE_COMMANDS_START && id <= IDC_LUXXLE_COMMANDS_LAST;
}

bool IsLuxxleOverrideCommands(int id) {
  static constexpr auto kOverrideCommands = base::MakeFixedFlatSet<int>({
      IDC_NEW_WINDOW,
      IDC_NEW_INCOGNITO_WINDOW,
  });
  return kOverrideCommands.contains(id);
}

}  // namespace

namespace chrome {

LuxxleBrowserCommandController::LuxxleBrowserCommandController(Browser* browser)
    : BrowserCommandController(browser),
      browser_(*browser),
      luxxle_command_updater_(nullptr) {
  InitLuxxleCommandState();
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  if (auto* vpn_service = luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(
          browser_->profile())) {
    luxxle_vpn::LuxxleVPNServiceObserver::Observe(vpn_service);
  }
#endif
}

LuxxleBrowserCommandController::~LuxxleBrowserCommandController() = default;

void LuxxleBrowserCommandController::TabChangedAt(content::WebContents* contents,
                                                 int index,
                                                 TabChangeType type) {
  UpdateCommandEnabled(IDC_CLOSE_DUPLICATE_TABS,
                       luxxle::HasDuplicateTabs(&*browser_));
  UpdateCommandsForTabs();
  UpdateCommandsForSend();
}

void LuxxleBrowserCommandController::TabPinnedStateChanged(
    TabStripModel* tab_strip_model,
    content::WebContents* contents,
    int index) {
  UpdateCommandsForPin();
}

void LuxxleBrowserCommandController::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  BrowserCommandController::OnTabStripModelChanged(tab_strip_model, change,
                                                   selection);

  UpdateCommandEnabled(IDC_WINDOW_CLOSE_TABS_TO_LEFT,
                       luxxle::CanCloseTabsToLeft(&*browser_));
  UpdateCommandEnabled(IDC_CLOSE_DUPLICATE_TABS,
                       luxxle::HasDuplicateTabs(&*browser_));
  UpdateCommandsForTabs();
  UpdateCommandsForSend();
  UpdateCommandsForPin();

  if (tabs::features::IsLuxxleSplitViewEnabled() && browser_->is_type_normal() &&
      selection.active_tab_changed()) {
    UpdateCommandForSplitView();
  }
}

void LuxxleBrowserCommandController::OnTabGroupChanged(
    const TabGroupChange& change) {
  UpdateCommandsForTabs();
}

void LuxxleBrowserCommandController::OnTileTabs(const TabTile& tile) {
  UpdateCommandForSplitView();
}

void LuxxleBrowserCommandController::OnWillBreakTile(const TabTile& tile) {
  UpdateCommandForSplitView();
}

void LuxxleBrowserCommandController::OnWillDeleteBrowserData() {
  split_view_browser_data_observation_.Reset();
}

bool LuxxleBrowserCommandController::SupportsCommand(int id) const {
  return IsLuxxleCommands(id) ? luxxle_command_updater_.SupportsCommand(id)
                             : BrowserCommandController::SupportsCommand(id);
}

bool LuxxleBrowserCommandController::IsCommandEnabled(int id) const {
  return IsLuxxleCommands(id) ? luxxle_command_updater_.IsCommandEnabled(id)
                             : BrowserCommandController::IsCommandEnabled(id);
}

bool LuxxleBrowserCommandController::ExecuteCommandWithDisposition(
    int id,
    WindowOpenDisposition disposition,
    base::TimeTicks time_stamp) {
  return IsLuxxleCommands(id) || IsLuxxleOverrideCommands(id)
             ? ExecuteLuxxleCommandWithDisposition(id, disposition, time_stamp)
             : BrowserCommandController::ExecuteCommandWithDisposition(
                   id, disposition, time_stamp);
}

void LuxxleBrowserCommandController::AddCommandObserver(
    int id,
    CommandObserver* observer) {
  IsLuxxleCommands(id)
      ? luxxle_command_updater_.AddCommandObserver(id, observer)
      : BrowserCommandController::AddCommandObserver(id, observer);
}

void LuxxleBrowserCommandController::RemoveCommandObserver(
    int id,
    CommandObserver* observer) {
  IsLuxxleCommands(id)
      ? luxxle_command_updater_.RemoveCommandObserver(id, observer)
      : BrowserCommandController::RemoveCommandObserver(id, observer);
}

void LuxxleBrowserCommandController::RemoveCommandObserver(
    CommandObserver* observer) {
  luxxle_command_updater_.RemoveCommandObserver(observer);
  BrowserCommandController::RemoveCommandObserver(observer);
}

bool LuxxleBrowserCommandController::UpdateCommandEnabled(int id, bool state) {
  return IsLuxxleCommands(id)
             ? luxxle_command_updater_.UpdateCommandEnabled(id, state)
             : BrowserCommandController::UpdateCommandEnabled(id, state);
}

void LuxxleBrowserCommandController::InitLuxxleCommandState() {
  // Sync, Rewards, and Wallet pages don't work in tor(guest) sessions.
  // They also don't work in private windows but they are redirected
  // to a normal window in this case.
  const bool is_guest_session = browser_->profile()->IsGuestSession();
  if (!is_guest_session) {
    // If Rewards is not supported due to OFAC sanctions we still want to show
    // the menu item.
    if (luxxle_rewards::IsSupported(browser_->profile()->GetPrefs())) {
      UpdateCommandForLuxxleRewards();
    }
    if (luxxle_wallet::IsAllowed(browser_->profile()->GetPrefs())) {
      UpdateCommandForLuxxleWallet();
    }
    if (syncer::IsSyncAllowedByFlag()) {
      UpdateCommandForLuxxleSync();
    }
  }
  UpdateCommandForWebcompatReporter();
#if BUILDFLAG(ENABLE_TOR)
  UpdateCommandForTor();
#endif
  UpdateCommandForSidebar();
  UpdateCommandForLuxxleVPN();
  UpdateCommandForPlaylist();
  UpdateCommandForWaybackMachine();
  pref_change_registrar_.Init(browser_->profile()->GetPrefs());

  UpdateCommandForAIChat();
  if (ai_chat::IsAllowedForContext(browser_->profile(), false)) {
    pref_change_registrar_.Add(
        ai_chat::prefs::kEnabledByPolicy,
        base::BindRepeating(
            &LuxxleBrowserCommandController::UpdateCommandForAIChat,
            base::Unretained(this)));
  }

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  if (luxxle_vpn::IsAllowedForContext(browser_->profile())) {
    pref_change_registrar_.Add(
        luxxle_vpn::prefs::kManagedLuxxleVPNDisabled,
        base::BindRepeating(
            &LuxxleBrowserCommandController::UpdateCommandForLuxxleVPN,
            base::Unretained(this)));
  }
#endif
  bool add_new_profile_enabled = !is_guest_session;
  bool open_guest_profile_enabled = !is_guest_session;
  if (!is_guest_session) {
    if (PrefService* local_state = g_browser_process->local_state()) {
      add_new_profile_enabled =
          local_state->GetBoolean(prefs::kBrowserAddPersonEnabled);
      open_guest_profile_enabled =
          local_state->GetBoolean(prefs::kBrowserGuestModeEnabled);
    }
  }
  UpdateCommandEnabled(IDC_ADD_NEW_PROFILE, add_new_profile_enabled);
  UpdateCommandEnabled(IDC_OPEN_GUEST_PROFILE, open_guest_profile_enabled);
  UpdateCommandEnabled(IDC_COPY_CLEAN_LINK, true);
  UpdateCommandEnabled(IDC_TOGGLE_TAB_MUTE, true);

#if BUILDFLAG(ENABLE_SPEEDREADER)
  if (base::FeatureList::IsEnabled(speedreader::kSpeedreaderFeature)) {
    UpdateCommandEnabled(IDC_SPEEDREADER_ICON_ONCLICK, true);
  }
#endif

#if BUILDFLAG(ENABLE_COMMANDER)
  UpdateCommandEnabled(IDC_COMMANDER, commander::IsEnabled());
#endif

  UpdateCommandEnabled(IDC_SHOW_APPS_PAGE,
                       !browser_->profile()->IsPrimaryOTRProfile());

  UpdateCommandEnabled(IDC_LUXXLE_BOOKMARK_BAR_SUBMENU, true);

  UpdateCommandEnabled(IDC_TOGGLE_VERTICAL_TABS, true);
  UpdateCommandEnabled(IDC_TOGGLE_VERTICAL_TABS_WINDOW_TITLE, true);
  UpdateCommandEnabled(IDC_TOGGLE_VERTICAL_TABS_EXPANDED, true);

  UpdateCommandEnabled(IDC_CONFIGURE_LUXXLE_NEWS,
                       !browser_->profile()->IsOffTheRecord());

  UpdateCommandEnabled(
      IDC_CONFIGURE_SHORTCUTS,
      base::FeatureList::IsEnabled(commands::features::kLuxxleCommands));

  UpdateCommandEnabled(IDC_SHOW_LUXXLE_TALK, true);
  UpdateCommandEnabled(IDC_TOGGLE_SHIELDS, true);
  UpdateCommandEnabled(IDC_TOGGLE_JAVASCRIPT, true);

  UpdateCommandEnabled(IDC_CLOSE_DUPLICATE_TABS,
                       luxxle::HasDuplicateTabs(&*browser_));
  UpdateCommandEnabled(IDC_WINDOW_ADD_ALL_TABS_TO_NEW_GROUP, true);

  UpdateCommandEnabled(IDC_SCROLL_TAB_TO_TOP, true);
  UpdateCommandEnabled(IDC_SCROLL_TAB_TO_BOTTOM, true);

  UpdateCommandEnabled(IDC_LUXXLE_SEND_TAB_TO_SELF, true);

  UpdateCommandsForTabs();
  UpdateCommandsForSend();
  UpdateCommandsForPin();

  UpdateCommandEnabled(IDC_TOGGLE_ALL_BOOKMARKS_BUTTON_VISIBILITY, true);
  UpdateCommandEnabled(IDC_EXPORT_ALL_BOOKMARKS, true);

  if (browser_->is_type_normal()) {
    // Delete these when upstream enables by default.
    UpdateCommandEnabled(IDC_READING_LIST_MENU, true);
    UpdateCommandEnabled(IDC_READING_LIST_MENU_ADD_TAB, true);
    UpdateCommandEnabled(IDC_READING_LIST_MENU_SHOW_UI, true);
  }
}

void LuxxleBrowserCommandController::UpdateCommandForLuxxleRewards() {
  UpdateCommandEnabled(IDC_SHOW_LUXXLE_REWARDS, true);
}

void LuxxleBrowserCommandController::UpdateCommandForWebcompatReporter() {
  UpdateCommandEnabled(IDC_SHOW_LUXXLE_WEBCOMPAT_REPORTER, true);
}

#if BUILDFLAG(ENABLE_TOR)
void LuxxleBrowserCommandController::UpdateCommandForTor() {
  // Enable new tor connection only for tor profile.
  UpdateCommandEnabled(IDC_NEW_TOR_CONNECTION_FOR_SITE,
                       browser_->profile()->IsTor());
  UpdateCommandEnabled(
      IDC_NEW_OFFTHERECORD_WINDOW_TOR,
      !TorProfileServiceFactory::IsTorDisabled(browser_->profile()));
}
#endif

void LuxxleBrowserCommandController::UpdateCommandForSidebar() {
  if (sidebar::CanUseSidebar(&*browser_)) {
    UpdateCommandEnabled(IDC_SIDEBAR_SHOW_OPTION_MENU, true);
    UpdateCommandEnabled(IDC_SIDEBAR_TOGGLE_POSITION, true);
    UpdateCommandEnabled(IDC_TOGGLE_SIDEBAR, true);
  }
}

void LuxxleBrowserCommandController::UpdateCommandForAIChat() {
  // AI Chat command implementation needs sidebar
  bool allowed_for_context = ai_chat::IsAllowedForContext(browser_->profile());
  UpdateCommandEnabled(IDC_TOGGLE_AI_CHAT, sidebar::CanUseSidebar(&*browser_) &&
                                               allowed_for_context);
  UpdateCommandEnabled(
      IDC_OPEN_FULL_PAGE_CHAT,
      ai_chat::features::IsAIChatHistoryEnabled() && allowed_for_context);
}

void LuxxleBrowserCommandController::UpdateCommandForLuxxleVPN() {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  if (!luxxle_vpn::IsLuxxleVPNEnabled(browser_->profile())) {
    UpdateCommandEnabled(IDC_SHOW_LUXXLE_VPN_PANEL, false);
    UpdateCommandEnabled(IDC_LUXXLE_VPN_MENU, false);
    UpdateCommandEnabled(IDC_TOGGLE_LUXXLE_VPN_TOOLBAR_BUTTON, false);
    UpdateCommandEnabled(IDC_SEND_LUXXLE_VPN_FEEDBACK, false);
    UpdateCommandEnabled(IDC_ABOUT_LUXXLE_VPN, false);
    UpdateCommandEnabled(IDC_MANAGE_LUXXLE_VPN_PLAN, false);
    UpdateCommandEnabled(IDC_TOGGLE_LUXXLE_VPN, false);
#if BUILDFLAG(IS_WIN)
    UpdateCommandEnabled(IDC_TOGGLE_LUXXLE_VPN_TRAY_ICON, false);
#endif
    return;
  }
  UpdateCommandEnabled(IDC_SHOW_LUXXLE_VPN_PANEL, true);
  UpdateCommandEnabled(IDC_TOGGLE_LUXXLE_VPN_TOOLBAR_BUTTON, true);
#if BUILDFLAG(IS_WIN)
  UpdateCommandEnabled(IDC_TOGGLE_LUXXLE_VPN_TRAY_ICON, true);
#endif
  UpdateCommandEnabled(IDC_SEND_LUXXLE_VPN_FEEDBACK, true);
  UpdateCommandEnabled(IDC_ABOUT_LUXXLE_VPN, true);
  UpdateCommandEnabled(IDC_MANAGE_LUXXLE_VPN_PLAN, true);

  if (auto* vpn_service = luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(
          browser_->profile())) {
    // Only show vpn sub menu for purchased user.
    UpdateCommandEnabled(IDC_LUXXLE_VPN_MENU, vpn_service->is_purchased_user());
    UpdateCommandEnabled(IDC_TOGGLE_LUXXLE_VPN,
                         vpn_service->is_purchased_user());
  }
#endif
}

void LuxxleBrowserCommandController::UpdateCommandForPlaylist() {
#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
  if (base::FeatureList::IsEnabled(playlist::features::kPlaylist)) {
    UpdateCommandEnabled(
        IDC_SHOW_PLAYLIST_BUBBLE,
        browser_->is_type_normal() &&
            playlist::PlaylistServiceFactory::GetForBrowserContext(
                browser_->profile()));
  }
#endif
}

void LuxxleBrowserCommandController::UpdateCommandForWaybackMachine() {
#if BUILDFLAG(ENABLE_LUXXLE_WAYBACK_MACHINE)
  UpdateCommandEnabled(IDC_SHOW_WAYBACK_MACHINE_BUBBLE, true);
#endif
}

void LuxxleBrowserCommandController::UpdateCommandsForTabs() {
  UpdateCommandEnabled(IDC_WINDOW_MUTE_ALL_TABS,
                       luxxle::CanMuteAllTabs(&*browser_, false));
  UpdateCommandEnabled(IDC_WINDOW_MUTE_OTHER_TABS,
                       luxxle::CanMuteAllTabs(&*browser_, true));
  UpdateCommandEnabled(IDC_WINDOW_UNMUTE_ALL_TABS,
                       luxxle::CanUnmuteAllTabs(&*browser_));

  UpdateCommandEnabled(IDC_GROUP_TABS_ON_CURRENT_ORIGIN, true);
  UpdateCommandEnabled(IDC_MOVE_GROUP_TO_NEW_WINDOW, true);

  bool is_in_group = luxxle::IsInGroup(&*browser_);
  bool has_ungrouped_tabs = luxxle::HasUngroupedTabs(&*browser_);
  UpdateCommandEnabled(IDC_WINDOW_GROUP_UNGROUPED_TABS, has_ungrouped_tabs);
  UpdateCommandEnabled(IDC_WINDOW_UNGROUP_GROUP, is_in_group);
  UpdateCommandEnabled(IDC_WINDOW_REMOVE_TAB_FROM_GROUP, is_in_group);
  UpdateCommandEnabled(IDC_WINDOW_NAME_GROUP, is_in_group);
  UpdateCommandEnabled(IDC_WINDOW_NEW_TAB_IN_GROUP, is_in_group);
  UpdateCommandEnabled(IDC_WINDOW_UNGROUP_ALL_TABS,
                       luxxle::CanUngroupAllTabs(&*browser_));
  UpdateCommandEnabled(IDC_WINDOW_TOGGLE_GROUP_EXPANDED, is_in_group);
  UpdateCommandEnabled(IDC_WINDOW_CLOSE_UNGROUPED_TABS, has_ungrouped_tabs);
  UpdateCommandEnabled(IDC_WINDOW_CLOSE_TABS_NOT_IN_CURRENT_GROUP, is_in_group);
  UpdateCommandEnabled(IDC_WINDOW_CLOSE_GROUP, is_in_group);

  UpdateCommandEnabled(IDC_WINDOW_BRING_ALL_TABS,
                       luxxle::CanBringAllTabs(&*browser_));
}

void LuxxleBrowserCommandController::UpdateCommandsForSend() {
  UpdateCommandEnabled(
      IDC_LUXXLE_SEND_TAB_TO_SELF,
      send_tab_to_self::ShouldDisplayEntryPoint(
          browser_->tab_strip_model()->GetActiveWebContents()));
}

void LuxxleBrowserCommandController::UpdateCommandsForPin() {
  UpdateCommandEnabled(IDC_WINDOW_CLOSE_UNPINNED_TABS,
                       luxxle::CanCloseUnpinnedTabs(&*browser_));
}

void LuxxleBrowserCommandController::UpdateCommandForSplitView() {
  if (!split_view_browser_data_observation_.IsObserving()) {
    split_view_browser_data_observation_.Observe(
        browser_->GetFeatures().split_view_browser_data());
  }

  UpdateCommandEnabled(IDC_NEW_SPLIT_VIEW, luxxle::CanOpenNewSplitViewForTab(
                                               base::to_address(browser_)));
  UpdateCommandEnabled(IDC_TILE_TABS,
                       luxxle::CanTileTabs(base::to_address(browser_)));

  const auto is_tab_tiled = luxxle::IsTabsTiled(base::to_address(browser_));
  for (auto command_enabled_when_tab_is_tiled :
       {IDC_BREAK_TILE, IDC_SWAP_SPLIT_VIEW}) {
    UpdateCommandEnabled(command_enabled_when_tab_is_tiled, is_tab_tiled);
  }
}

void LuxxleBrowserCommandController::UpdateCommandForLuxxleSync() {
  UpdateCommandEnabled(IDC_SHOW_LUXXLE_SYNC, true);
}

void LuxxleBrowserCommandController::UpdateCommandForLuxxleWallet() {
  UpdateCommandEnabled(IDC_SHOW_LUXXLE_WALLET, true);
  UpdateCommandEnabled(IDC_SHOW_LUXXLE_WALLET_PANEL, true);
  UpdateCommandEnabled(IDC_CLOSE_LUXXLE_WALLET_PANEL, true);
}

bool LuxxleBrowserCommandController::ExecuteLuxxleCommandWithDisposition(
    int id,
    WindowOpenDisposition disposition,
    base::TimeTicks time_stamp) {
  if (!SupportsCommand(id) || !IsCommandEnabled(id)) {
    return false;
  }

  if (browser_->tab_strip_model()->active_index() == TabStripModel::kNoTab) {
    return true;
  }

  DCHECK(IsCommandEnabled(id)) << "Invalid/disabled command " << id;

  switch (id) {
    case IDC_NEW_WINDOW:
      // Use chromium's action for non-Tor profiles.
      if (!browser_->profile()->IsTor()) {
        return BrowserCommandController::ExecuteCommandWithDisposition(
            id, disposition, time_stamp);
      }
      NewEmptyWindow(browser_->profile()->GetOriginalProfile());
      break;
    case IDC_NEW_INCOGNITO_WINDOW:
      // Use chromium's action for non-Tor profiles.
      if (!browser_->profile()->IsTor()) {
        return BrowserCommandController::ExecuteCommandWithDisposition(
            id, disposition, time_stamp);
      }
      NewIncognitoWindow(browser_->profile()->GetOriginalProfile());
      break;
    case IDC_SHOW_LUXXLE_REWARDS:
      luxxle::ShowLuxxleRewards(&*browser_);
      break;
    case IDC_SHOW_LUXXLE_WEBCOMPAT_REPORTER:
      luxxle::ShowWebcompatReporter(&*browser_);
      break;
    case IDC_NEW_OFFTHERECORD_WINDOW_TOR:
      luxxle::NewOffTheRecordWindowTor(&*browser_);
      break;
    case IDC_NEW_TOR_CONNECTION_FOR_SITE:
      luxxle::NewTorConnectionForSite(&*browser_);
      break;
    case IDC_SHOW_LUXXLE_SYNC:
      luxxle::ShowSync(&*browser_);
      break;
    case IDC_SHOW_LUXXLE_WALLET:
      luxxle::ShowLuxxleWallet(&*browser_);
      break;
    case IDC_TOGGLE_AI_CHAT:
      luxxle::ToggleAIChat(&*browser_);
      break;
    case IDC_OPEN_FULL_PAGE_CHAT:
      luxxle::ShowFullpageChat(&*browser_);
      break;
    case IDC_SPEEDREADER_ICON_ONCLICK:
      luxxle::MaybeDistillAndShowSpeedreaderBubble(&*browser_);
      break;
    case IDC_SHOW_LUXXLE_WALLET_PANEL:
      luxxle::ShowWalletBubble(&*browser_);
      break;
    case IDC_CLOSE_LUXXLE_WALLET_PANEL:
      luxxle::CloseWalletBubble(&*browser_);
      break;
    case IDC_SHOW_LUXXLE_VPN_PANEL:
      luxxle::ShowLuxxleVPNBubble(&*browser_);
      break;
    case IDC_TOGGLE_LUXXLE_VPN_TRAY_ICON:
      luxxle::ToggleLuxxleVPNTrayIcon();
      break;
    case IDC_TOGGLE_LUXXLE_VPN_TOOLBAR_BUTTON:
      luxxle::ToggleLuxxleVPNButton(&*browser_);
      break;
    case IDC_SEND_LUXXLE_VPN_FEEDBACK:
    case IDC_ABOUT_LUXXLE_VPN:
    case IDC_MANAGE_LUXXLE_VPN_PLAN:
      luxxle::OpenLuxxleVPNUrls(&*browser_, id);
      break;
    case IDC_SIDEBAR_TOGGLE_POSITION:
      luxxle::ToggleSidebarPosition(&*browser_);
      break;
    case IDC_TOGGLE_SIDEBAR:
      luxxle::ToggleSidebar(&*browser_);
      break;
    case IDC_COPY_CLEAN_LINK:
      luxxle::CopySanitizedURL(
          &*browser_,
          browser_->tab_strip_model()->GetActiveWebContents()->GetVisibleURL());
      break;
    case IDC_TOGGLE_TAB_MUTE:
      luxxle::ToggleActiveTabAudioMute(&*browser_);
      break;
    case IDC_TOGGLE_VERTICAL_TABS:
      luxxle::ToggleVerticalTabStrip(&*browser_);
      break;
    case IDC_TOGGLE_VERTICAL_TABS_WINDOW_TITLE:
      luxxle::ToggleWindowTitleVisibilityForVerticalTabs(&*browser_);
      break;
    case IDC_TOGGLE_VERTICAL_TABS_EXPANDED:
      luxxle::ToggleVerticalTabStripExpanded(&*browser_);
      break;
    case IDC_CONFIGURE_LUXXLE_NEWS:
      luxxle::ShowLuxxleNewsConfigure(&*browser_);
      break;
    case IDC_CONFIGURE_SHORTCUTS:
      luxxle::ShowShortcutsPage(&*browser_);
      break;
    case IDC_SHOW_LUXXLE_TALK:
      luxxle::ShowLuxxleTalk(&*browser_);
      break;
    case IDC_TOGGLE_SHIELDS:
      luxxle::ToggleShieldsEnabled(&*browser_);
      break;
    case IDC_TOGGLE_JAVASCRIPT:
      luxxle::ToggleJavascriptEnabled(&*browser_);
      break;
    case IDC_SHOW_PLAYLIST_BUBBLE:
#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
      luxxle::ShowPlaylistBubble(&*browser_);
#else
      NOTREACHED() << " This command shouldn't be enabled";
#endif
      break;
    case IDC_SHOW_WAYBACK_MACHINE_BUBBLE:
#if BUILDFLAG(ENABLE_LUXXLE_WAYBACK_MACHINE)
      luxxle::ShowWaybackMachineBubble(&*browser_);
#endif
      break;
    case IDC_GROUP_TABS_ON_CURRENT_ORIGIN:
      luxxle::GroupTabsOnCurrentOrigin(&*browser_);
      break;
    case IDC_MOVE_GROUP_TO_NEW_WINDOW:
      luxxle::MoveGroupToNewWindow(&*browser_);
      break;
    case IDC_CLOSE_DUPLICATE_TABS:
      luxxle::CloseDuplicateTabs(&*browser_);
      break;
    case IDC_WINDOW_CLOSE_TABS_TO_LEFT:
      luxxle::CloseTabsToLeft(&*browser_);
      break;
    case IDC_WINDOW_CLOSE_UNPINNED_TABS:
      luxxle::CloseUnpinnedTabs(&*browser_);
      break;
    case IDC_WINDOW_ADD_ALL_TABS_TO_NEW_GROUP:
      luxxle::AddAllTabsToNewGroup(&*browser_);
      break;
    case IDC_WINDOW_MUTE_ALL_TABS:
      luxxle::MuteAllTabs(&*browser_, false);
      break;
    case IDC_WINDOW_MUTE_OTHER_TABS:
      luxxle::MuteAllTabs(&*browser_, true);
      break;
    case IDC_WINDOW_UNMUTE_ALL_TABS:
      luxxle::UnmuteAllTabs(&*browser_);
      break;
    case IDC_SCROLL_TAB_TO_TOP:
      luxxle::ScrollTabToTop(&*browser_);
      break;
    case IDC_SCROLL_TAB_TO_BOTTOM:
      luxxle::ScrollTabToBottom(&*browser_);
      break;
    case IDC_LUXXLE_SEND_TAB_TO_SELF:
      chrome::SendTabToSelf(&*browser_);
      break;
    case IDC_TOGGLE_ALL_BOOKMARKS_BUTTON_VISIBILITY:
      luxxle::ToggleAllBookmarksButtonVisibility(base::to_address(browser_));
      break;
    case IDC_EXPORT_ALL_BOOKMARKS:
      luxxle::ExportAllBookmarks(&*browser_);
      break;
    case IDC_COMMANDER:
#if BUILDFLAG(ENABLE_COMMANDER)
      luxxle::ToggleCommander(base::to_address(browser_));
#endif
      break;
    case IDC_SHOW_APPS_PAGE:
      luxxle::ShowAppsPage(&*browser_);
      break;
    case IDC_WINDOW_GROUP_UNGROUPED_TABS:
      luxxle::GroupUngroupedTabs(&*browser_);
      break;
    case IDC_WINDOW_UNGROUP_GROUP:
      luxxle::UngroupCurrentGroup(&*browser_);
      break;
    case IDC_WINDOW_REMOVE_TAB_FROM_GROUP:
      luxxle::RemoveTabFromGroup(&*browser_);
      break;
    case IDC_WINDOW_UNGROUP_ALL_TABS:
      luxxle::UngroupAllTabs(&*browser_);
      break;
    case IDC_WINDOW_NAME_GROUP:
      luxxle::NameGroup(&*browser_);
      break;
    case IDC_WINDOW_NEW_TAB_IN_GROUP:
      luxxle::NewTabInGroup(&*browser_);
      break;
    case IDC_WINDOW_TOGGLE_GROUP_EXPANDED:
      luxxle::ToggleGroupExpanded(&*browser_);
      break;
    case IDC_WINDOW_CLOSE_UNGROUPED_TABS:
      luxxle::CloseUngroupedTabs(&*browser_);
      break;
    case IDC_WINDOW_CLOSE_TABS_NOT_IN_CURRENT_GROUP:
      luxxle::CloseTabsNotInCurrentGroup(&*browser_);
      break;
    case IDC_WINDOW_CLOSE_GROUP:
      luxxle::CloseGroup(&*browser_);
      break;
    case IDC_WINDOW_BRING_ALL_TABS:
      luxxle::BringAllTabs(&*browser_);
      break;
    case IDC_NEW_SPLIT_VIEW:
      luxxle::NewSplitViewForTab(&*browser_);
      break;
    case IDC_TILE_TABS:
      luxxle::TileTabs(&*browser_);
      break;
    case IDC_BREAK_TILE:
      luxxle::BreakTiles(&*browser_);
      break;
    case IDC_SWAP_SPLIT_VIEW:
      luxxle::SwapTabsInTile(&*browser_);
      break;
    default:
      LOG(WARNING) << "Received Unimplemented Command: " << id;
      break;
  }

  return true;
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
void LuxxleBrowserCommandController::OnPurchasedStateChanged(
    luxxle_vpn::mojom::PurchasedState state,
    const std::optional<std::string>& description) {
  UpdateCommandForLuxxleVPN();
}
#endif

}  // namespace chrome
