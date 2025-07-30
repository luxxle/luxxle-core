/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/luxxle_tooltips/luxxle_tooltip_label_button.h"

#include <utility>

#include "ui/base/cursor/cursor.h"

namespace luxxle_tooltips {

LuxxleTooltipLabelButton::LuxxleTooltipLabelButton(PressedCallback callback,
                                                 const std::u16string& text,
                                                 int button_context)
    : LabelButton(std::move(callback), text, button_context) {}

LuxxleTooltipLabelButton::~LuxxleTooltipLabelButton() = default;

ui::Cursor LuxxleTooltipLabelButton::GetCursor(const ui::MouseEvent& event) {
  if (!GetEnabled())
    return ui::Cursor();
  return ui::mojom::CursorType::kHand;
}

}  // namespace luxxle_tooltips
