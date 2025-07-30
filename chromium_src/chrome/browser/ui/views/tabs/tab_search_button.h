/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_SEARCH_BUTTON_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_SEARCH_BUTTON_H_

#include "chrome/browser/ui/views/tabs/tab_strip_control_button.h"

#define GetCornerRadius              \
  GetCornerRadius_Unused();          \
  friend class LuxxleTabSearchButton; \
  int GetCornerRadius

#include "src/chrome/browser/ui/views/tabs/tab_search_button.h"  // IWYU pragma: export

#undef GetCornerRadius

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_SEARCH_BUTTON_H_
