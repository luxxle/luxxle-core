/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/frame/luxxle_browser_view.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "base/containers/contains.h"
#include "base/feature_list.h"
#include "base/functional/bind.h"
#include "base/task/sequenced_task_runner.h"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/browser/sparkle_buildflags.h"
#include "luxxle/browser/translate/luxxle_translate_utils.h"
#include "luxxle/browser/ui/luxxle_browser.h"
#include "luxxle/browser/ui/luxxle_rewards/rewards_panel_coordinator.h"
#include "luxxle/browser/ui/color/luxxle_color_id.h"
#include "luxxle/browser/ui/commands/accelerator_service.h"
#include "luxxle/browser/ui/commands/accelerator_service_factory.h"
#include "luxxle/browser/ui/page_action/luxxle_page_action_icon_type.h"
#include "luxxle/browser/ui/sidebar/sidebar_utils.h"
#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/tabs/split_view_browser_data.h"
#include "luxxle/browser/ui/views/luxxle_actions/luxxle_actions_container.h"
#include "luxxle/browser/ui/views/luxxle_actions/luxxle_shields_action_view.h"
#include "luxxle/browser/ui/views/luxxle_help_bubble/luxxle_help_bubble_host_view.h"
#include "luxxle/browser/ui/views/luxxle_shields/cookie_list_opt_in_bubble_host.h"
#include "luxxle/browser/ui/views/frame/luxxle_contents_layout_manager.h"
#include "luxxle/browser/ui/views/frame/luxxle_contents_view_util.h"
#include "luxxle/browser/ui/views/frame/vertical_tab_strip_region_view.h"
#include "luxxle/browser/ui/views/frame/vertical_tab_strip_widget_delegate_view.h"
#include "luxxle/browser/ui/views/location_bar/luxxle_location_bar_view.h"
#include "luxxle/browser/ui/views/omnibox/luxxle_omnibox_view_views.h"
#include "luxxle/browser/ui/views/sidebar/sidebar_container_view.h"
#include "luxxle/browser/ui/views/split_view/split_view.h"
#include "luxxle/browser/ui/views/tabs/vertical_tab_utils.h"
#include "luxxle/browser/ui/views/toolbar/bookmark_button.h"
#include "luxxle/browser/ui/views/toolbar/luxxle_toolbar_view.h"
#include "luxxle/browser/ui/views/toolbar/wallet_button.h"
#include "luxxle/browser/ui/views/window_closing_confirm_dialog_view.h"
#include "luxxle/components/commands/common/features.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/speedreader/common/buildflags/buildflags.h"
#include "luxxle/ui/color/nala/nala_color_id.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/app_mode/app_mode_utils.h"
#include "chrome/browser/devtools/devtools_window.h"
#include "chrome/browser/enterprise/watermark/watermark_view.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_window/public/browser_window_features.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/browser/ui/exclusive_access/exclusive_access_manager.h"
#include "chrome/browser/ui/exclusive_access/fullscreen_controller.h"
#include "chrome/browser/ui/frame/window_frame_util.h"
#include "chrome/browser/ui/ui_features.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/frame/contents_layout_manager.h"
#include "chrome/browser/ui/views/frame/contents_web_view.h"
#include "chrome/browser/ui/views/frame/multi_contents_view.h"
#include "chrome/browser/ui/views/frame/tab_strip_region_view.h"
#include "chrome/browser/ui/views/frame/top_container_view.h"
#include "chrome/browser/ui/views/side_panel/side_panel_coordinator.h"
#include "chrome/browser/ui/views/tabs/tab_search_button.h"
#include "chrome/browser/ui/views/tabs/tab_strip.h"
#include "chrome/browser/ui/views/toolbar/browser_app_menu_button.h"
#include "chrome/common/pref_names.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/web_contents.h"
#include "extensions/buildflags/buildflags.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/accelerators/accelerator_manager.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/compositor/layer.h"
#include "ui/events/event_observer.h"
#include "ui/gfx/geometry/rounded_corners_f.h"
#include "ui/views/border.h"
#include "ui/views/bubble/bubble_dialog_delegate_view.h"
#include "ui/views/controls/native/native_view_host.h"
#include "ui/views/event_monitor.h"
#include "ui/views/layout/fill_layout.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
#include "luxxle/browser/ui/views/toolbar/luxxle_vpn_button.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

#if BUILDFLAG(ENABLE_SPARKLE)
#include "luxxle/browser/ui/views/update_recommended_message_box_mac.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "luxxle/browser/speedreader/speedreader_tab_helper.h"
#include "luxxle/browser/ui/views/speedreader/reader_mode_bubble.h"
#include "luxxle/browser/ui/views/speedreader/reader_mode_toolbar_view.h"
#endif

#if BUILDFLAG(ENABLE_LUXXLE_WAYBACK_MACHINE)
#include "luxxle/browser/ui/views/wayback_machine_bubble_view.h"
#endif

namespace {

std::optional<bool> g_download_confirm_return_allow_for_testing;

bool IsUnsupportedCommand(int command_id, Browser* browser) {
  return IsRunningInForcedAppMode() &&
         !IsCommandAllowedInAppMode(command_id, browser->is_type_popup());
}

// A control separator that is displayed when the sidebar is displayed adjacent
// to the tabstrip in vertical tabs mode.
class SidebarSeparator : public views::View {
  METADATA_HEADER(SidebarSeparator, views::View)
 public:
  SidebarSeparator() {
    SetBackground(
        views::CreateSolidBackground(kColorLuxxleVerticalTabSeparator));
  }
};
BEGIN_METADATA(SidebarSeparator)
END_METADATA

// A view that paints a background under the content area of the browser view so
// that the web content area can be displayed with rounded corners and a shadow.
class ContentsBackground : public views::View {
  METADATA_HEADER(ContentsBackground, views::View)
 public:
  ContentsBackground() {
    SetBackground(views::CreateSolidBackground(kColorToolbar));
    SetEnabled(false);
  }
};
BEGIN_METADATA(ContentsBackground)
END_METADATA

}  // namespace

// static
void LuxxleBrowserView::SetDownloadConfirmReturnForTesting(bool allow) {
  g_download_confirm_return_allow_for_testing = allow;
}

class LuxxleBrowserView::TabCyclingEventHandler : public ui::EventObserver,
                                                 public views::WidgetObserver {
 public:
  explicit TabCyclingEventHandler(LuxxleBrowserView* browser_view)
      : browser_view_(browser_view) {
    Start();
  }

  ~TabCyclingEventHandler() override { Stop(); }

  TabCyclingEventHandler(const TabCyclingEventHandler&) = delete;
  TabCyclingEventHandler& operator=(const TabCyclingEventHandler&) = delete;

 private:
  // ui::EventObserver overrides:
  void OnEvent(const ui::Event& event) override {
    if (event.type() == ui::EventType::kKeyReleased &&
        event.AsKeyEvent()->key_code() == ui::VKEY_CONTROL) {
      // Ctrl key was released, stop the tab cycling
      Stop();
      return;
    }

    if (event.type() == ui::EventType::kMousePressed) {
      Stop();
    }
  }

  // views::WidgetObserver overrides:
  void OnWidgetActivationChanged(views::Widget* widget, bool active) override {
    // We should stop cycling if other application gets active state.
    if (!active) {
      Stop();
    }
  }

  // Handle Browser widget closing while tab Cycling is in-progress.
  void OnWidgetClosing(views::Widget* widget) override { Stop(); }

  void Start() {
    // Add the event handler
    auto* widget = browser_view_->GetWidget();
    if (widget->GetNativeWindow()) {
      monitor_ = views::EventMonitor::CreateWindowMonitor(
          this, widget->GetNativeWindow(),
          {ui::EventType::kMousePressed, ui::EventType::kKeyReleased});
    }

    widget->AddObserver(this);
  }

  void Stop() {
    if (!monitor_.get()) {
      // We already stopped
      return;
    }

    // Remove event handler
    auto* widget = browser_view_->GetWidget();
    monitor_.reset();
    widget->RemoveObserver(this);
    browser_view_->StopTabCycling();
  }

  raw_ptr<LuxxleBrowserView> browser_view_ = nullptr;
  std::unique_ptr<views::EventMonitor> monitor_;
};

// static
LuxxleBrowserView* LuxxleBrowserView::From(BrowserView* view) {
  return static_cast<LuxxleBrowserView*>(view);
}

LuxxleBrowserView::LuxxleBrowserView(std::unique_ptr<Browser> browser)
    : BrowserView(std::move(browser)) {
  const bool use_rounded_corners =
      LuxxleBrowser::ShouldUseLuxxleWebViewRoundedCorners(browser_.get());
#if BUILDFLAG(ENABLE_SPEEDREADER)
  reader_mode_toolbar_ =
      contents_container_->AddChildView(std::make_unique<ReaderModeToolbarView>(
          browser_->profile(), use_rounded_corners));

  views::View* contents_view = contents_web_view_;
  // MultiContentsView is contents view with SideBySide feature.
  if (base::FeatureList::IsEnabled(features::kSideBySide)) {
    contents_view = multi_contents_view_;
  }
  CHECK(contents_view);
  contents_container_->SetLayoutManager(
      std::make_unique<LuxxleContentsLayoutManager>(
          devtools_web_view(), devtools_scrim_view(), contents_view,
          lens_overlay_view_, contents_scrim_view(), /*border_view*/ nullptr,
          watermark_view_.get(), reader_mode_toolbar_));
#endif

  if (use_rounded_corners) {
    // Collapse the separator line between the toolbar or bookmark bar and the
    // views below.
    contents_separator_->SetPreferredSize(gfx::Size());
    contents_shadow_ = LuxxleContentsViewUtil::CreateShadow(contents_container_);
    contents_background_view_ =
        AddChildView(std::make_unique<ContentsBackground>());
  }

  pref_change_registrar_.Init(GetProfile()->GetPrefs());
  pref_change_registrar_.Add(
      kTabsSearchShow,
      base::BindRepeating(&LuxxleBrowserView::OnPreferenceChanged,
                          base::Unretained(this)));
  // Show the correct value in settings on initial start
  UpdateSearchTabsButtonState();

  auto* rewards_service =
      luxxle_rewards::RewardsServiceFactory::GetForProfile(browser_->profile());
  if (rewards_service) {
    luxxle_rewards::RewardsPanelCoordinator::CreateForBrowser(browser_.get());
  }

  luxxle_shields::CookieListOptInBubbleHost::MaybeCreateForBrowser(
      browser_.get());

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  pref_change_registrar_.Add(
      luxxle_vpn::prefs::kLuxxleVPNShowButton,
      base::BindRepeating(&LuxxleBrowserView::OnPreferenceChanged,
                          base::Unretained(this)));
#endif

  // Only normal window (tabbed) should have sidebar.
  const bool can_have_sidebar = sidebar::CanUseSidebar(browser_.get());
  if (can_have_sidebar) {
    // Wrap chromium side panel with our sidebar container
    auto original_side_panel = RemoveChildViewT(unified_side_panel_.get());
    sidebar_container_view_ =
        AddChildView(std::make_unique<SidebarContainerView>(
            browser_.get(), browser_->GetFeatures().side_panel_coordinator(),
            std::move(original_side_panel)));
    unified_side_panel_ = sidebar_container_view_->side_panel();

    if (LuxxleBrowser::ShouldUseLuxxleWebViewRoundedCorners(browser_.get())) {
      sidebar_separator_view_ =
          AddChildView(std::make_unique<SidebarSeparator>());
    }

#if defined(USE_AURA)
    sidebar_host_view_ = AddChildView(std::make_unique<views::View>());
#endif

    pref_change_registrar_.Add(
        prefs::kSidePanelHorizontalAlignment,
        base::BindRepeating(&LuxxleBrowserView::OnPreferenceChanged,
                            base::Unretained(this)));
  }

  if (tabs::features::IsLuxxleSplitViewEnabled() && browser_->is_type_normal()) {
    split_view_ =
        contents_container_->parent()->AddChildView(std::make_unique<SplitView>(
            *browser_, contents_container_, contents_web_view_));
    set_contents_view(split_view_);
  }

  const bool supports_vertical_tabs =
      tabs::utils::SupportsVerticalTabs(browser_.get());
  if (supports_vertical_tabs) {
    vertical_tab_strip_host_view_ =
        AddChildView(std::make_unique<views::View>());
  }

  if (!supports_vertical_tabs && !can_have_sidebar) {
    return;
  }

  // Make sure |find_bar_host_view_| is the last child of BrowserView by
  // re-ordering. FindBarHost widgets uses this view as a  kHostViewKey.
  // See the comments of BrowserView::find_bar_host_view().
  ReorderChildView(find_bar_host_view_, -1);
}

views::View* LuxxleBrowserView::GetContentsBoundingView() const {
  return contents_view();
}

void LuxxleBrowserView::OnPreferenceChanged(const std::string& pref_name) {
  if (pref_name == kTabsSearchShow) {
    UpdateSearchTabsButtonState();
    return;
  }

  if (pref_name == prefs::kSidePanelHorizontalAlignment) {
    UpdateSideBarHorizontalAlignment();
    return;
  }

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  if (pref_name == luxxle_vpn::prefs::kLuxxleVPNShowButton) {
    vpn_panel_controller_.ResetBubbleManager();
    return;
  }
#endif
}

void LuxxleBrowserView::UpdateSideBarHorizontalAlignment() {
  DCHECK(sidebar_container_view_);

  const bool on_left = !GetProfile()->GetPrefs()->GetBoolean(
      prefs::kSidePanelHorizontalAlignment);

  sidebar_container_view_->SetSidebarOnLeft(on_left);

  DeprecatedLayoutImmediately();
}

void LuxxleBrowserView::UpdateSearchTabsButtonState() {
  if (auto* tab_search_button = tab_strip_region_view_->GetTabSearchButton()) {
    auto is_tab_search_visible =
        GetProfile()->GetPrefs()->GetBoolean(kTabsSearchShow);
    tab_search_button->SetVisible(is_tab_search_visible);
  }
}

LuxxleBrowserView::~LuxxleBrowserView() {
  tab_cycling_event_handler_.reset();
  // Removes the bubble from the browser, as it uses the `ToolbarView` as an
  // archor, and that leaves a dangling reference once the `TopContainerView` is
  // destroyed before all `SupportsUserData` is cleared.
  if (luxxle_shields::CookieListOptInBubbleHost::FromBrowser(browser_.get())) {
    luxxle_shields::CookieListOptInBubbleHost::RemoveFromBrowser(browser_.get());
  }

  DCHECK(!tab_cycling_event_handler_);
}

sidebar::Sidebar* LuxxleBrowserView::InitSidebar() {
  // Start Sidebar UI initialization.
  DCHECK(sidebar_container_view_);
  sidebar_container_view_->Init();
  UpdateSideBarHorizontalAlignment();
  return sidebar_container_view_;
}

void LuxxleBrowserView::ToggleSidebar() {
  browser_->GetFeatures().side_panel_ui()->Toggle();
}

void LuxxleBrowserView::ShowLuxxleVPNBubble(bool show_select) {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  vpn_panel_controller_.ShowLuxxleVPNPanel(show_select);
#endif
}

views::View* LuxxleBrowserView::GetAnchorViewForLuxxleVPNPanel() {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  auto* vpn_button =
      static_cast<LuxxleToolbarView*>(toolbar())->luxxle_vpn_button();
  if (vpn_button->GetVisible()) {
    return vpn_button;
  }
  return toolbar()->app_menu_button();
#else
  return nullptr;
#endif
}

gfx::Rect LuxxleBrowserView::GetShieldsBubbleRect() {
  auto* luxxle_location_bar_view =
      static_cast<LuxxleLocationBarView*>(GetLocationBarView());
  if (!luxxle_location_bar_view) {
    return gfx::Rect();
  }

  auto* shields_action_view =
      luxxle_location_bar_view->luxxle_actions_contatiner_view()
          ->GetShieldsActionView();
  if (!shields_action_view) {
    return gfx::Rect();
  }

  auto* bubble_widget = shields_action_view->GetBubbleWidget();
  if (!bubble_widget) {
    return gfx::Rect();
  }

  return bubble_widget->GetClientAreaBoundsInScreen();
}

bool LuxxleBrowserView::GetTabStripVisible() const {
  if (tabs::utils::ShouldShowVerticalTabs(browser())) {
    return false;
  }

  return BrowserView::GetTabStripVisible();
}

#if BUILDFLAG(IS_WIN)
bool LuxxleBrowserView::GetSupportsTitle() const {
  if (tabs::utils::SupportsVerticalTabs(browser())) {
    return true;
  }

  return BrowserView::GetSupportsTitle();
}
#endif

void LuxxleBrowserView::SetStarredState(bool is_starred) {
  LuxxleBookmarkButton* button =
      static_cast<LuxxleToolbarView*>(toolbar())->bookmark_button();
  if (button) {
    button->SetToggled(is_starred);
  }
}

#if BUILDFLAG(ENABLE_SPEEDREADER)

speedreader::SpeedreaderBubbleView* LuxxleBrowserView::ShowSpeedreaderBubble(
    speedreader::SpeedreaderTabHelper* tab_helper,
    speedreader::SpeedreaderBubbleLocation location) {
  views::View* anchor = nullptr;
  views::BubbleBorder::Arrow arrow = views::BubbleBorder::NONE;
  switch (location) {
    case speedreader::SpeedreaderBubbleLocation::kLocationBar:
      anchor = GetLocationBarView();
      arrow = views::BubbleBorder::TOP_RIGHT;
      break;
    case speedreader::SpeedreaderBubbleLocation::kToolbar:
      anchor = reader_mode_toolbar_->toolbar();
      arrow = views::BubbleBorder::TOP_LEFT;
      break;
  }

  auto* reader_mode_bubble =
      new speedreader::ReaderModeBubble(anchor, tab_helper);
  views::BubbleDialogDelegateView::CreateBubble(reader_mode_bubble);
  reader_mode_bubble->SetArrow(arrow);
  reader_mode_bubble->Show();
  return reader_mode_bubble;
}

void LuxxleBrowserView::UpdateReaderModeToolbar() {
  auto is_distilled = [](content::WebContents* web_contents) {
    if (!web_contents) {
      return false;
    }
    if (auto* th =
            speedreader::SpeedreaderTabHelper::FromWebContents(web_contents)) {
      return speedreader::DistillStates::IsDistilled(th->PageDistillState());
    }
    return false;
  };
  reader_mode_toolbar_->SetVisible(
      is_distilled(browser()->tab_strip_model()->GetActiveWebContents()));
}
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)

void LuxxleBrowserView::ShowUpdateChromeDialog() {
#if BUILDFLAG(ENABLE_SPARKLE)
  // On mac, sparkle frameworks's relaunch api is used.
  UpdateRecommendedMessageBoxMac::Show(GetNativeWindow());
#else
  BrowserView::ShowUpdateChromeDialog();
#endif
}

bool LuxxleBrowserView::HasSelectedURL() const {
  if (!GetLocationBarView() || !GetLocationBarView()->HasFocus()) {
    return false;
  }
  auto* luxxle_omnibox_view =
      static_cast<LuxxleOmniboxViewViews*>(GetLocationBarView()->omnibox_view());
  return luxxle_omnibox_view && luxxle_omnibox_view->SelectedTextIsURL();
}

void LuxxleBrowserView::CleanAndCopySelectedURL() {
  if (!GetLocationBarView()) {
    return;
  }
  auto* luxxle_omnibox_view =
      static_cast<LuxxleOmniboxViewViews*>(GetLocationBarView()->omnibox_view());
  if (!luxxle_omnibox_view) {
    return;
  }
  luxxle_omnibox_view->CleanAndCopySelectedURL();
}

#if BUILDFLAG(ENABLE_PLAYLIST_WEBUI)
void LuxxleBrowserView::ShowPlaylistBubble() {
  static_cast<LuxxleLocationBarView*>(GetLocationBarView())
      ->ShowPlaylistBubble();
}
#endif

#if BUILDFLAG(ENABLE_LUXXLE_WAYBACK_MACHINE)
void LuxxleBrowserView::ShowWaybackMachineBubble() {
  if (auto* anchor = toolbar_button_provider_->GetPageActionIconView(
          luxxle::kWaybackMachineActionIconType)) {
    DCHECK(anchor->GetVisible());
    // Launch bubble with this anchor.
    WaybackMachineBubbleView::Show(browser(), anchor);
  }
}
#endif

WalletButton* LuxxleBrowserView::GetWalletButton() {
  return static_cast<LuxxleToolbarView*>(toolbar())->wallet_button();
}

void LuxxleBrowserView::NotifyDialogPositionRequiresUpdate() {
  GetBrowserViewLayout()->NotifyDialogPositionRequiresUpdate();
}

views::View* LuxxleBrowserView::GetWalletButtonAnchorView() {
  return static_cast<LuxxleToolbarView*>(toolbar())
      ->wallet_button()
      ->GetAsAnchorView();
}

void LuxxleBrowserView::OnAcceleratorsChanged(
    const commands::Accelerators& changed) {
  DCHECK(base::FeatureList::IsEnabled(commands::features::kLuxxleCommands));

  auto* focus_manager = GetFocusManager();
  DCHECK(focus_manager);

  for (const auto& [command_id, accelerators] : changed) {
    if (IsUnsupportedCommand(command_id, browser())) {
      continue;
    }

    std::vector<ui::Accelerator> old_accelerators;
    for (const auto& [accelerator, accelerator_command] : accelerator_table_) {
      if (accelerator_command != command_id) {
        continue;
      }
      old_accelerators.push_back(accelerator);
    }

    // Register current accelerators
    for (const auto& accelerator : accelerators) {
      if (focus_manager->IsAcceleratorRegistered(accelerator)) {
        focus_manager->UnregisterAccelerator(accelerator, this);
      }

      focus_manager->RegisterAccelerator(
          accelerator, ui::AcceleratorManager::kNormalPriority, this);
      accelerator_table_[accelerator] = command_id;
    }

    // Unregister removed accelerators
    for (const auto& old_accelerator : old_accelerators) {
      if (base::Contains(accelerators, old_accelerator)) {
        continue;
      }
      focus_manager->UnregisterAccelerator(old_accelerator, this);
      accelerator_table_.erase(old_accelerator);
    }
  }
}

void LuxxleBrowserView::CreateWalletBubble() {
  DCHECK(GetWalletButton());
  GetWalletButton()->ShowWalletBubble();
}

void LuxxleBrowserView::CreateApproveWalletBubble() {
  DCHECK(GetWalletButton());
  GetWalletButton()->ShowApproveWalletBubble();
}

void LuxxleBrowserView::CloseWalletBubble() {
  if (GetWalletButton()) {
    GetWalletButton()->CloseWalletBubble();
  }
}

void LuxxleBrowserView::AddedToWidget() {
  BrowserView::AddedToWidget();
  // we must call all new views once LuxxleBrowserView is added to widget

  GetBrowserViewLayout()->set_contents_background(contents_background_view_);
  GetBrowserViewLayout()->set_sidebar_container(sidebar_container_view_);
  GetBrowserViewLayout()->set_sidebar_separator(sidebar_separator_view_);

  UpdateWebViewRoundedCorners();

  if (vertical_tab_strip_host_view_) {
    vertical_tab_strip_widget_ = VerticalTabStripWidgetDelegateView::Create(
        this, vertical_tab_strip_host_view_);
    vertical_tab_strip_widget_delegate_view_ =
        static_cast<VerticalTabStripWidgetDelegateView*>(
            vertical_tab_strip_widget_->widget_delegate());

    // By setting this property to the widget for vertical tabs,
    // BrowserView::GetBrowserViewForNativeWindow() will return browser view
    // properly even when we pass the native window for vertical tab strip.
    // As a result, we don't have to call GetTopLevelWidget() in order to
    // get browser view from the vertical tab strip's widget.
    SetNativeWindowPropertyForWidget(vertical_tab_strip_widget_.get());

    GetBrowserViewLayout()->set_vertical_tab_strip_host(
        vertical_tab_strip_host_view_.get());
  }
}

bool LuxxleBrowserView::ShowLuxxleHelpBubbleView(const std::string& text) {
  auto* shields_action_view =
      static_cast<LuxxleLocationBarView*>(GetLocationBarView())
          ->luxxle_actions_contatiner_view()
          ->GetShieldsActionView();
  if (!shields_action_view || !shields_action_view->GetVisible()) {
    return false;
  }

  // When help bubble is closed, this host view gets hidden.
  // For now, this help bubble host view is only used for shield icon, but it
  // could be re-used for other icons or views in the future.
  if (!luxxle_help_bubble_host_view_) {
    luxxle_help_bubble_host_view_ =
        AddChildView(std::make_unique<LuxxleHelpBubbleHostView>());
  }
  luxxle_help_bubble_host_view_->set_text(text);
  luxxle_help_bubble_host_view_->set_tracked_element(shields_action_view);
  return luxxle_help_bubble_host_view_->Show();
}

void LuxxleBrowserView::LoadAccelerators() {
  if (base::FeatureList::IsEnabled(commands::features::kLuxxleCommands)) {
    auto* accelerator_service =
        commands::AcceleratorServiceFactory::GetForContext(
            browser()->profile());
    if (accelerator_service) {
      accelerators_observation_.Observe(accelerator_service);
      return;
    }
  }
  BrowserView::LoadAccelerators();
}

void LuxxleBrowserView::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  BrowserView::OnTabStripModelChanged(tab_strip_model, change, selection);

  if (change.type() != TabStripModelChange::kSelectionOnly) {
    // Stop tab cycling if tab is closed dusing the cycle.
    // This can happen when tab is closed by shortcut (ex, ctrl + F4).
    // After stopping, current tab cycling, new tab cycling will be started.
    StopTabCycling();
  }

  if (selection.active_tab_changed() && luxxle_help_bubble_host_view_ &&
      luxxle_help_bubble_host_view_->GetVisible()) {
    luxxle_help_bubble_host_view_->Hide();
  }
}

views::CloseRequestResult LuxxleBrowserView::OnWindowCloseRequested() {
  if (GetLuxxleBrowser()->ShouldAskForBrowserClosingBeforeHandlers()) {
    if (!closing_confirm_dialog_activated_) {
      WindowClosingConfirmDialogView::Show(
          browser(),
          base::BindOnce(&LuxxleBrowserView::OnWindowClosingConfirmResponse,
                         weak_ptr_.GetWeakPtr()));
      closing_confirm_dialog_activated_ = true;
    }
    return views::CloseRequestResult::kCannotClose;
  }

  return BrowserView::OnWindowCloseRequested();
}

void LuxxleBrowserView::OnWindowClosingConfirmResponse(bool allowed_to_close) {
  DCHECK(closing_confirm_dialog_activated_);
  closing_confirm_dialog_activated_ = false;

  auto* browser = GetLuxxleBrowser();
  // Set to Browser instance because Browser instance knows about the result
  // of any warning handlers or beforeunload handlers.
  browser->set_confirmed_to_close(allowed_to_close);
  if (allowed_to_close) {
    // Start close window again as user allowed to close it.
    // Confirm dialog will not be launched for this closing request
    // as we set LuxxleBrowser::confirmed_to_closed_window_ to true.
    // If user cancels this window closing via additional warnings
    // or beforeunload handler, this dialog will be shown again.
    chrome::CloseWindow(browser);
  }
}

void LuxxleBrowserView::ConfirmBrowserCloseWithPendingDownloads(
    int download_count,
    Browser::DownloadCloseType dialog_type,
    base::OnceCallback<void(bool)> callback) {
  // Simulate user response.
  if (g_download_confirm_return_allow_for_testing) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE,
        base::BindOnce(std::move(callback),
                       *g_download_confirm_return_allow_for_testing));
    return;
  }
  BrowserView::ConfirmBrowserCloseWithPendingDownloads(
      download_count, dialog_type, std::move(callback));
}

void LuxxleBrowserView::MaybeShowReadingListInSidePanelIPH() {
  // Do nothing.
}

void LuxxleBrowserView::UpdateDevToolsForContents(
    content::WebContents* web_contents,
    bool update_devtools_web_contents) {
  CHECK(!web_contents || web_contents == GetActiveWebContents())
      << "This method is supposed to be called only for the active web "
         "contents";

  if (split_view_) {
    split_view_->WillUpdateDevToolsForActiveContents({});
  }

  BrowserView::UpdateDevToolsForContents(web_contents,
                                         update_devtools_web_contents);

  if (split_view_) {
    split_view_->DidUpdateDevToolsForActiveContents({});
  }

  UpdateWebViewRoundedCorners();
}

void LuxxleBrowserView::OnWidgetActivationChanged(views::Widget* widget,
                                                 bool active) {
  BrowserView::OnWidgetActivationChanged(widget, active);

  // For updating sidebar's item state.
  // As we can activate other window's Talk tab with current window's sidebar
  // Talk item, sidebar Talk item should have activated state if other windows
  // have Talk tab. It would be complex to get updated when Talk tab is opened
  // from other windows. So, simply trying to update when window activation
  // state is changed. With this, active window could have correct sidebar
  // item state.
  if (sidebar_container_view_) {
    sidebar_container_view_->UpdateSidebarItemsState();
  }
}

void LuxxleBrowserView::GetAccessiblePanes(std::vector<views::View*>* panes) {
  BrowserView::GetAccessiblePanes(panes);

  if (split_view_) {
    split_view_->GetAccessiblePanes({}, panes);
  }
}

void LuxxleBrowserView::ShowSplitView() {
  BrowserView::ShowSplitView();

  UpdateContentsSeparatorVisibility();
}

void LuxxleBrowserView::HideSplitView() {
  BrowserView::HideSplitView();

  UpdateContentsSeparatorVisibility();
}

bool LuxxleBrowserView::ShouldShowWindowTitle() const {
  if (BrowserView::ShouldShowWindowTitle()) {
    return true;
  }

  if (tabs::utils::ShouldShowWindowTitleForVerticalTabs(browser())) {
    return true;
  }

  return false;
}

void LuxxleBrowserView::OnThemeChanged() {
  BrowserView::OnThemeChanged();
  if (vertical_tab_strip_host_view_) {
    const auto background_color = GetColorProvider()->GetColor(kColorToolbar);
    vertical_tab_strip_host_view_->SetBackground(
        views::CreateSolidBackground(background_color));
  }
}

TabSearchBubbleHost* LuxxleBrowserView::GetTabSearchBubbleHost() {
  if (!tabs::utils::ShouldShowVerticalTabs(browser())) {
    return BrowserView::GetTabSearchBubbleHost();
  }

  return vertical_tab_strip_widget_delegate_view_
      ->vertical_tab_strip_region_view()
      ->GetTabSearchBubbleHost();
}

void LuxxleBrowserView::OnActiveTabChanged(content::WebContents* old_contents,
                                          content::WebContents* new_contents,
                                          int index,
                                          int reason) {
  // Update separator visibility first before starting split view layout
  // to give their final position.
  UpdateContentsSeparatorVisibility();

  if (split_view_) {
    split_view_->WillChangeActiveWebContents(
        /*passkey*/ {}, old_contents, new_contents);
  }

  BrowserView::OnActiveTabChanged(old_contents, new_contents, index, reason);

  if (split_view_) {
    split_view_->DidChangeActiveWebContents(
        /*passkey*/ {}, old_contents, new_contents);
  }

#if BUILDFLAG(ENABLE_SPEEDREADER)
  UpdateReaderModeToolbar();
#endif
}

void LuxxleBrowserView::UpdateContentsSeparatorVisibility() {
  // It's not shown with rounded corners mode always.
  if (LuxxleBrowser::ShouldUseLuxxleWebViewRoundedCorners(browser_.get())) {
    return;
  }

  // Control its visibility by changing its preferred size as layout manager
  // refers it's preferred size.
  // Don't show that separator as split view has border around contents
  // container.
  if ((split_view_ && split_view_->IsSplitViewActive()) ||
      (multi_contents_view_ && multi_contents_view_->IsInSplitView())) {
    contents_separator_->SetPreferredSize({});
    return;
  }
  contents_separator_->SetPreferredSize(
      gfx::Size(views::Separator::kThickness, views::Separator::kThickness));
}

bool LuxxleBrowserView::AcceleratorPressed(const ui::Accelerator& accelerator) {
  if (base::FeatureList::IsEnabled(tabs::features::kLuxxleSharedPinnedTabs) &&
      browser()->profile()->GetPrefs()->GetBoolean(
          luxxle_tabs::kSharedPinnedTab)) {
    if (int command_id; FindCommandIdForAccelerator(accelerator, &command_id) &&
                        command_id == IDC_CLOSE_TAB) {
      auto* tab_strip_model = browser()->tab_strip_model();
      if (tab_strip_model->IsTabPinned(tab_strip_model->active_index())) {
        // Ignore CLOSE TAB command via accelerator if the tab is shared/dummy
        // pinned tab.
        return true;
      }
    }
  }
  return BrowserView::AcceleratorPressed(accelerator);
}

bool LuxxleBrowserView::IsInTabDragging() const {
  return frame()->tab_drag_kind() == TabDragKind::kAllTabs;
}

views::View* LuxxleBrowserView::GetContentsContainerForLayoutManager() {
  // In split view, |split_view_| wraps primary and secondary contents and
  // it manages each content's bounds. So, BrowserViewLayoutManager only
  // need to manage |split_view_|'s bounds.
  return split_view_ ? split_view_
                     : BrowserView::GetContentsContainerForLayoutManager();
}

void LuxxleBrowserView::ReadyToListenFullscreenChanges() {
  if (split_view_) {
    split_view_->ListenFullscreenChanges();
  }
}

bool LuxxleBrowserView::IsSidebarVisible() const {
  return sidebar_container_view_ && sidebar_container_view_->IsSidebarVisible();
}

LuxxleBrowser* LuxxleBrowserView::GetLuxxleBrowser() const {
  return static_cast<LuxxleBrowser*>(browser_.get());
}

void LuxxleBrowserView::UpdateWebViewRoundedCorners() {
  if (!LuxxleBrowser::ShouldUseLuxxleWebViewRoundedCorners(browser_.get())) {
    return;
  }

  gfx::RoundedCornersF corners(LuxxleContentsViewUtil::kBorderRadius);

  // In fullscreen-for-tab mode (e.g. full-screen video), no corners should be
  // rounded.
  if (auto* exclusive_access_manager = GetExclusiveAccessManager()) {
    if (auto* controller = exclusive_access_manager->fullscreen_controller()) {
      if (controller->IsWindowFullscreenForTabOrPending()) {
        corners = gfx::RoundedCornersF(0);
      }
    }
  }

  // Set the appropriate corner radius for the view that contains both the web
  // contents and devtools.
  contents_container_->layer()->SetRoundedCornerRadius(corners);

  const auto in_split_view_mode =
      !!browser_->GetFeatures().split_view_browser_data();

  auto update_corner_radius =
      [in_split_view_mode](views::WebView* contents, views::WebView* devtools,
                           DevToolsDockedPlacement devtools_placement,
                           gfx::RoundedCornersF corners) {
        // In addition to giving the contents container rounded corners, we also
        // need to round the corners of the native view holder that displays the
        // web contents.

        // Devtools lies underneath the contents webview. Round all four
        // corners.
        if (devtools && devtools->holder()) {
          devtools->holder()->SetCornerRadii(corners);
        }

        if (!in_split_view_mode) {
          // In order to make the contents web view and devtools appear to be
          // contained within a single rounded-corner view, square the contents
          // webview corners that are adjacent to devtools.
          // TODO(sko) We need to override
          // BrowserView::GetDevToolsDockedPlacement(). It depends on coordinate
          // of it but in split view mode, the calculation is not correct.
          switch (devtools_placement) {
            case DevToolsDockedPlacement::kLeft:
              corners.set_upper_left(0);
              corners.set_lower_left(0);
              break;
            case DevToolsDockedPlacement::kRight:
              corners.set_upper_right(0);
              corners.set_lower_right(0);
              break;
            case DevToolsDockedPlacement::kBottom:
              corners.set_lower_left(0);
              corners.set_lower_right(0);
              break;
            case DevToolsDockedPlacement::kNone:
              break;
            case DevToolsDockedPlacement::kUnknown:
              break;
          }
        }

        if (contents && contents->holder()) {
          // Upstream uses layer for its background.
          CHECK(contents->layer());
          contents->layer()->SetRoundedCornerRadius(corners);
          contents->holder()->SetCornerRadii(corners);
        }
      };

  update_corner_radius(contents_web_view_, devtools_web_view_,
                       devtools_docked_placement(), corners);

  if (in_split_view_mode) {
    split_view_->UpdateCornerRadius(corners);
  }
}

void LuxxleBrowserView::Layout(PassKey) {
  LayoutSuperclass<BrowserView>(this);
  UpdateWebViewRoundedCorners();
}

void LuxxleBrowserView::StartTabCycling() {
  tab_cycling_event_handler_ = std::make_unique<TabCyclingEventHandler>(this);
}

void LuxxleBrowserView::StopTabCycling() {
  tab_cycling_event_handler_.reset();
  static_cast<LuxxleTabStripModel*>(browser()->tab_strip_model())
      ->StopMRUCycling();
}

void LuxxleBrowserView::SetSidePanelOperationByActiveTabChange(bool tab_change) {
  if (!sidebar_container_view_) {
    return;
  }

  sidebar_container_view_->set_operation_from_active_tab_change(tab_change);
}

BEGIN_METADATA(LuxxleBrowserView)
END_METADATA
