/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_BROWSER_TAB_STRIP_CONTROLLER_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_BROWSER_TAB_STRIP_CONTROLLER_H_

#define CloseContextMenuForTesting             \
  CloseContextMenuForTesting_UnUsed() {}       \
  friend class LuxxleBrowserTabStripController; \
  void CloseContextMenuForTesting

#define GetIndexOfLastFocusedTabInSplit virtual GetIndexOfLastFocusedTabInSplit

#include "src/chrome/browser/ui/views/tabs/browser_tab_strip_controller.h"  // IWYU pragma: export

#undef GetIndexOfLastFocusedTabInSplit
#undef CloseContextMenuForTesting

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_BROWSER_TAB_STRIP_CONTROLLER_H_
