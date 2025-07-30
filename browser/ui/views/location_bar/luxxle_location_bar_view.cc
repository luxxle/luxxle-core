/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/location_bar/luxxle_location_bar_view.h"

#include <optional>
#include <utility>

#include "base/containers/contains.h"
#include "base/feature_list.h"
#include "luxxle/app/vector_icons/vector_icons.h"
#include "luxxle/browser/themes/luxxle_theme_service.h"
#include "luxxle/browser/ui/color/luxxle_color_id.h"
#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/views/luxxle_actions/luxxle_actions_container.h"
#include "luxxle/browser/ui/views/luxxle_news/luxxle_news_action_icon_view.h"
#include "luxxle/browser/ui/views/location_bar/luxxle_search_conversion/promotion_button_controller.h"
#include "luxxle/browser/ui/views/location_bar/luxxle_search_conversion/promotion_button_view.h"
#include "luxxle/browser/ui/views/playlist/playlist_action_icon_view.h"
#include "luxxle/browser/ui/views/toolbar/luxxle_toolbar_view.h"
#include "luxxle/components/commander/common/buildflags/buildflags.h"
#include "luxxle/grit/luxxle_theme_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/layout_constants.h"
#include "chrome/browser/ui/omnibox/omnibox_theme.h"
#include "chrome/browser/ui/views/chrome_layout_provider.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "chrome/browser/ui/views/omnibox/omnibox_view_views.h"
#include "chrome/browser/ui/views/page_action/page_action_icon_controller.h"
#include "chrome/grit/branded_strings.h"
#include "components/grit/luxxle_components_strings.h"
#include "components/omnibox/browser/omnibox_edit_model.h"
#include "components/version_info/channel.h"
#include "content/public/browser/navigation_entry.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/compositor/layer.h"
#include "ui/gfx/geometry/size.h"
#include "ui/gfx/geometry/skia_conversions.h"
#include "ui/gfx/image/image_skia.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/views/animation/ink_drop.h"
#include "ui/views/controls/highlight_path_generator.h"
#include "ui/views/view_utils.h"

#if BUILDFLAG(ENABLE_TOR)
#include "luxxle/browser/ui/views/location_bar/onion_location_view.h"
#endif

#if BUILDFLAG(ENABLE_COMMANDER)
#include "luxxle/browser/ui/commander/commander_service_factory.h"
#include "luxxle/components/commander/browser/commander_frontend_delegate.h"
#include "luxxle/components/commander/common/features.h"
#endif

namespace {

class LuxxleLocationBarViewFocusRingHighlightPathGenerator
    : public views::HighlightPathGenerator {
 public:
  LuxxleLocationBarViewFocusRingHighlightPathGenerator() = default;
  LuxxleLocationBarViewFocusRingHighlightPathGenerator(
      const LuxxleLocationBarViewFocusRingHighlightPathGenerator&) = delete;
  LuxxleLocationBarViewFocusRingHighlightPathGenerator& operator=(
      const LuxxleLocationBarViewFocusRingHighlightPathGenerator&) = delete;

  // HighlightPathGenerator
  SkPath GetHighlightPath(const views::View* view) override {
    return static_cast<const LuxxleLocationBarView*>(view)
        ->GetFocusRingHighlightPath();
  }
};

std::optional<LuxxleColorIds> GetFocusRingColor(Profile* profile) {
  if (profile->IsGuestSession()) {
    // Don't update color.
    return std::nullopt;
  }

  return kColorLocationBarFocusRing;
}

}  // namespace

LuxxleLocationBarView::LuxxleLocationBarView(Browser* browser,
                                           Profile* profile,
                                           CommandUpdater* command_updater,
                                           Delegate* delegate,
                                           bool is_popup_mode)
    : LocationBarView(browser,
                      profile,
                      command_updater,
                      delegate,
                      is_popup_mode) {}

LuxxleLocationBarView::~LuxxleLocationBarView() = default;

void LuxxleLocationBarView::Init() {
  // base method calls Update and Layout
  LocationBarView::Init();
  // Change focus ring highlight path
  views::FocusRing* focus_ring = views::FocusRing::Get(this);
  if (focus_ring) {
    focus_ring->SetPathGenerator(
        std::make_unique<
            LuxxleLocationBarViewFocusRingHighlightPathGenerator>());
    if (const auto color_id = GetFocusRingColor(profile())) {
      focus_ring->SetColorId(color_id.value());
    }
  }

  if (!browser_->profile()->IsOffTheRecord()) {
    luxxle_news_action_icon_view_ =
        AddChildView(std::make_unique<LuxxleNewsActionIconView>(
            browser_->profile(), this, this));
    luxxle_news_action_icon_view_->SetVisible(false);
    views::InkDrop::Get(luxxle_news_action_icon_view_)
        ->SetVisibleOpacity(GetPageActionInkDropVisibleOpacity());
  }
#if BUILDFLAG(ENABLE_TOR)
  onion_location_view_ = AddChildView(
      std::make_unique<OnionLocationView>(browser_->profile(), this, this));
#endif

  if (PromotionButtonController::PromotionEnabled(profile()->GetPrefs())) {
    promotion_button_ = AddChildView(std::make_unique<PromotionButtonView>());
    promotion_controller_ = std::make_unique<PromotionButtonController>(
        promotion_button_, omnibox_view_, browser());
  }

  // luxxle action buttons
  luxxle_actions_ = AddChildView(
      std::make_unique<LuxxleActionsContainer>(browser_, profile()));
  luxxle_actions_->Init();
  // Call Update again to cause a Layout
  Update(nullptr);

  // Stop slide animation for all content settings views icon.
  for (ContentSettingImageView* content_setting_view : content_setting_views_) {
    content_setting_view->disable_animation();
  }
}

void LuxxleLocationBarView::ShowPlaylistBubble(
    playlist::PlaylistBubblesController::BubbleType type) {
  if (auto* playlist_action_icon_view = GetPlaylistActionIconView()) {
    playlist_action_icon_view->ShowPlaylistBubble(type);
  }
}

PlaylistActionIconView* LuxxleLocationBarView::GetPlaylistActionIconView() {
  auto* playlist_action_icon_view =
      page_action_icon_controller()->GetPlaylistActionIconView();
  if (!playlist_action_icon_view) {
    return nullptr;
  }

  return views::AsViewClass<PlaylistActionIconView>(playlist_action_icon_view);
}

void LuxxleLocationBarView::Update(content::WebContents* contents) {
  // base Init calls update before our Init is run, so our children
  // may not be initialized yet
  if (luxxle_actions_) {
    luxxle_actions_->Update();
  }

#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_) {
    onion_location_view_->Update();
  }
#endif

  if (luxxle_news_action_icon_view_) {
    luxxle_news_action_icon_view_->Update();
  }

  LocationBarView::Update(contents);
}

void LuxxleLocationBarView::OnOmniboxBlurred() {
#if BUILDFLAG(ENABLE_COMMANDER)
  if (base::FeatureList::IsEnabled(features::kLuxxleCommander)) {
    if (auto* commander_service =
            commander::CommanderServiceFactory::GetForBrowserContext(
                profile_)) {
      commander_service->Hide();
    }
  }
#endif
  LocationBarView::OnOmniboxBlurred();
}

void LuxxleLocationBarView::OnChanged() {
  auto hide_page_actions = ShouldHidePageActionIcons();
  if (luxxle_actions_) {
    luxxle_actions_->SetShouldHide(hide_page_actions);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_) {
    onion_location_view_->Update();
  }
#endif

  if (luxxle_news_action_icon_view_) {
    luxxle_news_action_icon_view_->Update();
  }

  if (promotion_controller_) {
    const bool show_button =
        promotion_controller_->ShouldShowSearchPromotionButton() &&
        !ShouldChipOverrideLocationIcon() && !ShouldShowKeywordBubble();
    promotion_controller_->Show(show_button);
  }

  // OnChanged calls Layout
  LocationBarView::OnChanged();
}

std::vector<views::View*> LuxxleLocationBarView::GetRightMostTrailingViews() {
  std::vector<views::View*> views;
  if (luxxle_news_action_icon_view_) {
    views.push_back(luxxle_news_action_icon_view_);
  }

  if (luxxle_actions_) {
    views.push_back(luxxle_actions_);
  }

  return views;
}

std::vector<views::View*> LuxxleLocationBarView::GetLeftMostTrailingViews() {
  std::vector<views::View*> views;
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_) {
    views.push_back(onion_location_view_);
  }
#endif
  return views;
}

views::View* LuxxleLocationBarView::GetSearchPromotionButton() const {
  return promotion_button_;
}

void LuxxleLocationBarView::RefreshBackground() {
  LocationBarView::RefreshBackground();

  if (shadow_) {
    const bool show_shadow =
        IsMouseHovered() && !omnibox_view_->model()->is_caret_visible();
    shadow_->SetVisible(show_shadow);
    return;
  }
}

gfx::Size LuxxleLocationBarView::CalculatePreferredSize(
    const views::SizeBounds& available_size) const {
  gfx::Size min_size = LocationBarView::CalculatePreferredSize(available_size);
  if (luxxle_actions_ && luxxle_actions_->GetVisible()) {
    const int luxxle_actions_min = luxxle_actions_->GetMinimumSize().width();
    const int extra_width =
        luxxle_actions_min + GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING);
    min_size.Enlarge(extra_width, 0);
  }
  if (luxxle_news_action_icon_view_ &&
      luxxle_news_action_icon_view_->GetVisible()) {
    const int extra_width =
        GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
        luxxle_news_action_icon_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#if BUILDFLAG(ENABLE_TOR)
  if (onion_location_view_ && onion_location_view_->GetVisible()) {
    const int extra_width = GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING) +
                            onion_location_view_->GetMinimumSize().width();
    min_size.Enlarge(extra_width, 0);
  }
#endif

  return min_size;
}

void LuxxleLocationBarView::OnThemeChanged() {
  LocationBarView::OnThemeChanged();

  if (!IsInitialized()) {
    return;
  }

  Update(nullptr);
  SetupShadow();
}

void LuxxleLocationBarView::AddedToWidget() {
  SetupShadow();
}

void LuxxleLocationBarView::ChildVisibilityChanged(views::View* child) {
  LocationBarView::ChildVisibilityChanged(child);
  // Normally, PageActionIcons are in a container which is always visible, only
  // the size changes when an icon is shown or hidden. The LocationBarView
  // does not listen to ChildVisibilityChanged events so we must make we Layout
  // and re-caculate trailing decorator positions when a child changes.
  if (base::Contains(GetLeftMostTrailingViews(), child) ||
      base::Contains(GetRightMostTrailingViews(), child)) {
    DeprecatedLayoutImmediately();
    SchedulePaint();
  }
}

void LuxxleLocationBarView::SetupShadow() {
  const auto* const color_provider = GetColorProvider();
  if (!color_provider) {
    return;
  }

  const int radius = GetBorderRadius();
  ViewShadow::ShadowParameters shadow{
      .offset_x = 0,
      .offset_y = 1,
      .blur_radius = radius,
      .shadow_color = color_provider->GetColor(kColorLocationBarHoveredShadow)};

  shadow_ = std::make_unique<ViewShadow>(this, radius, shadow);
}

int LuxxleLocationBarView::GetBorderRadius() const {
  return ChromeLayoutProvider::Get()->GetCornerRadiusMetric(
      views::Emphasis::kMaximum, size());
}

void LuxxleLocationBarView::FocusLocation(bool is_user_initiated) {
  if (base::FeatureList::IsEnabled(tabs::features::kLuxxleSharedPinnedTabs) &&
      browser_->profile()->GetPrefs()->GetBoolean(
          luxxle_tabs::kSharedPinnedTab)) {
    // When updating dummy contents, this could be called even when the widget
    // is inactive. We shouldn't focus the omnibox in that case.
    if (auto* widget = GetWidget(); !widget || !widget->IsActive()) {
      return;
    }
  }

  omnibox_view_->SetFocus(is_user_initiated);
}

SkPath LuxxleLocationBarView::GetFocusRingHighlightPath() const {
  const SkScalar radius = GetBorderRadius();
  return SkPath().addRoundRect(gfx::RectToSkRect(GetLocalBounds()), radius,
                               radius);
}

ContentSettingImageView*
LuxxleLocationBarView::GetContentSettingsImageViewForTesting(size_t idx) {
  DCHECK(idx < content_setting_views_.size());
  return content_setting_views_[idx];
}

BEGIN_METADATA(LuxxleLocationBarView)
END_METADATA
