/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_LABEL_BUTTON_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_LABEL_BUTTON_H_

#include <string>

#include "ui/views/controls/button/label_button.h"

namespace luxxle_tooltips {

class LuxxleTooltipLabelButton : public views::LabelButton {
 public:
  // Creates a LuxxleTooltipLabelButton with pressed events sent to |callback|
  // and label |text|. |button_context| is a value from
  // views::style::TextContext and determines the appearance of |text|.
  explicit LuxxleTooltipLabelButton(
      PressedCallback callback = PressedCallback(),
      const std::u16string& text = std::u16string(),
      int button_context = views::style::CONTEXT_BUTTON);
  ~LuxxleTooltipLabelButton() override;

  LuxxleTooltipLabelButton(const LuxxleTooltipLabelButton&) = delete;
  LuxxleTooltipLabelButton& operator=(const LuxxleTooltipLabelButton&) = delete;

  // views::LabelButton:
  ui::Cursor GetCursor(const ui::MouseEvent& event) override;
};

}  // namespace luxxle_tooltips

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_LABEL_BUTTON_H_
