/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <memory>

#include "luxxle/browser/ui/views/frame/luxxle_browser_non_client_frame_view_mac.h"

#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/views/frame/luxxle_non_client_hit_test_helper.h"
#include "luxxle/browser/ui/views/frame/luxxle_window_frame_graphic.h"
#include "luxxle/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/view_ids.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/base/hit_test.h"
#include "ui/gfx/geometry/insets.h"
#include "ui/gfx/geometry/rect.h"
#include "ui/gfx/scoped_canvas.h"

LuxxleBrowserNonClientFrameViewMac::LuxxleBrowserNonClientFrameViewMac(
    BrowserFrame* frame, BrowserView* browser_view)
    : BrowserNonClientFrameViewMac(frame, browser_view) {
  auto* browser = browser_view->browser();
  frame_graphic_ =
      std::make_unique<LuxxleWindowFrameGraphic>(browser->profile());

  if (tabs::utils::SupportsVerticalTabs(browser)) {
    auto* prefs = browser->profile()->GetOriginalProfile()->GetPrefs();
    show_vertical_tabs_.Init(
        luxxle_tabs::kVerticalTabsEnabled, prefs,
        base::BindRepeating(
            &LuxxleBrowserNonClientFrameViewMac::UpdateWindowTitleAndControls,
            base::Unretained(this)));
    show_title_bar_on_vertical_tabs_.Init(
        luxxle_tabs::kVerticalTabsShowTitleOnWindow, prefs,
        base::BindRepeating(
            &LuxxleBrowserNonClientFrameViewMac::UpdateWindowTitleVisibility,
            base::Unretained(this)));
  }
}

LuxxleBrowserNonClientFrameViewMac::~LuxxleBrowserNonClientFrameViewMac() = default;

void LuxxleBrowserNonClientFrameViewMac::OnPaint(gfx::Canvas* canvas) {
  BrowserNonClientFrameViewMac::OnPaint(canvas);

  // Don't draw frame graphic over border outline.
  gfx::ScopedCanvas scoped_canvas(canvas);
  gfx::Rect bounds_to_frame_graphic(bounds());
  if (!IsFrameCondensed()) {
    // Native frame has 1px border outline.
    constexpr int kFrameBorderOutlineThickness = 1;
    bounds_to_frame_graphic.Inset(gfx::Insets::VH(
        kFrameBorderOutlineThickness, kFrameBorderOutlineThickness));
    canvas->ClipRect(bounds_to_frame_graphic);
  }
  frame_graphic_->Paint(canvas, bounds_to_frame_graphic);
}

int LuxxleBrowserNonClientFrameViewMac::GetTopInset(bool restored) const {
  if (tabs::utils::ShouldShowVerticalTabs(browser_view()->browser())) {
    if (ShouldShowWindowTitleForVerticalTabs()) {
      // Set minimum top inset to show caption buttons on frame.
      return 30;
    }

    // Bypassing BrowserNonClientFrameViewMac's implementation so that we don't
    // have any inset when hiding title bar.
    return 0;
  }

  if (!tabs::features::HorizontalTabsUpdateEnabled()) {
    return BrowserNonClientFrameViewMac::GetTopInset(restored);
  }

  return 0;
}

bool LuxxleBrowserNonClientFrameViewMac::ShouldShowWindowTitleForVerticalTabs()
    const {
  return tabs::utils::ShouldShowWindowTitleForVerticalTabs(
      browser_view()->browser());
}

void LuxxleBrowserNonClientFrameViewMac::UpdateWindowTitleVisibility() {
  if (!browser_view()->browser()->is_type_normal())
    return;

  frame()->SetWindowTitleVisibility(ShouldShowWindowTitleForVerticalTabs());
}

void LuxxleBrowserNonClientFrameViewMac::UpdateWindowTitleColor() {
  if (!browser_view()->browser()->is_type_normal()) {
    return;
  }

  frame()->UpdateWindowTitleColor(
      GetCaptionColor(BrowserFrameActiveState::kUseCurrent));
}

int LuxxleBrowserNonClientFrameViewMac::NonClientHitTest(
    const gfx::Point& point) {
  if (auto res = luxxle::NonClientHitTest(browser_view(), point);
      res != HTNOWHERE) {
    return res;
  }

  return BrowserNonClientFrameViewMac::NonClientHitTest(point);
}

void LuxxleBrowserNonClientFrameViewMac::OnThemeChanged() {
  BrowserNonClientFrameViewMac::OnThemeChanged();
  UpdateWindowTitleColor();
}

void LuxxleBrowserNonClientFrameViewMac::UpdateWindowTitleAndControls() {
  UpdateWindowTitleVisibility();

  // In case title visibility wasn't changed and only vertical tab strip enabled
  // state changed, we should reset controls positions manually.
  base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
      FROM_HERE, base::BindOnce(&views::Widget::ResetWindowControlsPosition,
                                frame()->GetWeakPtr()));
}

gfx::Size LuxxleBrowserNonClientFrameViewMac::GetMinimumSize() const {
  if (tabs::utils::ShouldShowVerticalTabs(browser_view()->browser())) {
    // In order to ignore tab strip height, skip BrowserNonClientFrameViewMac's
    // implementation.
    auto size = frame()->client_view()->GetMinimumSize();
    size.SetToMax(gfx::Size(0, (size.width() * 3) / 4));
    // Note that we can't set empty bounds on Mac.
    size.SetToMax({1, 1});
    return size;
  }

  return BrowserNonClientFrameViewMac::GetMinimumSize();
}
