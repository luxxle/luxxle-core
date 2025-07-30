/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_ACTIONS_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_ACTIONS_H_

#define InitializeBrowserActions    \
  UnUsed() {}                       \
  friend class LuxxleBrowserActions; \
  virtual void InitializeBrowserActions

#include "src/chrome/browser/ui/browser_actions.h"  // IWYU pragma: export

#undef InitializeBrowserActions

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_BROWSER_ACTIONS_H_
