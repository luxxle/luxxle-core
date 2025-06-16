/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_BRAVE_LAYOUT_CONSTANTS_H_
#define LUXXLE_BROWSER_UI_BRAVE_LAYOUT_CONSTANTS_H_

#include <optional>

#include "chrome/browser/ui/layout_constants.h"

std::optional<gfx::Insets> GetBraveLayoutInsets(LayoutInset inset);
std::optional<int> GetBraveLayoutConstant(LayoutConstant constant);

#endif  // LUXXLE_BROWSER_UI_BRAVE_LAYOUT_CONSTANTS_H_
