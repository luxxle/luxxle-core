/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * you can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_

#define CreateHoverCard                     \
  CreateHoverCard_Unused();                 \
  friend class LuxxleTabHoverTest;           \
  friend class LuxxleTabHoverCardController; \
  virtual void CreateHoverCard

#define OnHovercardImagesEnabledChanged     \
  OnHovercardImagesEnabledChanged_Unused(); \
  virtual void OnHovercardImagesEnabledChanged

#include "src/chrome/browser/ui/views/tabs/tab_hover_card_controller.h"  // IWYU pragma: export

#undef OnHovercardImagesEnabledChanged
#undef CreateHoverCard

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_HOVER_CARD_CONTROLLER_H_
