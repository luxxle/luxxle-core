/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_GROUP_UNDERLINE_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_GROUP_UNDERLINE_H_

#define UpdateBounds virtual UpdateBounds
#define GetInsetsForUnderline virtual GetInsetsForUnderline
#define CalculateTabGroupUnderlineBounds      \
  Unused_CalculateTabGroupUnderlineBounds() { \
    return {};                                \
  }                                           \
  friend class LuxxleTabGroupUnderline;        \
  virtual gfx::Rect CalculateTabGroupUnderlineBounds

#include "src/chrome/browser/ui/views/tabs/tab_group_underline.h"  // IWYU pragma: export

#undef CalculateTabGroupUnderlineBounds
#undef GetInsetsForUnderline
#undef UpdateBounds

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_GROUP_UNDERLINE_H_
