/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/toolbar/luxxle_toolbar_view.h"

#include <algorithm>
#include <memory>
#include <utility>

#include "base/functional/bind.h"
#include "luxxle/app/luxxle_command_ids.h"
#include "luxxle/browser/ai_chat/ai_chat_utils.h"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#include "luxxle/browser/ui/views/tabs/vertical_tab_utils.h"
#include "luxxle/browser/ui/views/toolbar/ai_chat_button.h"
#include "luxxle/browser/ui/views/toolbar/bookmark_button.h"
#include "luxxle/browser/ui/views/toolbar/side_panel_button.h"
#include "luxxle/browser/ui/views/toolbar/wallet_button.h"
#include "luxxle/components/ai_chat/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/constants/pref_names.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/defaults.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/ui_features.h"
#include "chrome/browser/ui/views/bookmarks/bookmark_bubble_view.h"
#include "chrome/browser/ui/views/toolbar/toolbar_view.h"
#include "components/bookmarks/common/bookmark_pref_names.h"
#include "components/prefs/pref_service.h"
#include "ui/base/hit_test.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/window_open_disposition_utils.h"
#include "ui/events/event.h"
#include "ui/views/window/hit_test_utils.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
#include "luxxle/browser/ui/views/toolbar/luxxle_vpn_button.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

#if BUILDFLAG(IS_LINUX)
#include "chrome/common/pref_names.h"
#endif

namespace {
constexpr int kLocationBarMaxWidth = 1080;

double GetLocationBarMarginHPercent(int toolbar_width) {
  double location_bar_margin_h_pc = 0.07;
  if (toolbar_width < 700) {
    location_bar_margin_h_pc = 0;
  } else if (toolbar_width < 850) {
    location_bar_margin_h_pc = 0.03;
  } else if (toolbar_width < 1000) {
    location_bar_margin_h_pc = 0.05;
  }
  return location_bar_margin_h_pc;
}

gfx::Insets CalcLocationBarMargin(int toolbar_width,
                                  int available_location_bar_width,
                                  int location_bar_min_width,
                                  int location_bar_x) {
  // Apply the target margin, adjusting for min and max width of LocationBar
  // Make sure any margin doesn't shrink the LocationBar beyond minimum width
  int location_bar_max_margin_h =
      (available_location_bar_width - location_bar_min_width) / 2;
  int location_bar_margin_h =
      std::min(static_cast<int>(toolbar_width *
                                GetLocationBarMarginHPercent(toolbar_width)),
               location_bar_max_margin_h);
  int location_bar_width =
      available_location_bar_width - (location_bar_margin_h * 2);
  // Allow the margin to expand so LocationBar is restrained to max width
  if (location_bar_width > kLocationBarMaxWidth) {
    location_bar_margin_h += (location_bar_width - kLocationBarMaxWidth) / 2;
    location_bar_width = kLocationBarMaxWidth;
  }

  // Center LocationBar as much as possible within Toolbar
  const int location_bar_toolbar_center_point =
      location_bar_x + location_bar_margin_h + (location_bar_width / 2);
  // Calculate offset - positive for move left and negative for move right
  int location_bar_center_offset =
      location_bar_toolbar_center_point - (toolbar_width / 2);
  // Can't shim more than we have space for, so restrict to margin size
  // or in the case of moving-right, 25% of the space since we want to avoid
  // touching browser actions where possible
  location_bar_center_offset =
      (location_bar_center_offset > 0)
          ? std::min(location_bar_margin_h, location_bar_center_offset)
          : std::max(static_cast<int>(-location_bar_margin_h * .25),
                     location_bar_center_offset);

  // // Apply offset to margin
  const int location_bar_margin_l =
      location_bar_margin_h - location_bar_center_offset;
  const int location_bar_margin_r =
      location_bar_margin_h + location_bar_center_offset;
  return gfx::Insets::TLBR(0, location_bar_margin_l, 0, location_bar_margin_r);
}

bool HasMultipleUserProfiles() {
  ProfileAttributesStorage* profile_storage =
      &g_browser_process->profile_manager()->GetProfileAttributesStorage();
  size_t profile_count = profile_storage->GetNumberOfProfiles();
  return (profile_count != 1);
}

bool IsAvatarButtonHideable(Profile* profile) {
  return !profile->IsIncognitoProfile() && !profile->IsGuestSession();
}

}  // namespace

LuxxleToolbarView::LuxxleToolbarView(Browser* browser, BrowserView* browser_view)
    : ToolbarView(browser, browser_view) {}

LuxxleToolbarView::~LuxxleToolbarView() = default;

void LuxxleToolbarView::Init() {
  ToolbarView::Init();

  // This will allow us to move this window by dragging toolbar.
  // See luxxle_non_client_hit_test_helper.h
  views::SetHitTestComponent(this, HTCAPTION);

  DCHECK(location_bar_);
  // Get ToolbarView's container_view as a parent of location_bar_ because
  // container_view's type in ToolbarView is internal to toolbar_view.cc.
  views::View* container_view = location_bar_->parent();
  DCHECK(container_view);

  views::SetHitTestComponent(container_view, HTCAPTION);

  // For non-normal mode, we don't have to do any more work.
  if (display_mode_ != DisplayMode::NORMAL) {
    luxxle_initialized_ = true;
    return;
  }

  Profile* profile = browser()->profile();

  // We don't use divider between extensions container and other toolbar
  // buttons. Upstream conditionally creates |toolbar_divider_|, they check
  // whether it's null or not. So safe to make remove here.
  if (toolbar_divider_) {
    container_view->RemoveChildView(toolbar_divider_.get());
    toolbar_divider_ = nullptr;
  }

  // Track changes in profile count
  if (IsAvatarButtonHideable(profile)) {
    profile_observer_.Observe(
        &g_browser_process->profile_manager()->GetProfileAttributesStorage());
  }
  // track changes in bookmarks enabled setting
  edit_bookmarks_enabled_.Init(
      bookmarks::prefs::kEditBookmarksEnabled, profile->GetPrefs(),
      base::BindRepeating(&LuxxleToolbarView::OnEditBookmarksEnabledChanged,
                          base::Unretained(this)));
  show_bookmarks_button_.Init(
      kShowBookmarksButton, browser_->profile()->GetPrefs(),
      base::BindRepeating(&LuxxleToolbarView::OnShowBookmarksButtonChanged,
                          base::Unretained(this)));

  show_wallet_button_.Init(
      kShowWalletIconOnToolbar, browser_->profile()->GetPrefs(),
      base::BindRepeating(&LuxxleToolbarView::UpdateWalletButtonVisibility,
                          base::Unretained(this)));

  if (browser_->profile()->IsIncognitoProfile() &&
      !browser_->profile()->IsTor()) {
    wallet_private_window_enabled_.Init(
        kLuxxleWalletPrivateWindowsEnabled, browser_->profile()->GetPrefs(),
        base::BindRepeating(&LuxxleToolbarView::UpdateWalletButtonVisibility,
                            base::Unretained(this)));
  }

  // track changes in wide locationbar setting
  location_bar_is_wide_.Init(
      kLocationBarIsWide, profile->GetPrefs(),
      base::BindRepeating(&LuxxleToolbarView::OnLocationBarIsWideChanged,
                          base::Unretained(this)));

  if (tabs::utils::SupportsVerticalTabs(browser_)) {
    show_vertical_tabs_.Init(
        luxxle_tabs::kVerticalTabsEnabled,
        profile->GetOriginalProfile()->GetPrefs(),
        base::BindRepeating(&LuxxleToolbarView::UpdateHorizontalPadding,
                            base::Unretained(this)));
    show_title_bar_on_vertical_tabs_.Init(
        luxxle_tabs::kVerticalTabsShowTitleOnWindow,
        profile->GetOriginalProfile()->GetPrefs(),
        base::BindRepeating(&LuxxleToolbarView::UpdateHorizontalPadding,
                            base::Unretained(this)));
#if BUILDFLAG(IS_LINUX)
    use_custom_chrome_frame_.Init(
        prefs::kUseCustomChromeFrame, profile->GetOriginalProfile()->GetPrefs(),
        base::BindRepeating(&LuxxleToolbarView::UpdateHorizontalPadding,
                            base::Unretained(this)));
#endif  // BUILDFLAG(IS_LINUX)
  }

  const auto callback = [](Browser* browser, int command,
                           const ui::Event& event) {
    chrome::ExecuteCommandWithDisposition(
        browser, command, ui::DispositionFromEventFlags(event.flags()));
  };

  bookmark_ = container_view->AddChildViewAt(
      std::make_unique<LuxxleBookmarkButton>(
          base::BindRepeating(callback, browser_, IDC_BOOKMARK_THIS_TAB)),
      *container_view->GetIndexOf(location_bar_));
  bookmark_->SetTriggerableEventFlags(ui::EF_LEFT_MOUSE_BUTTON |
                                      ui::EF_MIDDLE_MOUSE_BUTTON);
  bookmark_->UpdateImageAndText();

  side_panel_ = container_view->AddChildViewAt(
      std::make_unique<SidePanelButton>(browser()),
      *container_view->GetIndexOf(GetAppMenuButton()) - 1);

  wallet_ = container_view->AddChildViewAt(
      std::make_unique<WalletButton>(GetAppMenuButton(), profile),
      *container_view->GetIndexOf(GetAppMenuButton()) - 1);
  wallet_->SetTriggerableEventFlags(ui::EF_LEFT_MOUSE_BUTTON |
                                    ui::EF_MIDDLE_MOUSE_BUTTON);
  wallet_->UpdateImageAndText();

  UpdateWalletButtonVisibility();

  // Don't check policy status since we're going to
  // setup a watcher for policy pref.
  if (ai_chat::IsAllowedForContext(browser_->profile(), false)) {
    ai_chat_button_ = container_view->AddChildViewAt(
        std::make_unique<AIChatButton>(browser()),
        *container_view->GetIndexOf(GetAppMenuButton()) - 1);
    show_ai_chat_button_.Init(
        ai_chat::prefs::kLuxxleAIChatShowToolbarButton,
        browser_->profile()->GetPrefs(),
        base::BindRepeating(&LuxxleToolbarView::UpdateAIChatButtonVisibility,
                            base::Unretained(this)));
    hide_ai_chat_button_by_policy_.Init(
        ai_chat::prefs::kEnabledByPolicy, profile->GetPrefs(),
        base::BindRepeating(&LuxxleToolbarView::UpdateAIChatButtonVisibility,
                            base::Unretained(this)));
    UpdateAIChatButtonVisibility();
  }

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  if (luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(profile)) {
    luxxle_vpn_ = container_view->AddChildViewAt(
        std::make_unique<LuxxleVPNButton>(browser()),
        *container_view->GetIndexOf(GetAppMenuButton()) - 1);
    show_luxxle_vpn_button_.Init(
        luxxle_vpn::prefs::kLuxxleVPNShowButton, profile->GetPrefs(),
        base::BindRepeating(&LuxxleToolbarView::OnVPNButtonVisibilityChanged,
                            base::Unretained(this)));
    hide_luxxle_vpn_button_by_policy_.Init(
        luxxle_vpn::prefs::kManagedLuxxleVPNDisabled, profile->GetPrefs(),
        base::BindRepeating(&LuxxleToolbarView::OnVPNButtonVisibilityChanged,
                            base::Unretained(this)));
    luxxle_vpn_->SetVisible(IsLuxxleVPNButtonVisible());
  }
#endif

  // Make sure that avatar button should be located right before the app menu.
  if (auto* avatar = GetAvatarToolbarButton()) {
    container_view->ReorderChildView(
        avatar, *container_view->GetIndexOf(GetAppMenuButton()) - 1);
  }

  luxxle_initialized_ = true;
  UpdateHorizontalPadding();
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
bool LuxxleToolbarView::IsLuxxleVPNButtonVisible() const {
  return show_luxxle_vpn_button_.GetValue() &&
         !hide_luxxle_vpn_button_by_policy_.GetValue();
}
void LuxxleToolbarView::OnVPNButtonVisibilityChanged() {
  DCHECK(luxxle_vpn_);
  luxxle_vpn_->SetVisible(IsLuxxleVPNButtonVisible());
}
#endif

void LuxxleToolbarView::OnEditBookmarksEnabledChanged() {
  DCHECK_EQ(DisplayMode::NORMAL, display_mode_);
  Update(nullptr);
}

void LuxxleToolbarView::OnShowBookmarksButtonChanged() {
  if (!bookmark_) {
    return;
  }

  UpdateBookmarkVisibility();
}

void LuxxleToolbarView::OnLocationBarIsWideChanged() {
  DCHECK_EQ(DisplayMode::NORMAL, display_mode_);

  DeprecatedLayoutImmediately();
  SchedulePaint();
}

void LuxxleToolbarView::OnThemeChanged() {
  ToolbarView::OnThemeChanged();

  if (!luxxle_initialized_) {
    return;
  }

  if (display_mode_ == DisplayMode::NORMAL && bookmark_) {
    bookmark_->UpdateImageAndText();
  }
  if (display_mode_ == DisplayMode::NORMAL && wallet_) {
    wallet_->UpdateImageAndText();
  }
}

void LuxxleToolbarView::OnProfileAdded(const base::FilePath& profile_path) {
  Update(nullptr);
}

void LuxxleToolbarView::OnProfileWasRemoved(const base::FilePath& profile_path,
                                           const std::u16string& profile_name) {
  Update(nullptr);
}

void LuxxleToolbarView::LoadImages() {
  ToolbarView::LoadImages();
  if (bookmark_) {
    bookmark_->UpdateImageAndText();
  }
  if (wallet_) {
    wallet_->UpdateImageAndText();
  }
}

void LuxxleToolbarView::Update(content::WebContents* tab) {
  ToolbarView::Update(tab);

  // Decide whether to show the bookmark button
  UpdateBookmarkVisibility();

  // Remove avatar menu if only a single user profile exists.
  // Always show if private / tor / guest window, as an indicator.
  auto* avatar_button = GetAvatarToolbarButton();
  if (avatar_button) {
    auto* profile = browser_->profile();
    const bool should_show_profile =
        !IsAvatarButtonHideable(profile) || HasMultipleUserProfiles();
    avatar_button->SetVisible(should_show_profile);
  }
}

void LuxxleToolbarView::UpdateBookmarkVisibility() {
  if (!bookmark_) {
    return;
  }

  DCHECK_EQ(DisplayMode::NORMAL, display_mode_);
  bookmark_->SetVisible(browser_defaults::bookmarks_enabled &&
                        edit_bookmarks_enabled_.GetValue() &&
                        show_bookmarks_button_.GetValue());
}

void LuxxleToolbarView::UpdateHorizontalPadding() {
  if (!luxxle_initialized_) {
    return;
  }

  // Get ToolbarView's container_view as a parent of location_bar_ because
  // container_view's type in ToolbarView is internal to toolbar_view.cc.
  DCHECK(location_bar_ && location_bar_->parent());
  views::View* container_view = location_bar_->parent();

  if (!tabs::utils::ShouldShowVerticalTabs(browser()) ||
      tabs::utils::ShouldShowWindowTitleForVerticalTabs(browser())) {
    container_view->SetBorder(nullptr);
  } else {
    auto [leading, trailing] =
        tabs::utils::GetLeadingTrailingCaptionButtonWidth(
            browser_view_->frame());
    container_view->SetBorder(views::CreateEmptyBorder(
        gfx::Insets().set_left(leading).set_right(trailing)));
  }
}

void LuxxleToolbarView::ShowBookmarkBubble(const GURL& url,
                                          bool already_bookmarked) {
  // Show BookmarkBubble attached to Luxxle's bookmark button
  // or the location bar if there is no bookmark button
  // (i.e. in non-normal display mode).
  views::View* anchor_view = location_bar_;
  if (bookmark_ && bookmark_->GetVisible()) {
    anchor_view = bookmark_;
  }

  BookmarkBubbleView::ShowBubble(anchor_view, GetWebContents(), bookmark_,
                                 browser_, url, already_bookmarked);
}

void LuxxleToolbarView::ViewHierarchyChanged(
    const views::ViewHierarchyChangedDetails& details) {
  ToolbarView::ViewHierarchyChanged(details);

  // Upstream has two more children |background_view_left_| and
  // |background_view_right_| behind the container view.
  const int container_view_index = 2;

  if (details.is_add && children().size() > container_view_index &&
      details.parent == children()[container_view_index]) {
    // Mark children of the container view as client area so that they are not
    // perceived as caption area. See luxxle_non_client_hit_test_helper.h
    views::SetHitTestComponent(details.child, HTCLIENT);
  }
}

void LuxxleToolbarView::Layout(PassKey) {
  LayoutSuperclass<ToolbarView>(this);

  if (!luxxle_initialized_) {
    return;
  }

  // ToolbarView::Layout() handles below modes. So just return.
  if (display_mode_ == DisplayMode::CUSTOM_TAB ||
      display_mode_ == DisplayMode::LOCATION) {
    return;
  }

  if (!location_bar_is_wide_.GetValue()) {
    ResetLocationBarBounds();
    ResetBookmarkButtonBounds();
  }
}

void LuxxleToolbarView::ResetLocationBarBounds() {
  DCHECK_EQ(DisplayMode::NORMAL, display_mode_);

  // Calculate proper location bar's margin and set its bounds.
  const gfx::Insets margin = CalcLocationBarMargin(
      width(), location_bar_->width(), location_bar_->GetMinimumSize().width(),
      location_bar_->x());

  location_bar_->SetBounds(
      location_bar_->x() + margin.left(), location_bar_->y(),
      location_bar_->width() - margin.width(), location_bar_->height());
}

void LuxxleToolbarView::ResetBookmarkButtonBounds() {
  DCHECK_EQ(DisplayMode::NORMAL, display_mode_);

  int button_right_margin = GetLayoutConstant(TOOLBAR_STANDARD_SPACING);

  if (bookmark_ && bookmark_->GetVisible()) {
    const int bookmark_width = bookmark_->GetPreferredSize().width();
    const int bookmark_x =
        location_bar_->x() - bookmark_width - button_right_margin;
    bookmark_->SetX(bookmark_x);
  }
}

void LuxxleToolbarView::UpdateAIChatButtonVisibility() {
  bool should_show = ai_chat::IsAllowedForContext(browser()->profile()) &&
                     show_ai_chat_button_.GetValue();
  ai_chat_button_->SetVisible(should_show);
}

void LuxxleToolbarView::UpdateWalletButtonVisibility() {
  Profile* profile = browser()->profile();
  if (luxxle_wallet::IsNativeWalletEnabled() &&
      luxxle_wallet::IsAllowedForContext(profile)) {
    // Hide all if user wants to hide.
    if (!show_wallet_button_.GetValue()) {
      wallet_->SetVisible(false);
      return;
    }

    if (!profile->IsIncognitoProfile()) {
      wallet_->SetVisible(true);
      return;
    }

    wallet_->SetVisible(wallet_private_window_enabled_.GetValue());
    return;
  }

  wallet_->SetVisible(false);
}

BEGIN_METADATA(LuxxleToolbarView)
END_METADATA
