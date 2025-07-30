/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/luxxle_actions/luxxle_actions_container.h"

#include <memory>
#include <utility>

#include "base/feature_list.h"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/browser/ui/views/luxxle_actions/luxxle_rewards_action_view.h"
#include "luxxle/browser/ui/views/luxxle_actions/luxxle_shields_action_view.h"
#include "luxxle/browser/ui/views/rounded_separator.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/layout_constants.h"
#include "components/prefs/pref_service.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/views/layout/box_layout.h"
#include "ui/views/view.h"

LuxxleActionsContainer::LuxxleActionsContainer(
    BrowserWindowInterface* browser_window_interface,
    Profile* profile)
    : browser_window_interface_(browser_window_interface) {}

LuxxleActionsContainer::~LuxxleActionsContainer() = default;

void LuxxleActionsContainer::Init() {
  // automatic layout
  auto vertical_container_layout = std::make_unique<views::BoxLayout>(
      views::BoxLayout::Orientation::kHorizontal);
  vertical_container_layout->set_main_axis_alignment(
      views::BoxLayout::MainAxisAlignment::kCenter);
  vertical_container_layout->set_cross_axis_alignment(
      views::BoxLayout::CrossAxisAlignment::kCenter);
  SetLayoutManager(std::move(vertical_container_layout));

  // children
  RoundedSeparator* luxxle_button_separator_ = new RoundedSeparator();
  // TODO(petemill): theme color
  luxxle_button_separator_->SetColor(SkColorSetRGB(0xb2, 0xb5, 0xb7));
  constexpr int kSeparatorMargin = 3;
  constexpr int kSeparatorWidth = 1;
  luxxle_button_separator_->SetPreferredSize(
      gfx::Size(kSeparatorWidth + kSeparatorMargin * 2,
                GetLayoutConstant(LOCATION_BAR_ICON_SIZE)));
  // separator left & right margin
  luxxle_button_separator_->SetBorder(views::CreateEmptyBorder(
      gfx::Insets::TLBR(0, kSeparatorMargin, 0, kSeparatorMargin)));
  // Just in case the extensions load before this function does (not likely!)
  // make sure separator is at index 0
  AddChildViewAt(luxxle_button_separator_, 0);
  AddActionViewForShields();
  AddActionViewForRewards();

  // React to Luxxle Rewards preferences changes.
  show_luxxle_rewards_button_.Init(
      luxxle_rewards::prefs::kShowLocationBarButton,
      browser_window_interface_->GetProfile()->GetPrefs(),
      base::BindRepeating(
          &LuxxleActionsContainer::OnLuxxleRewardsPreferencesChanged,
          base::Unretained(this)));
}

bool LuxxleActionsContainer::ShouldShowLuxxleRewardsAction() const {
  if (!luxxle_rewards::IsSupportedForProfile(
          browser_window_interface_->GetProfile())) {
    return false;
  }
  const PrefService* prefs =
      browser_window_interface_->GetProfile()->GetPrefs();
  return prefs->GetBoolean(luxxle_rewards::prefs::kShowLocationBarButton);
}

void LuxxleActionsContainer::AddActionViewForShields() {
  shields_action_btn_ = AddChildViewAt(
      std::make_unique<LuxxleShieldsActionView>(browser_window_interface_), 1);
  shields_action_btn_->SetPreferredSize(GetActionSize());
  shields_action_btn_->Init();
}

void LuxxleActionsContainer::AddActionViewForRewards() {
  auto button =
      std::make_unique<LuxxleRewardsActionView>(browser_window_interface_);
  rewards_action_btn_ = AddChildViewAt(std::move(button), 2);
  rewards_action_btn_->SetPreferredSize(GetActionSize());
  rewards_action_btn_->SetVisible(ShouldShowLuxxleRewardsAction());
  rewards_action_btn_->Update();
}

void LuxxleActionsContainer::Update() {
  if (shields_action_btn_) {
    shields_action_btn_->Update();
  }

  if (rewards_action_btn_) {
    rewards_action_btn_->Update();
  }

  UpdateVisibility();
  DeprecatedLayoutImmediately();
}

void LuxxleActionsContainer::UpdateVisibility() {
  bool can_show = false;

  if (shields_action_btn_) {
    can_show = shields_action_btn_->GetVisible();
  }

  if (rewards_action_btn_) {
    can_show = can_show || rewards_action_btn_->GetVisible();
  }

  // If no buttons are visible, then we want to hide this view so that the
  // separator is not displayed.
  SetVisible(!should_hide_ && can_show);
}

gfx::Size LuxxleActionsContainer::GetActionSize() const {
  return {34, GetLayoutConstant(LOCATION_BAR_HEIGHT) -
                  2 * GetLayoutConstant(LOCATION_BAR_ELEMENT_PADDING)};
}

void LuxxleActionsContainer::SetShouldHide(bool should_hide) {
  should_hide_ = should_hide;
  Update();
}

void LuxxleActionsContainer::ChildPreferredSizeChanged(views::View* child) {
  PreferredSizeChanged();
}

// Luxxle Rewards preferences change observers callback
void LuxxleActionsContainer::OnLuxxleRewardsPreferencesChanged() {
  if (rewards_action_btn_) {
    rewards_action_btn_->SetVisible(ShouldShowLuxxleRewardsAction());
  }
}

BEGIN_METADATA(LuxxleActionsContainer)
END_METADATA
