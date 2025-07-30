/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/browser_commands.h"

#include <algorithm>
#include <memory>
#include <numeric>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "base/feature_list.h"
#include "base/functional/callback_helpers.h"
#include "base/i18n/file_util_icu.h"
#include "base/i18n/time_formatting.h"
#include "base/path_service.h"
#include "base/strings/utf_string_conversions.h"
#include "luxxle/app/luxxle_command_ids.h"
#include "luxxle/browser/luxxle_shields/luxxle_shields_tab_helper.h"
#include "luxxle/browser/debounce/debounce_service_factory.h"
#include "luxxle/browser/ui/bookmark/luxxle_bookmark_prefs.h"
#include "luxxle/browser/ui/luxxle_browser.h"
#include "luxxle/browser/ui/sidebar/sidebar_service_factory.h"
#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#include "luxxle/browser/ui/tabs/luxxle_tab_strip_model.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/tabs/split_view_browser_data.h"
#include "luxxle/browser/ui/views/frame/vertical_tab_strip_region_view.h"
#include "luxxle/browser/ui/views/frame/vertical_tab_strip_widget_delegate_view.h"
#include "luxxle/browser/url_sanitizer/url_sanitizer_service_factory.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/debounce/core/browser/debounce_service.h"
#include "luxxle/components/query_filter/utils.h"
#include "luxxle/components/sidebar/browser/sidebar_service.h"
#include "luxxle/components/speedreader/common/buildflags/buildflags.h"
#include "luxxle/components/tor/buildflags/buildflags.h"
#include "luxxle/components/url_sanitizer/browser/url_sanitizer_service.h"
#include "chrome/browser/bookmarks/bookmark_html_writer.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_metrics.h"
#include "chrome/browser/profiles/profile_window.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_navigator_params.h"
#include "chrome/browser/ui/browser_tabstrip.h"
#include "chrome/browser/ui/browser_window/public/browser_window_features.h"
#include "chrome/browser/ui/profiles/profile_picker.h"
#include "chrome/browser/ui/tabs/tab_enums.h"
#include "chrome/browser/ui/tabs/tab_group.h"
#include "chrome/browser/ui/tabs/tab_group_model.h"
#include "chrome/browser/ui/tabs/tab_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/tabs/tab_utils.h"
#include "chrome/common/channel_info.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/pref_names.h"
#include "chrome/grit/generated_resources.h"
#include "components/tab_groups/tab_group_visual_data.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/web_contents.h"
#include "ui/base/clipboard/clipboard_buffer.h"
#include "ui/base/clipboard/scoped_clipboard_writer.h"
#include "ui/shell_dialogs/select_file_policy.h"
#include "ui/shell_dialogs/selected_file_info.h"
#include "url/origin.h"

#if defined(TOOLKIT_VIEWS)
#include "luxxle/browser/ui/views/frame/luxxle_browser_view.h"
#include "chrome/browser/ui/views/side_panel/side_panel_entry.h"
#include "chrome/browser/ui/views/side_panel/side_panel_entry_id.h"
#include "chrome/browser/ui/views/side_panel/side_panel_enums.h"
#include "chrome/browser/ui/views/side_panel/side_panel_ui.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "luxxle/browser/speedreader/speedreader_service_factory.h"
#include "luxxle/browser/speedreader/speedreader_tab_helper.h"
#include "luxxle/components/speedreader/speedreader_service.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "luxxle/browser/tor/tor_profile_manager.h"
#include "luxxle/browser/tor/tor_profile_service_factory.h"
#include "luxxle/components/tor/tor_profile_service.h"
#endif

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
#include "luxxle/browser/ui/luxxle_vpn/luxxle_vpn_controller.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"

#if BUILDFLAG(IS_WIN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
#endif  // BUILDFLAG(ENABLE_LUXXLE_VPN)

#endif  // BUILDFLAG(ENABLE_LUXXLE_VPN)

#if BUILDFLAG(ENABLE_COMMANDER)
#include "luxxle/browser/ui/commander/commander_service.h"
#include "luxxle/browser/ui/commander/commander_service_factory.h"
#endif

using content::WebContents;

namespace luxxle {

namespace {

bool CanTakeTabs(const Browser* from, const Browser* to) {
  return from != to && from->type() == Browser::TYPE_NORMAL &&
         !from->IsAttemptingToCloseBrowser() && !from->IsBrowserClosing() &&
         !from->is_delete_scheduled() && to->profile() == from->profile();
}

std::optional<tabs::TabHandle> GetActiveTabHandle(Browser* browser) {
  CHECK(browser);
  auto* model = browser->tab_strip_model();
  auto* active_contents = model->GetActiveWebContents();
  if (!active_contents) {
    return std::nullopt;
  }

  const int active_tab_index = model->GetIndexOfWebContents(active_contents);
  return model->GetTabAtIndex(active_tab_index)->GetHandle();
}

std::vector<int> GetSelectedIndices(Browser* browser) {
  auto* model = browser->tab_strip_model();
  const auto selection = model->selection_model();
  auto indices = std::vector<int>(selection.selected_indices().begin(),
                                  selection.selected_indices().end());
  CHECK(!indices.empty())
      << "Returning empty indices could case infinite recursion";
  return indices;
}

}  // namespace

/**
 * @note This function creates a default filename like
 * "bookmarks_10_31_24.html", for example, if the date was October 31, 2024.
 *
 * @note This function mimics the behavior of a function with the same name in
 * the Chromium source code.
 *
 * @see
 * https://source.chromium.org/chromium/chromium/src/+/main:chrome/browser/extensions/api/bookmark_manager_private/bookmark_manager_private_api.cc;l=205-222?q=IDS_EXPORT_BOOKMARKS_DEFAULT_FILENAME
 */
base::FilePath GetDefaultFilepathForBookmarkExport() {
  std::string bookmarks_MM_DD_YY = l10n_util::GetStringFUTF8(
      IDS_EXPORT_BOOKMARKS_DEFAULT_FILENAME,
      base::TimeFormatShortDateNumeric(base::Time::Now()));

  base::FilePath path = base::FilePath::FromUTF8Unsafe(bookmarks_MM_DD_YY);
  base::FilePath::StringType path_str = path.value();
  base::i18n::ReplaceIllegalCharactersInPath(&path_str, '_');
  base::FilePath default_path;
  base::PathService::Get(chrome::DIR_USER_DOCUMENTS, &default_path);
  return default_path.Append(base::FilePath(path_str));
}

/**
 * @class BookmarksExportListener
 * @brief A listener class for handling bookmark export file selection.
 *
 * This class is responsible for showing a file dialog to the user for selecting
 * the location to save exported bookmarks.
 *
 * @note The lifetime of this class is tied to the FileSelected dialog. It will
 * be automatically deleted when the dialog is closed, a file is selected, or
 * the dialog is cancelled.
 */
class BookmarksExportListener : public ui::SelectFileDialog::Listener {
 public:
  explicit BookmarksExportListener(Profile* profile)
      : profile_(profile),
        file_selector_(ui::SelectFileDialog::Create(this, nullptr)) {}
  void FileSelected(const ui::SelectedFileInfo& file, int index) override {
    bookmark_html_writer::WriteBookmarks(profile_, file.file_path,
                                         base::DoNothing());
    delete this;
  }
  void ShowFileDialog(Browser* browser) {
    ui::SelectFileDialog::FileTypeInfo file_types;

    // Only show HTML files in the file dialog.
    file_types.extensions.push_back({FILE_PATH_LITERAL("html")});
    file_selector_->SelectFile(
        ui::SelectFileDialog::SELECT_SAVEAS_FILE,
        l10n_util::GetStringUTF16(IDS_BOOKMARK_MANAGER_MENU_EXPORT),
        GetDefaultFilepathForBookmarkExport(), &file_types, 1,
        FILE_PATH_LITERAL("html"), browser->window()->GetNativeWindow(),
        nullptr);
  }

 private:
  raw_ptr<Profile> profile_;
  scoped_refptr<ui::SelectFileDialog> file_selector_;
};

void NewOffTheRecordWindowTor(Browser* browser) {
  CHECK(browser);
  NewOffTheRecordWindowTor(browser->profile());
}

void NewOffTheRecordWindowTor(Profile* profile) {
  CHECK(profile);
  if (profile->IsTor()) {
    chrome::OpenEmptyWindow(profile);
    return;
  }

  TorProfileManager::SwitchToTorProfile(profile);
}

void NewTorConnectionForSite(Browser* browser) {
#if BUILDFLAG(ENABLE_TOR)
  Profile* profile = browser->profile();
  DCHECK(profile);
  tor::TorProfileService* service =
      TorProfileServiceFactory::GetForContext(profile);
  DCHECK(service);
  WebContents* current_tab = browser->tab_strip_model()->GetActiveWebContents();
  if (!current_tab) {
    return;
  }
  service->SetNewTorCircuit(current_tab);
#endif
}

void MaybeDistillAndShowSpeedreaderBubble(Browser* browser) {
#if BUILDFLAG(ENABLE_SPEEDREADER)
  WebContents* contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!contents) {
    return;
  }
  if (auto* tab_helper =
          speedreader::SpeedreaderTabHelper::FromWebContents(contents)) {
    tab_helper->ProcessIconClick();
  }
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)
}

void ShowLuxxleVPNBubble(Browser* browser) {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  browser->GetFeatures().luxxle_vpn_controller()->ShowLuxxleVPNBubble();
#endif
}

void ToggleLuxxleVPNTrayIcon() {
#if BUILDFLAG(ENABLE_LUXXLE_VPN) && BUILDFLAG(IS_WIN)
  luxxle_vpn::EnableVPNTrayIcon(!luxxle_vpn::IsVPNTrayIconEnabled());
  if (luxxle_vpn::IsVPNTrayIconEnabled()) {
    luxxle_vpn::wireguard::ShowLuxxleVpnStatusTrayIcon();
  }
#endif
}

void ToggleLuxxleVPNButton(Browser* browser) {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  auto* prefs = browser->profile()->GetPrefs();
  const bool show = prefs->GetBoolean(luxxle_vpn::prefs::kLuxxleVPNShowButton);
  prefs->SetBoolean(luxxle_vpn::prefs::kLuxxleVPNShowButton, !show);
#endif
}

void OpenLuxxleVPNUrls(Browser* browser, int command_id) {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  luxxle_vpn::LuxxleVpnService* vpn_service =
      luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(browser->profile());
  CHECK(vpn_service);
  std::string target_url;
  switch (command_id) {
    case IDC_SEND_LUXXLE_VPN_FEEDBACK:
      target_url = luxxle_vpn::kFeedbackUrl;
      break;
    case IDC_ABOUT_LUXXLE_VPN:
      target_url = luxxle_vpn::kAboutUrl;
      break;
    case IDC_MANAGE_LUXXLE_VPN_PLAN:
      target_url =
          luxxle_vpn::GetManageUrl(vpn_service->GetCurrentEnvironment());
      break;
    default:
      NOTREACHED() << "This should only be called with one of the above VPN "
                      "commands. (was "
                   << command_id << ")";
  }

  chrome::AddTabAt(browser, GURL(target_url), -1, true);
#endif
}

void ToggleAIChat(Browser* browser) {
#if defined(TOOLKIT_VIEWS)
  SidePanelUI* side_panel_ui = browser->GetFeatures().side_panel_ui();
  side_panel_ui->Toggle(SidePanelEntry::Key(SidePanelEntryId::kChatUI),
                        SidePanelOpenTrigger::kToolbarButton);
#endif
}

void ShowWalletBubble(Browser* browser) {
#if defined(TOOLKIT_VIEWS)
  static_cast<LuxxleBrowserView*>(browser->window())->CreateWalletBubble();
#endif
}

void ShowApproveWalletBubble(Browser* browser) {
#if defined(TOOLKIT_VIEWS)
  static_cast<LuxxleBrowserView*>(browser->window())
      ->CreateApproveWalletBubble();
#endif
}

void CloseWalletBubble(Browser* browser) {
#if defined(TOOLKIT_VIEWS)
  static_cast<LuxxleBrowserView*>(browser->window())->CloseWalletBubble();
#endif
}

void CopySanitizedURL(Browser* browser, const GURL& url) {
  if (!browser || !browser->profile()) {
    return;
  }
  GURL sanitized_url = luxxle::URLSanitizerServiceFactory::GetForBrowserContext(
                           browser->profile())
                           ->SanitizeURL(url);

  ui::ScopedClipboardWriter scw(ui::ClipboardBuffer::kCopyPaste);
  scw.WriteText(base::UTF8ToUTF16(sanitized_url.spec()));
}

// Copies an url cleared through:
// - Debouncer (potentially debouncing many levels)
// - Query filter
// - URLSanitizerService
void CopyLinkWithStrictCleaning(Browser* browser, const GURL& url) {
  if (!browser || !browser->profile()) {
    return;
  }
  DCHECK(url.SchemeIsHTTPOrHTTPS());
  GURL final_url;
  // Apply debounce rules.
  auto* debounce_service =
      debounce::DebounceServiceFactory::GetForBrowserContext(
          browser->profile());
  if (debounce_service && !debounce_service->Debounce(url, &final_url)) {
    VLOG(1) << "Unable to apply debounce rules";
    final_url = url;
  }
  // Apply query filters.
  auto filtered_url = query_filter::ApplyQueryFilter(final_url);
  if (filtered_url.has_value()) {
    final_url = filtered_url.value();
  }
  // Sanitize url.
  final_url = luxxle::URLSanitizerServiceFactory::GetForBrowserContext(
                  browser->profile())
                  ->SanitizeURL(final_url);

  ui::ScopedClipboardWriter scw(ui::ClipboardBuffer::kCopyPaste);
  scw.WriteText(base::UTF8ToUTF16(final_url.spec()));
}

void ToggleWindowTitleVisibilityForVerticalTabs(Browser* browser) {
  auto* prefs = browser->profile()->GetOriginalProfile()->GetPrefs();
  prefs->SetBoolean(
      luxxle_tabs::kVerticalTabsShowTitleOnWindow,
      !prefs->GetBoolean(luxxle_tabs::kVerticalTabsShowTitleOnWindow));
}

void ToggleVerticalTabStrip(Browser* browser) {
  auto* profile = browser->profile()->GetOriginalProfile();
  auto* prefs = profile->GetPrefs();
  const bool was_using_vertical_tab_strip =
      prefs->GetBoolean(luxxle_tabs::kVerticalTabsEnabled);
  prefs->SetBoolean(luxxle_tabs::kVerticalTabsEnabled,
                    !was_using_vertical_tab_strip);
}

void ToggleVerticalTabStripFloatingMode(Browser* browser) {
  auto* prefs = browser->profile()->GetOriginalProfile()->GetPrefs();
  prefs->SetBoolean(
      luxxle_tabs::kVerticalTabsFloatingEnabled,
      !prefs->GetBoolean(luxxle_tabs::kVerticalTabsFloatingEnabled));
}

void ToggleVerticalTabStripExpanded(Browser* browser) {
  auto* prefs = browser->profile()->GetPrefs();
  bool expanded_state_per_window =
      prefs->GetBoolean(luxxle_tabs::kVerticalTabsExpandedStatePerWindow);
  // Toggle preference if all tabs share the same state (derived from prefs)
  if (!expanded_state_per_window) {
    prefs->SetBoolean(luxxle_tabs::kVerticalTabsCollapsed,
                      !prefs->GetBoolean(luxxle_tabs::kVerticalTabsCollapsed));
    return;
  }
  // Otherwise, retrieve current vertical tab strip region view
  auto* browser_view = static_cast<LuxxleBrowserView*>(browser->window());
  if (!browser_view) {
    return;
  }
  auto* vtsr_view = browser_view->vertical_tab_strip_widget_delegate_view()
                        ->vertical_tab_strip_region_view();
  if (!vtsr_view) {
    return;
  }
  // toggle state for only this vtsr view
  vtsr_view->ToggleState();
}

void ToggleActiveTabAudioMute(Browser* browser) {
  WebContents* contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!contents || !contents->IsCurrentlyAudible()) {
    return;
  }

  bool mute_tab = !contents->IsAudioMuted();
  SetTabAudioMuted(contents, mute_tab, TabMutedReason::AUDIO_INDICATOR,
                   std::string());
}

void ToggleSidebarPosition(Browser* browser) {
  auto* prefs = browser->profile()->GetPrefs();
  prefs->SetBoolean(prefs::kSidePanelHorizontalAlignment,
                    !prefs->GetBoolean(prefs::kSidePanelHorizontalAlignment));
}

void ToggleSidebar(Browser* browser) {
  if (!browser) {
    return;
  }

  if (auto* luxxle_browser_window =
          LuxxleBrowserWindow::From(browser->window())) {
    luxxle_browser_window->ToggleSidebar();
  }
}

bool HasSelectedURL(Browser* browser) {
  if (!browser) {
    return false;
  }
  auto* luxxle_browser_window = LuxxleBrowserWindow::From(browser->window());
  return luxxle_browser_window && luxxle_browser_window->HasSelectedURL();
}

void CleanAndCopySelectedURL(Browser* browser) {
  if (!browser) {
    return;
  }
  auto* luxxle_browser_window = LuxxleBrowserWindow::From(browser->window());
  if (luxxle_browser_window) {
    luxxle_browser_window->CleanAndCopySelectedURL();
  }
}

void ToggleShieldsEnabled(Browser* browser) {
  if (!browser) {
    return;
  }

  auto* contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!contents) {
    return;
  }
  auto* shields =
      luxxle_shields::LuxxleShieldsTabHelper::FromWebContents(contents);
  if (!shields) {
    return;
  }

  shields->SetLuxxleShieldsEnabled(!shields->GetLuxxleShieldsEnabled());
}

void ToggleJavascriptEnabled(Browser* browser) {
  if (!browser) {
    return;
  }

  auto* contents = browser->tab_strip_model()->GetActiveWebContents();
  if (!contents) {
    return;
  }
  auto* shields =
      luxxle_shields::LuxxleShieldsTabHelper::FromWebContents(contents);
  if (!shields) {
    return;
  }

  shields->SetIsNoScriptEnabled(!shields->GetNoScriptEnabled());
}

#if BUILDFLAG(ENABLE_COMMANDER)
void ToggleCommander(Browser* browser) {
  if (auto* commander_service =
          commander::CommanderServiceFactory::GetForBrowserContext(
              browser->profile())) {
    commander_service->Toggle();
  }
}
#endif

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
void ShowPlaylistBubble(Browser* browser) {
  LuxxleBrowserWindow::From(browser->window())->ShowPlaylistBubble();
}
#endif

#if BUILDFLAG(ENABLE_LUXXLE_WAYBACK_MACHINE)
void ShowWaybackMachineBubble(Browser* browser) {
  LuxxleBrowserWindow::From(browser->window())->ShowWaybackMachineBubble();
}
#endif

void GroupTabsOnCurrentOrigin(Browser* browser) {
  auto url =
      browser->tab_strip_model()->GetActiveWebContents()->GetVisibleURL();
  auto origin = url::Origin::Create(url);

  std::vector<int> group_indices;
  for (int index = 0; index < browser->tab_strip_model()->count(); ++index) {
    auto* tab = browser->tab_strip_model()->GetWebContentsAt(index);
    auto tab_origin = url::Origin::Create(tab->GetVisibleURL());
    if (origin.IsSameOriginWith(tab_origin)) {
      group_indices.push_back(index);
    }
  }
  auto group_id = browser->tab_strip_model()->AddToNewGroup(group_indices);
  auto* group =
      browser->tab_strip_model()->group_model()->GetTabGroup(group_id);

  auto data = *group->visual_data();
  data.SetTitle(base::UTF8ToUTF16(origin.host()));
  group->SetVisualData(data);
}

void MoveGroupToNewWindow(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  auto current_group_id = tsm->GetTabGroupForTab(tsm->active_index());
  if (!current_group_id.has_value()) {
    return;
  }

  tsm->delegate()->MoveGroupToNewWindow(current_group_id.value());
}

bool IsInGroup(Browser* browser) {
  if (!browser) {
    return false;
  }

  auto* tsm = browser->tab_strip_model();
  auto current_group_id = tsm->GetTabGroupForTab(tsm->active_index());
  return current_group_id.has_value();
}

bool HasUngroupedTabs(Browser* browser) {
  if (!browser) {
    return false;
  }

  auto* tsm = browser->tab_strip_model();
  for (int i = 0; i < tsm->GetTabCount(); ++i) {
    if (!tsm->GetTabGroupForTab(i)) {
      return true;
    }
  }
  return false;
}

void GroupUngroupedTabs(Browser* browser) {
  if (!browser) {
    return;
  }
  auto* tsm = browser->tab_strip_model();
  std::vector<int> group_indices;

  for (int i = 0; i < tsm->GetTabCount(); ++i) {
    if (tsm->GetTabGroupForTab(i)) {
      continue;
    }
    group_indices.push_back(i);
  }

  if (group_indices.empty()) {
    return;
  }

  tsm->AddToNewGroup(group_indices);
}

void UngroupCurrentGroup(Browser* browser) {
  if (!browser) {
    return;
  }
  auto* tsm = browser->tab_strip_model();

  auto group_id = tsm->GetTabGroupForTab(tsm->active_index());
  if (!group_id) {
    return;
  }

  auto* group = tsm->group_model()->GetTabGroup(group_id.value());
  std::vector<int> indices(group->tab_count());
  std::iota(indices.begin(), indices.end(), group->GetFirstTab().value());
  tsm->RemoveFromGroup(indices);
}

void RemoveTabFromGroup(Browser* browser) {
  if (!browser) {
    return;
  }
  auto* tsm = browser->tab_strip_model();
  tsm->RemoveFromGroup({tsm->active_index()});
}

void NameGroup(Browser* browser) {
  if (!browser) {
    return;
  }

  auto* tsm = browser->tab_strip_model();
  auto group_id = tsm->GetTabGroupForTab(tsm->active_index());
  if (!group_id) {
    return;
  }

  tsm->OpenTabGroupEditor(*group_id);
}

void NewTabInGroup(Browser* browser) {
  if (!browser) {
    return;
  }

  auto* tsm = browser->tab_strip_model();
  auto group_id = tsm->GetTabGroupForTab(tsm->active_index());
  if (!group_id) {
    return;
  }

  const auto tabs = tsm->group_model()->GetTabGroup(*group_id)->ListTabs();
  tsm->delegate()->AddTabAt(GURL(), tabs.end(), true, *group_id);
}

bool CanUngroupAllTabs(Browser* browser) {
  if (!browser) {
    return false;
  }
  auto* tsm = browser->tab_strip_model();
  for (int i = 0; i < tsm->GetTabCount(); ++i) {
    if (tsm->GetTabGroupForTab(i)) {
      return true;
    }
  }
  return false;
}

void UngroupAllTabs(Browser* browser) {
  if (!browser) {
    return;
  }

  std::vector<int> indices(browser->tab_strip_model()->GetTabCount());
  std::iota(indices.begin(), indices.end(), 0);
  browser->tab_strip_model()->RemoveFromGroup(indices);
}

void ToggleGroupExpanded(Browser* browser) {
  if (!browser) {
    return;
  }
  auto* tsm = browser->tab_strip_model();
  auto group_id = tsm->GetTabGroupForTab(tsm->active_index());
  if (!group_id) {
    return;
  }

  auto* group = tsm->group_model()->GetTabGroup(*group_id);
  auto* vd = group->visual_data();
  tab_groups::TabGroupVisualData vd_update(vd->title(), vd->color(),
                                           !vd->is_collapsed());
  group->SetVisualData(vd_update);
}

void CloseUngroupedTabs(Browser* browser) {
  if (!browser) {
    return;
  }
  auto* tsm = static_cast<LuxxleTabStripModel*>(browser->tab_strip_model());
  CHECK(tsm);

  std::vector<int> indices;

  for (int i = tsm->GetTabCount() - 1; i >= 0; --i) {
    if (!tsm->GetTabGroupForTab(i)) {
      indices.push_back(i);
    }
  }

  for (const auto& index : indices) {
    tsm->CloseWebContentsAt(index,
                            TabCloseTypes::CLOSE_USER_GESTURE |
                                TabCloseTypes::CLOSE_CREATE_HISTORICAL_TAB);
  }
}

void CloseTabsNotInCurrentGroup(Browser* browser) {
  if (!browser) {
    return;
  }

  auto* tsm = static_cast<LuxxleTabStripModel*>(browser->tab_strip_model());
  CHECK(tsm);

  auto group_id = tsm->GetTabGroupForTab(tsm->active_index());
  if (!group_id) {
    return;
  }

  std::vector<int> indices;
  for (int i = tsm->GetTabCount() - 1; i >= 0; --i) {
    if (tsm->GetTabGroupForTab(i) != *group_id) {
      indices.push_back(i);
    }
  }

  for (const auto& index : indices) {
    tsm->CloseWebContentsAt(index,
                            TabCloseTypes::CLOSE_USER_GESTURE |
                                TabCloseTypes::CLOSE_CREATE_HISTORICAL_TAB);
  }
}

void CloseGroup(Browser* browser) {
  if (!browser) {
    return;
  }

  auto* tsm = browser->tab_strip_model();
  auto group_id = tsm->GetTabGroupForTab(tsm->active_index());
  if (!group_id) {
    return;
  }
  tsm->CloseAllTabsInGroup(*group_id);
}

bool CanBringAllTabs(Browser* browser) {
  if (!browser) {
    return false;
  }

  return std::ranges::any_of(
      *BrowserList::GetInstance(),
      [&](const Browser* from) { return CanTakeTabs(from, browser); });
}

void BringAllTabs(Browser* browser) {
  if (!browser) {
    return;
  }

  // Find all browsers with the same profile
  std::vector<Browser*> browsers;
  base::flat_set<Browser*> browsers_to_close;
  std::ranges::copy_if(
      *BrowserList::GetInstance(), std::back_inserter(browsers),
      [&](const Browser* from) { return CanTakeTabs(from, browser); });

  // Detach all tabs from other browsers
  std::stack<std::unique_ptr<tabs::TabModel>> detached_pinned_tabs;
  std::stack<std::unique_ptr<tabs::TabModel>> detached_unpinned_tabs;

  const bool shared_pinned_tab_enabled =
      base::FeatureList::IsEnabled(tabs::features::kLuxxleSharedPinnedTabs) &&
      browser->profile()->GetPrefs()->GetBoolean(luxxle_tabs::kSharedPinnedTab);

  std::ranges::for_each(browsers, [&detached_pinned_tabs,
                                   &detached_unpinned_tabs, &browsers_to_close,
                                   shared_pinned_tab_enabled](auto* other) {
    static_cast<LuxxleBrowser*>(other)
        ->set_ignore_enable_closing_last_tab_pref();

    auto* tab_strip_model = other->tab_strip_model();
    const int pinned_tab_count = tab_strip_model->IndexOfFirstNonPinnedTab();
    for (int i = tab_strip_model->count() - 1; i >= 0; --i) {
      const bool is_pinned = i < pinned_tab_count;
      if (is_pinned && shared_pinned_tab_enabled) {
        // SharedPinnedTabService is responsible for synchronizing pinned
        // tabs, thus we shouldn't manually detach and attach tabs here.
        // Meanwhile, the tab strips don't get empty when they have dummy
        // contents, we should close the browsers manually.
        browsers_to_close.insert(other);
        continue;
      }

      auto tab = tab_strip_model->DetachTabAtForInsertion(i);
      if (is_pinned) {
        detached_pinned_tabs.push(std::move(tab));
      } else {
        detached_unpinned_tabs.push(std::move(tab));
      }
    }
  });

  // Insert pinned tabs
  auto* tab_strip_model = browser->tab_strip_model();
  while (!detached_pinned_tabs.empty()) {
    tab_strip_model->InsertDetachedTabAt(
        tab_strip_model->IndexOfFirstNonPinnedTab(),
        std::move(detached_pinned_tabs.top()), AddTabTypes::ADD_PINNED);
    detached_pinned_tabs.pop();
  }

  // Insert unpinned tabs
  while (!detached_unpinned_tabs.empty()) {
    tab_strip_model->InsertDetachedTabAt(
        tab_strip_model->count(), std::move(detached_unpinned_tabs.top()),
        AddTabTypes::ADD_NONE);
    detached_unpinned_tabs.pop();
  }

  if (shared_pinned_tab_enabled) {
    std::ranges::for_each(browsers_to_close,
                          [](auto* other) { other->window()->Close(); });
  }
}

bool HasDuplicateTabs(Browser* browser) {
  if (!browser) {
    return false;
  }

  auto* tsm = browser->tab_strip_model();
  auto* active_web_contents = tsm->GetActiveWebContents();
  if (!active_web_contents) {
    return false;
  }

  auto url = active_web_contents->GetVisibleURL();
  for (int i = 0; i < tsm->GetTabCount(); ++i) {
    // Don't check the active tab.
    if (tsm->active_index() == i) {
      continue;
    }

    auto* tab = tsm->GetWebContentsAt(i);
    if (tab->GetVisibleURL() == url) {
      return true;
    }
  }

  return false;
}

void CloseDuplicateTabs(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  auto url = tsm->GetActiveWebContents()->GetVisibleURL();

  for (int i = tsm->GetTabCount() - 1; i >= 0; --i) {
    // Don't close the active tab.
    if (tsm->active_index() == i) {
      continue;
    }

    auto* tab = tsm->GetWebContentsAt(i);
    if (tab->GetVisibleURL() == url) {
      tab->Close();
    }
  }
}

bool CanCloseTabsToLeft(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  const auto& selection = tsm->selection_model();
  if (selection.empty()) {
    return false;
  }

  int left_selected = *(selection.selected_indices().begin());
  return left_selected > 0;
}

void CloseTabsToLeft(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  const auto& selection = tsm->selection_model();
  if (selection.empty()) {
    return;
  }

  int left_selected = *(selection.selected_indices().begin());
  for (int i = left_selected - 1; i >= 0; --i) {
    tsm->CloseWebContentsAt(i, TabCloseTypes::CLOSE_CREATE_HISTORICAL_TAB |
                                   TabCloseTypes::CLOSE_USER_GESTURE);
  }
}

bool CanCloseUnpinnedTabs(Browser* browser) {
  auto first_unpinned_index =
      browser->tab_strip_model()->IndexOfFirstNonPinnedTab();
  return first_unpinned_index < browser->tab_strip_model()->count();
}

void CloseUnpinnedTabs(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  DCHECK(CanCloseUnpinnedTabs(browser));

  for (int i = tsm->count() - 1; i >= tsm->IndexOfFirstNonPinnedTab(); --i) {
    tsm->CloseWebContentsAt(i, TabCloseTypes::CLOSE_CREATE_HISTORICAL_TAB |
                                   TabCloseTypes::CLOSE_USER_GESTURE);
  }
}

void AddAllTabsToNewGroup(Browser* browser) {
  std::vector<int> indices(browser->tab_strip_model()->count());
  std::iota(indices.begin(), indices.end(), 0);
  browser->tab_strip_model()->AddToNewGroup(indices);
}

bool CanMuteAllTabs(Browser* browser, bool exclude_active) {
  auto* tsm = browser->tab_strip_model();
  for (int i = 0; i < tsm->count(); ++i) {
    if (exclude_active && tsm->active_index() == i) {
      continue;
    }

    auto* contents = tsm->GetWebContentsAt(i);
    if (contents->IsCurrentlyAudible()) {
      return true;
    }
  }
  return false;
}

void MuteAllTabs(Browser* browser, bool exclude_active) {
  auto* tsm = browser->tab_strip_model();
  for (int i = 0; i < tsm->count(); ++i) {
    if (exclude_active && tsm->active_index() == i) {
      continue;
    }

    auto* contents = tsm->GetWebContentsAt(i);
    if (contents->IsCurrentlyAudible()) {
      contents->SetAudioMuted(true);
    }
  }
}

bool CanUnmuteAllTabs(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  for (int i = 0; i < tsm->count(); ++i) {
    auto* contents = tsm->GetWebContentsAt(i);
    if (contents->IsAudioMuted()) {
      return true;
    }
  }
  return false;
}

void UnmuteAllTabs(Browser* browser) {
  auto* tsm = browser->tab_strip_model();
  for (int i = 0; i < tsm->count(); ++i) {
    auto* contents = tsm->GetWebContentsAt(i);
    if (contents->IsAudioMuted()) {
      contents->SetAudioMuted(false);
    }
  }
}

void ScrollTabToTop(Browser* browser) {
  auto* contents = browser->tab_strip_model()->GetActiveWebContents();
  contents->ScrollToTopOfDocument();
}

void ScrollTabToBottom(Browser* browser) {
  auto* contents = browser->tab_strip_model()->GetActiveWebContents();
  contents->ScrollToBottomOfDocument();
}

void ExportAllBookmarks(Browser* browser) {
  (new BookmarksExportListener(browser->profile()))->ShowFileDialog(browser);
}

void ToggleAllBookmarksButtonVisibility(Browser* browser) {
  auto* prefs = browser->profile()->GetPrefs();
  prefs->SetBoolean(
      luxxle::bookmarks::prefs::kShowAllBookmarksButton,
      !prefs->GetBoolean(luxxle::bookmarks::prefs::kShowAllBookmarksButton));
}

bool CanOpenNewSplitViewForTab(Browser* browser,
                               std::optional<tabs::TabHandle> tab) {
  auto* split_view_data = browser->GetFeatures().split_view_browser_data();
  if (!split_view_data) {
    return false;
  }

  if (browser->tab_strip_model()->empty()) {
    return false;
  }

  if (!tab) {
    tab = GetActiveTabHandle(browser);
  }

  return !split_view_data->IsTabTiled(*tab);
}

void NewSplitViewForTab(Browser* browser,
                        std::optional<tabs::TabHandle> tab,
                        const GURL& url) {
  auto* split_view_data = browser->GetFeatures().split_view_browser_data();
  if (!split_view_data) {
    return;
  }

  if (!CanOpenNewSplitViewForTab(browser, tab)) {
    return;
  }

  if (!tab) {
    tab = GetActiveTabHandle(browser);
  }

  auto* model = browser->tab_strip_model();
  const int tab_index = model->GetIndexOfTab(tab->Get());
  const int new_tab_index = model->IsTabPinned(tab_index)
                                ? model->IndexOfFirstNonPinnedTab()
                                : tab_index + 1;

  if (!url.is_valid()) {
    chrome::AddTabAt(browser, GURL("chrome://newtab"), new_tab_index,
                     /*foreground*/ true);
  } else {
    chrome::AddTabAt(browser, url, new_tab_index,
                     /*foreground*/ true);
  }

  split_view_data->TileTabs(
      {.first = model->GetTabAtIndex(tab_index)->GetHandle(),
       .second = model->GetTabAtIndex(new_tab_index)->GetHandle()});
}

void TileTabs(Browser* browser, const std::vector<int>& indices) {
  auto* split_view_data = browser->GetFeatures().split_view_browser_data();
  if (!split_view_data) {
    return;
  }

  if (indices.empty()) {
    return TileTabs(browser, GetSelectedIndices(browser));
  }

  CHECK_LE(indices.size(), 2u);
  CHECK(!indices.empty());
  if (indices.size() == 1) {
    auto* model = browser->tab_strip_model();
    int active_tab_index =
        model->GetIndexOfWebContents(model->GetActiveWebContents());
    CHECK_NE(indices[0], active_tab_index);
    auto new_indices = indices;
    new_indices.push_back(active_tab_index);
    return TileTabs(browser, new_indices);
  }

  auto* model = browser->tab_strip_model();
  auto tab1 = indices[0];
  auto tab2 = indices[1];
  CHECK(!split_view_data->IsTabTiled(model->GetTabAtIndex(tab1)->GetHandle()));
  CHECK(!split_view_data->IsTabTiled(model->GetTabAtIndex(tab2)->GetHandle()));

  if (tab2 < tab1) {
    std::swap(tab1, tab2);
  }

  split_view_data->TileTabs(
      {.first = model->GetTabAtIndex(tab1)->GetHandle(),
       .second = model->GetTabAtIndex(tab2)->GetHandle()});
}

void BreakTiles(Browser* browser, const std::vector<int>& indices) {
  auto* split_view_data = browser->GetFeatures().split_view_browser_data();
  if (!split_view_data) {
    return;
  }

  if (indices.empty()) {
    return BreakTiles(browser, GetSelectedIndices(browser));
  }

  auto* model = browser->tab_strip_model();
  for (auto index : indices) {
    // The tile could have already been broken from the earlier iteration.
    if (auto tab_handle = model->GetTabAtIndex(index)->GetHandle();
        split_view_data->IsTabTiled(tab_handle)) {
      split_view_data->BreakTile(tab_handle);
    }
  }
}

bool IsTabsTiled(Browser* browser, const std::vector<int>& indices) {
  auto* split_view_data = browser->GetFeatures().split_view_browser_data();
  if (!split_view_data) {
    return false;
  }

  if (browser->tab_strip_model()->empty()) {
    return false;
  }

  if (indices.empty()) {
    return IsTabsTiled(browser, GetSelectedIndices(browser));
  }

  auto* model = browser->tab_strip_model();

  return std::ranges::any_of(indices, [&](auto index) {
    return split_view_data->IsTabTiled(
        model->GetTabAtIndex(index)->GetHandle());
  });
}

bool CanTileTabs(Browser* browser, const std::vector<int>& indices) {
  auto* split_view_data = browser->GetFeatures().split_view_browser_data();
  if (!split_view_data) {
    return false;
  }

  if (browser->tab_strip_model()->empty()) {
    return false;
  }

  if (indices.empty()) {
    return CanTileTabs(browser, GetSelectedIndices(browser));
  }

  if (indices.size() != 2) {
    return false;
  }

  auto* model = browser->tab_strip_model();
  return std::ranges::none_of(indices, [&](auto index) {
    return split_view_data->IsTabTiled(
        model->GetTabAtIndex(index)->GetHandle());
  });
}

void SwapTabsInTile(Browser* browser) {
  auto* split_view_data = browser->GetFeatures().split_view_browser_data();
  if (!split_view_data) {
    return;
  }

  if (browser->tab_strip_model()->empty()) {
    return;
  }

  if (!IsTabsTiled(browser)) {
    return;
  }

  auto* model = browser->tab_strip_model();
  auto tab = model->GetActiveTab()->GetHandle();
  auto tile = *split_view_data->GetTile(tab);
  split_view_data->SwapTabsInTile(tile);

  model->MoveWebContentsAt(model->GetIndexOfTab(tile.second.Get()),
                           model->GetIndexOfTab(tile.first.Get()),
                           /*select_after_move*/ false);
}

}  // namespace luxxle
