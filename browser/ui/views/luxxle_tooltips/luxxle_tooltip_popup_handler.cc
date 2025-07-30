/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/luxxle_tooltips/luxxle_tooltip_popup_handler.h"

#include <map>
#include <string>

#include "luxxle/browser/ui/luxxle_tooltips/luxxle_tooltip.h"
#include "luxxle/browser/ui/views/luxxle_tooltips/luxxle_tooltip_popup.h"

namespace {

std::map<std::string, luxxle_tooltips::LuxxleTooltipPopup* /* NOT OWNED */>
    tooltip_popups_;

}  // namespace

namespace luxxle_tooltips {

LuxxleTooltipPopupHandler::LuxxleTooltipPopupHandler() = default;

LuxxleTooltipPopupHandler::~LuxxleTooltipPopupHandler() = default;

// static
void LuxxleTooltipPopupHandler::Show(std::unique_ptr<LuxxleTooltip> tooltip) {
  DCHECK(tooltip);

  const std::string tooltip_id = tooltip->id();
  if (!tooltip_popups_[tooltip_id]) {
    tooltip_popups_[tooltip_id] = new LuxxleTooltipPopup(std::move(tooltip));
    tooltip_popups_[tooltip_id]->Show();
  }
}

// static
void LuxxleTooltipPopupHandler::Close(const std::string& tooltip_id) {
  DCHECK(!tooltip_id.empty());

  if (!tooltip_popups_[tooltip_id]) {
    return;
  }

  tooltip_popups_[tooltip_id]->Close();
}

// static
void LuxxleTooltipPopupHandler::Destroy(const std::string& tooltip_id) {
  DCHECK(!tooltip_id.empty());

  // Note: The pointed-to LuxxleTooltipPopup members are deallocated by their
  // containing Widgets
  tooltip_popups_.erase(tooltip_id);
}

}  // namespace luxxle_tooltips
