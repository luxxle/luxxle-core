/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_TOOLBAR_CHROME_LOCATION_BAR_MODEL_DELEGATE_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_TOOLBAR_CHROME_LOCATION_BAR_MODEL_DELEGATE_H_

#define GetNavigationEntry                    \
  GetNavigationEntryUnused();                 \
  friend class LuxxleLocationBarModelDelegate; \
  content::NavigationEntry* GetNavigationEntry

#include "src/chrome/browser/ui/toolbar/chrome_location_bar_model_delegate.h"  // IWYU pragma: export
#undef GetNavigationEntry

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_TOOLBAR_CHROME_LOCATION_BAR_MODEL_DELEGATE_H_
