/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_POPUP_HANDLER_H_
#define LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_POPUP_HANDLER_H_

#include <memory>
#include <string>

namespace luxxle_tooltips {

class LuxxleTooltip;

class LuxxleTooltipPopupHandler {
 public:
  LuxxleTooltipPopupHandler();
  ~LuxxleTooltipPopupHandler();

  LuxxleTooltipPopupHandler(const LuxxleTooltipPopupHandler&) = delete;
  LuxxleTooltipPopupHandler& operator=(const LuxxleTooltipPopupHandler&) = delete;

  // Show the |tooltip|.
  static void Show(std::unique_ptr<LuxxleTooltip> tooltip);

  // Close the tooltip with the associated |tooltip_id|.
  static void Close(const std::string& tooltip_id);

  // Destroy the tooltip with the associated |tooltip_id|.
  static void Destroy(const std::string& tooltip_id);
};

}  // namespace luxxle_tooltips

#endif  // LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_POPUP_HANDLER_H_
