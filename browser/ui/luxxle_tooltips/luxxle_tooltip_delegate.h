/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_DELEGATE_H_
#define LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_DELEGATE_H_

#include <string>

#include "base/memory/weak_ptr.h"

namespace luxxle_tooltips {

class LuxxleTooltipDelegate {
 public:
  virtual ~LuxxleTooltipDelegate() = default;

  // Called when the tooltip is shown
  virtual void OnTooltipShow(const std::string& tooltip_id) {}

  // Called when the tooltip is closed
  virtual void OnTooltipClose(const std::string& tooltip_id) {}

  // Called when the underlying widget for the tooltip is destroyed
  virtual void OnTooltipWidgetDestroyed(const std::string& tooltip_id) {}

  // Called when the Ok button is pressed
  virtual void OnTooltipOkButtonPressed(const std::string& tooltip_id) {}

  // Called when the Cancel button is pressed
  virtual void OnTooltipCancelButtonPressed(const std::string& tooltip_id) {}

  // Returns a WeakPtr to the implementation instance.
  virtual base::WeakPtr<LuxxleTooltipDelegate> AsWeakPtr() = 0;
};

}  // namespace luxxle_tooltips

#endif  // LUXXLE_BROWSER_UI_LUXXLE_TOOLTIPS_LUXXLE_TOOLTIP_DELEGATE_H_
