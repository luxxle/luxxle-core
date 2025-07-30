/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_POPUP_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_POPUP_H_

#include <cstdint>
#include <memory>
#include <string>

#include "base/memory/raw_ptr.h"
#include "base/scoped_observation.h"
#include "luxxle/browser/ui/luxxle_tooltips/luxxle_tooltip.h"
#include "luxxle/browser/ui/views/luxxle_tooltips/luxxle_tooltip_view.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/display/display_observer.h"
#include "ui/gfx/shadow_util.h"
#include "ui/gfx/shadow_value.h"
#include "ui/views/controls/button/button.h"
#include "ui/views/widget/widget_delegate.h"
#include "ui/views/widget/widget_observer.h"

namespace gfx {
class Point;
class Rect;
class Size;
}  // namespace gfx

namespace views {
class Widget;
}  // namespace views

namespace luxxle_tooltips {

class LuxxleTooltipView;

// Provides a generalized interface for displaying user-actionable tooltips on
// the desktop window.
//
// Usage example:
//   auto tooltip = std::make_unique<luxxle_tooltips::LuxxleTooltip>(
//       "id", luxxle_tooltips::LuxxleTooltipAttributes(u"Title", u"Body", u"OK"),
//       this);
//   auto popup = std::make_unique<luxxle_tooltips::LuxxleTooltipPopup>(
//       std::move(tooltip));
//   popup->Show();
//   ...
//   popup->Close();
//
// This creates and show a tooltip with the given attributes/controls. Button
// presseses will forward to the delegate (set via the LuxxleTooltip
// constructor). Finally, the tooltip is closed.
class LuxxleTooltipPopup : public views::WidgetDelegateView,
                          public views::WidgetObserver,
                          public display::DisplayObserver {
  METADATA_HEADER(LuxxleTooltipPopup, views::WidgetDelegateView)
 public:
  explicit LuxxleTooltipPopup(std::unique_ptr<LuxxleTooltip> tooltip);
  ~LuxxleTooltipPopup() override;

  LuxxleTooltipPopup(const LuxxleTooltipPopup&) = delete;
  LuxxleTooltipPopup& operator=(const LuxxleTooltipPopup&) = delete;

  // Show the tooltip popup view
  void Show();

  // Close the tooltip popup view
  void Close();

  // Close the widget
  void CloseWidget();

  // User pressed the Ok button
  void OnOkButtonPressed();

  // User pressed the Cancel button
  void OnCancelButtonPressed();

  gfx::Rect CalculateBounds(bool use_default_origin);

  void set_normalized_display_coordinates(double x, double y);

  void set_display_work_area_insets(int x, int y);

  views::Button* ok_button_for_testing() const {
    return tooltip_view_ ? tooltip_view_->ok_button_for_testing() : nullptr;
  }

  views::Button* cancel_button_for_testing() const {
    return tooltip_view_ ? tooltip_view_->cancel_button_for_testing() : nullptr;
  }

  // display::DisplayObserver:
  void OnDisplaysRemoved(const display::Displays& old_displays) override;
  void OnDisplayMetricsChanged(const display::Display& display,
                               uint32_t changed_metrics) override;

  // views::WidgetDelegateView:
  void OnDisplayChanged() override;
  void OnWorkAreaChanged() override;
  void OnPaintBackground(gfx::Canvas* canvas) override;
  void OnThemeChanged() override;

  // views::WidgetObserver:
  void OnWidgetCreated(views::Widget* widget) override;
  void OnWidgetDestroyed(views::Widget* widget) override;
  void OnWidgetBoundsChanged(views::Widget* widget,
                             const gfx::Rect& new_bounds) override;

 private:
  void CreatePopup();

  gfx::Point GetDefaultOriginForSize(const gfx::Size& size);

  void RecomputeAlignment();

  const gfx::ShadowDetails& GetShadowDetails() const;
  gfx::Insets GetShadowMargin() const;

  void CreateWidgetView();
  void CloseWidgetView();

  bool IsWidgetValid() const;

  std::unique_ptr<views::Widget> widget_;
  std::unique_ptr<LuxxleTooltip> tooltip_;

  raw_ptr<LuxxleTooltipView> tooltip_view_ = nullptr;

  gfx::Point widget_origin_ = {0, 0};

  double normalized_display_coordinate_x_ = 1.0;
  double normalized_display_coordinate_y_ = 0.0;

  int display_work_area_inset_x_ = -13;
  int display_work_area_inset_y_ = 18;

  base::ScopedObservation<views::Widget, views::WidgetObserver>
      widget_observation_{this};
};

}  // namespace luxxle_tooltips

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_POPUP_H_
