/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/tabs/tab_style.h"

#include "luxxle/browser/ui/tabs/luxxle_tab_layout_constants.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "chrome/browser/ui/layout_constants.h"

namespace {

// A subclass of TabStyle used to customize tab layout and visuals to support
// Luxxle specifics including horizontal tabs.
class LuxxleTabStyle : public TabStyle {
 public:
  int GetTabOverlap() const override {
    if (!tabs::features::HorizontalTabsUpdateEnabled()) {
      return TabStyle::GetTabOverlap();
    }
    return luxxle_tabs::kHorizontalTabOverlap;
  }

  int GetTopCornerRadius() const override {
    if (!tabs::features::HorizontalTabsUpdateEnabled()) {
      return TabStyle::GetTopCornerRadius();
    }
    return luxxle_tabs::kTabBorderRadius;
  }

  int GetBottomCornerRadius() const override {
    if (!tabs::features::HorizontalTabsUpdateEnabled()) {
      return TabStyle::GetBottomCornerRadius();
    }
    return luxxle_tabs::kTabBorderRadius;
  }

  gfx::Insets GetContentsInsets() const override {
    if (!tabs::features::HorizontalTabsUpdateEnabled()) {
      return TabStyle::GetContentsInsets();
    }
    return gfx::Insets::VH(0, luxxle_tabs::GetHorizontalTabPadding() +
                                  luxxle_tabs::kHorizontalTabInset);
  }

  int GetPinnedWidth() const override {
    if (!tabs::features::HorizontalTabsUpdateEnabled()) {
      return TabStyle::GetPinnedWidth();
    }
    return luxxle_tabs::GetHorizontalTabHeight() +
           luxxle_tabs::kHorizontalTabInset * 2;
  }

  int GetDragHandleExtension(int height) const override {
    if (!tabs::features::HorizontalTabsUpdateEnabled()) {
      return TabStyle::GetDragHandleExtension(height);
    }
    // The "drag handle extension" is the amount of space in DIP at the top of
    // inactive tabs where mouse clicks are treated as clicks in the "caption"
    // area, i.e. the draggable part of the window frame.
    return 4;
  }

  gfx::Size GetSeparatorSize() const override {
    return gfx::Size(1, GetLayoutConstant(TAB_SEPARATOR_HEIGHT));
  }

  gfx::Insets GetSeparatorMargins() const override {
    return gfx::Insets::TLBR(0, GetSeparatorSize().width() * -1, 0,
                             GetSeparatorSize().width() * -1);
  }

  int GetSeparatorCornerRadius() const override { return 0; }
};

}  // namespace

#define LUXXLE_TAB_STYLE_GET return new LuxxleTabStyle();
#include "src/chrome/browser/ui/tabs/tab_style.cc"
#undef LUXXLE_TAB_STYLE_GET
