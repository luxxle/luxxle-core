/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_VIEW_H_

#include "base/memory/raw_ptr.h"
#include "luxxle/browser/ui/luxxle_tooltips/luxxle_tooltip.h"
#include "luxxle/browser/ui/views/luxxle_tooltips/luxxle_tooltip_label_button.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/views/view.h"

namespace gfx {
class Canvas;
class Point;
}  // namespace gfx

namespace views {
class ImageView;
class Label;
class LabelButton;
class View;
}  // namespace views

namespace luxxle_tooltips {

class LuxxleTooltipPopup;

class LuxxleTooltipView : public views::View {
  METADATA_HEADER(LuxxleTooltipView, views::View)
 public:

  LuxxleTooltipView(LuxxleTooltipPopup* tooltip_popup,
                   const LuxxleTooltipAttributes& tooltip_attributes);
  ~LuxxleTooltipView() override;

  LuxxleTooltipView(const LuxxleTooltipView&) = delete;
  LuxxleTooltipView& operator=(const LuxxleTooltipView&) = delete;

  views::Button* ok_button_for_testing() const { return ok_button_; }
  views::Button* cancel_button_for_testing() const { return cancel_button_; }

  // views::InkDropHostView:
  bool OnMousePressed(const ui::MouseEvent& event) override;
  bool OnMouseDragged(const ui::MouseEvent& event) override;
  void OnMouseReleased(const ui::MouseEvent& event) override;
  void OnDeviceScaleFactorChanged(float old_device_scale_factor,
                                  float new_device_scale_factor) override;
  void OnThemeChanged() override;

 private:
  void CreateView();

  void Close();

  views::View* CreateHeaderView();

  views::ImageView* CreateIconView();

  views::Label* CreateTitleLabel();

  views::View* CreateButtonView();

  LuxxleTooltipLabelButton* CreateOkButton();
  void OnOkButtonPressed();

  LuxxleTooltipLabelButton* CreateCancelButton();
  void OnCancelButtonPressed();

  views::View* CreateBodyView();
  views::Label* CreateBodyLabel();

  void UpdateTitleLabelColors();
  void UpdateBodyLabelColors();
  void UpdateOkButtonColors();
  void UpdateCancelButtonColors();

  raw_ptr<LuxxleTooltipPopup> tooltip_popup_;
  LuxxleTooltipAttributes tooltip_attributes_;

  gfx::Point initial_mouse_pressed_location_;
  bool is_dragging_ = false;

  bool is_closing_ = false;

  raw_ptr<views::Label> title_label_ = nullptr;
  raw_ptr<views::Label> body_label_ = nullptr;

  raw_ptr<views::LabelButton> ok_button_ = nullptr;
  raw_ptr<views::LabelButton> cancel_button_ = nullptr;
};

}  // namespace luxxle_tooltips

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_VIEW_H_
