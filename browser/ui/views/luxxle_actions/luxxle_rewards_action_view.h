// Copyright (c) 2022 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ACTIONS_LUXXLE_REWARDS_ACTION_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ACTIONS_LUXXLE_REWARDS_ACTION_VIEW_H_

#include <memory>
#include <string>
#include <utility>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/browser/ui/luxxle_rewards/rewards_panel_coordinator.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "chrome/browser/ui/views/toolbar/toolbar_button.h"
#include "components/prefs/pref_change_registrar.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/gfx/geometry/skia_conversions.h"
#include "ui/views/widget/widget_observer.h"

class Profile;
class TabStripModel;
class WebUIBubbleManager;

// A button that lives in the actions container and opens the Rewards panel. The
// button has an associated context menu and can be hidden by user settings.
class LuxxleRewardsActionView
    : public ToolbarButton,
      public views::WidgetObserver,
      public TabStripModelObserver,
      public luxxle_rewards::RewardsTabHelper::Observer,
      public luxxle_rewards::RewardsPanelCoordinator::Observer,
      public luxxle_rewards::RewardsServiceObserver,
      public luxxle_rewards::RewardsNotificationServiceObserver {
  METADATA_HEADER(LuxxleRewardsActionView, ToolbarButton)
 public:
  explicit LuxxleRewardsActionView(
      BrowserWindowInterface* browser_window_interface);

  ~LuxxleRewardsActionView() override;

  LuxxleRewardsActionView(const LuxxleRewardsActionView&) = delete;
  LuxxleRewardsActionView& operator=(const LuxxleRewardsActionView&) = delete;

  void Update();

  void ClosePanelForTesting();

  // views::View:
  gfx::Rect GetAnchorBoundsInScreen() const override;

  // views::LabelButton:
  std::unique_ptr<views::LabelButtonBorder> CreateDefaultBorder()
      const override;
  void OnThemeChanged() override;

  // views::WidgetObserver:
  void OnWidgetDestroying(views::Widget* widget) override;

  // TabStripModelObserver:
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;

  // luxxle_rewards::RewardsTabHelper::Observer:
  void OnPublisherForTabUpdated(const std::string& publisher_id) override;

  // luxxle_rewards::RewardsPanelCoordinator::Observer:
  void OnRewardsPanelRequested() override;

  // luxxle_rewards::RewardsServiceObserver:
  void OnPublisherRegistryUpdated() override;

  void OnPublisherUpdated(const std::string& publisher_id) override;

  // luxxle_rewards::RewardsNotificationServiceObserver:
  void OnNotificationAdded(
      luxxle_rewards::RewardsNotificationService* service,
      const luxxle_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;

  void OnNotificationDeleted(
      luxxle_rewards::RewardsNotificationService* service,
      const luxxle_rewards::RewardsNotificationService::RewardsNotification&
          notification) override;

 private:
  void OnButtonPressed();
  void OnPreferencesChanged(const std::string& key);
  content::WebContents* GetActiveWebContents();
  luxxle_rewards::RewardsService* GetRewardsService();
  luxxle_rewards::RewardsNotificationService* GetNotificationService();
  bool IsPanelOpen();
  void ToggleRewardsPanel();
  gfx::ImageSkia GetRewardsIcon();
  std::pair<std::string, SkColor> GetBadgeTextAndBackground();
  size_t GetRewardsNotificationCount();
  bool UpdatePublisherStatus();
  void IsPublisherRegisteredCallback(const std::string& publisher_id,
                                     bool is_registered);
  void UpdateTabHelper(content::WebContents* web_contents);

  using WidgetObservation =
      base::ScopedObservation<views::Widget, views::WidgetObserver>;

  using RewardsObservation =
      base::ScopedObservation<luxxle_rewards::RewardsService,
                              luxxle_rewards::RewardsServiceObserver>;

  using NotificationServiceObservation = base::ScopedObservation<
      luxxle_rewards::RewardsNotificationService,
      luxxle_rewards::RewardsNotificationServiceObserver>;

  raw_ptr<BrowserWindowInterface> browser_window_interface_ = nullptr;
  raw_ptr<luxxle_rewards::RewardsPanelCoordinator> panel_coordinator_ = nullptr;
  raw_ptr<luxxle_rewards::RewardsTabHelper> tab_helper_ = nullptr;
  std::unique_ptr<WebUIBubbleManager> bubble_manager_;
  PrefChangeRegistrar pref_change_registrar_;
  std::pair<std::string, bool> publisher_registered_;
  luxxle_rewards::RewardsTabHelper::Observation tab_helper_observation_{this};
  luxxle_rewards::RewardsPanelCoordinator::Observation panel_observation_{this};
  WidgetObservation bubble_observation_{this};
  RewardsObservation rewards_service_observation_{this};
  NotificationServiceObservation notification_service_observation_{this};
  base::WeakPtrFactory<LuxxleRewardsActionView> weak_factory_{this};
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ACTIONS_LUXXLE_REWARDS_ACTION_VIEW_H_
