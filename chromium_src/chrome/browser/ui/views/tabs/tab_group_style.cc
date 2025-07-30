/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/views/tabs/tab_group_style.h"

#include "luxxle/browser/ui/tabs/luxxle_tab_layout_constants.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/views/tabs/luxxle_tab_group_underline.h"
#include "luxxle/browser/ui/views/tabs/vertical_tab_utils.h"

#define TabGroupUnderline LuxxleTabGroupUnderline
#define TabGroupStyle TabGroupStyle_ChromiumImpl

#include "src/chrome/browser/ui/views/tabs/tab_group_style.cc"

#undef TabGroupStyle
#undef TabGroupUnderline

bool TabGroupStyle::TabGroupUnderlineShouldBeHidden() const {
  return false;
}

// Upstream currently hides the tab group underline in certain scenarios,
// whereas we always show the underline.
bool TabGroupStyle::TabGroupUnderlineShouldBeHidden(
    const views::View* leading_view,
    const views::View* trailing_view) const {
  return false;
}

SkPath TabGroupStyle::GetUnderlinePath(gfx::Rect local_bounds) const {
  if (!ShouldShowVerticalTabs()) {
    return TabGroupStyle_ChromiumImpl::GetUnderlinePath(local_bounds);
  }

  // In vertical tabs, underline is not actually "underline'. It's vertical line
  // at the left side of the tab group. And it has half rounded corners.
  //
  // +   group header   | '+' is the underline.
  // ++  tab 1          | Drawing starts from top-right and goes
  // ++  tab 2          | counter-clockwise
  // +   tab 3          |
  //
  SkPath path;
  path.arcTo(/* rx = */ kStrokeThicknessForVerticalTabs,
             /* ry = */ kStrokeThicknessForVerticalTabs,
             /* angle = */ 180.f, SkPath::kSmall_ArcSize, SkPathDirection::kCW,
             /* x = */ kStrokeThicknessForVerticalTabs,
             /* y = */ kStrokeThicknessForVerticalTabs);
  path.lineTo(kStrokeThicknessForVerticalTabs,
              local_bounds.height() - kStrokeThicknessForVerticalTabs);
  path.arcTo(/* rx = */ kStrokeThicknessForVerticalTabs,
             /* ry = */ kStrokeThicknessForVerticalTabs,
             /* angle = */ 180.f, SkPath::kSmall_ArcSize, SkPathDirection::kCW,
             /* x = */ 0,
             /* y = */ local_bounds.height());
  path.close();

  return path;
}

gfx::Insets TabGroupStyle::GetInsetsForHeaderChip() const {
  auto insets = TabGroupStyle_ChromiumImpl::GetInsetsForHeaderChip();
  if (!tabs::features::HorizontalTabsUpdateEnabled()) {
    return insets;
  }
  if (!ShouldShowVerticalTabs()) {
    return gfx::Insets::VH(luxxle_tabs::GetTabGroupTitleVerticalInset(),
                           luxxle_tabs::GetTabGroupTitleHorizontalInset());
  }
  return insets;
}

gfx::Point TabGroupStyle::GetTitleChipOffset(
    std::optional<int> text_height) const {
  if (!tabs::features::HorizontalTabsUpdateEnabled()) {
    return TabGroupStyle_ChromiumImpl::GetTitleChipOffset(text_height);
  }
  return gfx::Point(luxxle_tabs::kHorizontalTabInset,
                    luxxle_tabs::kHorizontalTabVerticalSpacing);
}

bool TabGroupStyle::ShouldShowVerticalTabs() const {
  return tabs::utils::ShouldShowVerticalTabs(tab_group_views_->GetBrowser());
}

float TabGroupStyle::GetEmptyChipSize() const {
  if (!tabs::features::HorizontalTabsUpdateEnabled()) {
    return TabGroupStyle_ChromiumImpl::GetEmptyChipSize();
  }
  return luxxle_tabs::GetHorizontalTabHeight();
}

int TabGroupStyle::GetChipCornerRadius() const {
  if (!tabs::features::HorizontalTabsUpdateEnabled()) {
    return TabGroupStyle_ChromiumImpl::GetChipCornerRadius();
  }
  return luxxle_tabs::kTabBorderRadius;
}
