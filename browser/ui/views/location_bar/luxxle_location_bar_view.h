/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LOCATION_BAR_LUXXLE_LOCATION_BAR_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LOCATION_BAR_LUXXLE_LOCATION_BAR_VIEW_H_

#include <memory>
#include <vector>

#include "base/gtest_prod_util.h"
#include "luxxle/browser/ui/views/luxxle_news/luxxle_news_action_icon_view.h"
#include "luxxle/browser/ui/views/playlist/playlist_bubbles_controller.h"
#include "luxxle/browser/ui/views/view_shadow.h"
#include "luxxle/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

class LuxxleActionsContainer;
class LuxxleActionsContainerTest;
class PromotionButtonController;
class PromotionButtonView;
class PlaylistActionIconView;
class RewardsBrowserTest;
class SkPath;

#if BUILDFLAG(ENABLE_TOR)
class OnionLocationView;
#endif

namespace playlist {
FORWARD_DECLARE_TEST(PlaylistBrowserTest, AddItemsToList);
FORWARD_DECLARE_TEST(PlaylistBrowserTestWithSitesUsingMediaSource,
                     MediaShouldBeExtractedFromBackground_SucceedInExtracting);
FORWARD_DECLARE_TEST(PlaylistBrowserTestWithSitesUsingMediaSource,
                     MediaShouldBeExtractedFromBackground_FailToExtract);
FORWARD_DECLARE_TEST(
    PlaylistBrowserTestWithSitesUsingMediaSource,
    MediaShouldBeExtractedFromBackground_DynamicallyAddedMedia);
}  // namespace playlist

namespace policy {
FORWARD_DECLARE_TEST(LuxxleRewardsPolicyTest, RewardsIconIsHidden);
}

// The purposes of this subclass are to:
// - Add the LuxxleActionsContainer to the location bar
class LuxxleLocationBarView : public LocationBarView {
  METADATA_HEADER(LuxxleLocationBarView, LocationBarView)
 public:
  LuxxleLocationBarView(Browser* browser,
                       Profile* profile,
                       CommandUpdater* command_updater,
                       Delegate* delegate,
                       bool is_popup_mode);
  ~LuxxleLocationBarView() override;

  LuxxleLocationBarView(const LuxxleLocationBarView&) = delete;
  LuxxleLocationBarView& operator=(const LuxxleLocationBarView&) = delete;

  void Init() override;
  void Update(content::WebContents* contents) override;
  void OnChanged() override;
  LuxxleActionsContainer* GetLuxxleActionsContainer() { return luxxle_actions_; }
#if BUILDFLAG(ENABLE_TOR)
  OnionLocationView* GetOnionLocationView() { return onion_location_view_; }
#endif

  // LocationBarView:
  // Views that locates at right side of upstream's trailing views.
  std::vector<views::View*> GetRightMostTrailingViews() override;
  // Views that locates at left side of upstream's trailing views.
  std::vector<views::View*> GetLeftMostTrailingViews() override;
  views::View* GetSearchPromotionButton() const override;
  void RefreshBackground() override;
  void OnOmniboxBlurred() override;

  // views::View:
  gfx::Size CalculatePreferredSize(
      const views::SizeBounds& available_size) const override;
  void OnThemeChanged() override;
  void ChildVisibilityChanged(views::View* child) override;
  void AddedToWidget() override;
  int GetBorderRadius() const override;
  void FocusLocation(bool is_user_initiated) override;

  SkPath GetFocusRingHighlightPath() const;
  ContentSettingImageView* GetContentSettingsImageViewForTesting(size_t idx);
  LuxxleActionsContainer* luxxle_actions_contatiner_view() {
    return luxxle_actions_;
  }

  void ShowPlaylistBubble(
      playlist::PlaylistBubblesController::BubbleType type =
          playlist::PlaylistBubblesController::BubbleType::kInfer);

 private:
  FRIEND_TEST_ALL_PREFIXES(playlist::PlaylistBrowserTest, AddItemsToList);
  FRIEND_TEST_ALL_PREFIXES(
      playlist::PlaylistBrowserTestWithSitesUsingMediaSource,
      MediaShouldBeExtractedFromBackground_SucceedInExtracting);
  FRIEND_TEST_ALL_PREFIXES(
      playlist::PlaylistBrowserTestWithSitesUsingMediaSource,
      MediaShouldBeExtractedFromBackground_FailToExtract);
  FRIEND_TEST_ALL_PREFIXES(
      playlist::PlaylistBrowserTestWithSitesUsingMediaSource,
      MediaShouldBeExtractedFromBackground_DynamicallyAddedMedia);
  FRIEND_TEST_ALL_PREFIXES(policy::LuxxleRewardsPolicyTest, RewardsIconIsHidden);
  FRIEND_TEST_ALL_PREFIXES(LuxxleLocationBarViewBrowserTest,
                           SearchConversionButtonTest);
  friend class ::LuxxleActionsContainerTest;
  friend class ::RewardsBrowserTest;

  PlaylistActionIconView* GetPlaylistActionIconView();
  void SetupShadow();

  std::unique_ptr<ViewShadow> shadow_;
  raw_ptr<LuxxleActionsContainer> luxxle_actions_ = nullptr;
  raw_ptr<LuxxleNewsActionIconView> luxxle_news_action_icon_view_ = nullptr;
  std::unique_ptr<PromotionButtonController> promotion_controller_;
  raw_ptr<PromotionButtonView> promotion_button_ = nullptr;
#if BUILDFLAG(ENABLE_TOR)
  raw_ptr<OnionLocationView> onion_location_view_ = nullptr;
#endif
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LOCATION_BAR_LUXXLE_LOCATION_BAR_VIEW_H_
