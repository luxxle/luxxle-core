/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_BROWSER_APP_MENU_BUTTON_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_BROWSER_APP_MENU_BUTTON_H_

#define GetHighlightColor virtual GetHighlightColor
#define UpdateLayoutInsets virtual UpdateLayoutInsets
#define UpdateTextAndHighlightColor       \
  UpdateTextAndHighlightColor_UnUsed() {} \
  friend class LuxxleBrowserAppMenuButton; \
  void UpdateTextAndHighlightColor

#include "src/chrome/browser/ui/views/toolbar/browser_app_menu_button.h"  // IWYU pragma: export

#undef UpdateTextAndHighlightColor
#undef UpdateLayoutInsets
#undef GetHighlightColor

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_BROWSER_APP_MENU_BUTTON_H_
