/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ACTIONS_LUXXLE_ACTIONS_CONTAINER_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ACTIONS_LUXXLE_ACTIONS_CONTAINER_H_

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/browser.h"
#include "components/prefs/pref_member.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/gfx/skia_util.h"
#include "ui/views/view.h"

class LuxxleActionViewController;
class LuxxleActionsContainerTest;
class LuxxleRewardsActionView;
class LuxxleShieldsActionView;
class RewardsBrowserTest;

namespace policy {
FORWARD_DECLARE_TEST(LuxxleRewardsPolicyTest, RewardsIconIsHidden);
}

namespace views {
class Button;
}

// This View contains all the built-in LuxxleActions such as Shields and Payments
// TODO(petemill): consider splitting to separate model, like
// ToolbarActionsModel and ToolbarActionsBar
class LuxxleActionsContainer : public views::View {
  METADATA_HEADER(LuxxleActionsContainer, views::View)

 public:
  LuxxleActionsContainer(BrowserWindowInterface* browser_window_interface,
                        Profile* profile);
  LuxxleActionsContainer(const LuxxleActionsContainer&) = delete;

  LuxxleActionsContainer& operator=(const LuxxleActionsContainer&) = delete;

  ~LuxxleActionsContainer() override;

  void Init();
  void Update();
  void SetShouldHide(bool should_hide);

  // views::View:
  void ChildPreferredSizeChanged(views::View* child) override;

  LuxxleShieldsActionView* GetShieldsActionView() { return shields_action_btn_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(policy::LuxxleRewardsPolicyTest, RewardsIconIsHidden);
  friend class ::LuxxleActionsContainerTest;
  friend class ::RewardsBrowserTest;

  bool ShouldShowLuxxleRewardsAction() const;
  void AddActionViewForRewards();
  void AddActionViewForShields();

  void UpdateVisibility();
  gfx::Size GetActionSize() const;

  // Luxxle Rewards preferences change observers callback.
  void OnLuxxleRewardsPreferencesChanged();

  bool should_hide_ = false;

  // The Browser this LocationBarView is in.  Note that at least
  // chromeos::SimpleWebViewDialog uses a LocationBarView outside any browser
  // window, so this may be NULL.
  raw_ptr<BrowserWindowInterface> browser_window_interface_ = nullptr;

  raw_ptr<LuxxleShieldsActionView> shields_action_btn_ = nullptr;
  raw_ptr<LuxxleRewardsActionView> rewards_action_btn_ = nullptr;

  // Listen for Luxxle Rewards preferences changes.
  BooleanPrefMember show_luxxle_rewards_button_;

  base::WeakPtrFactory<LuxxleActionsContainer> weak_ptr_factory_{this};
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ACTIONS_LUXXLE_ACTIONS_CONTAINER_H_
