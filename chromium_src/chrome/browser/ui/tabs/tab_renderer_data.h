/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_TABS_TAB_RENDERER_DATA_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_TABS_TAB_RENDERER_DATA_H_

#define FromTabInModel                                                \
  FromTabInModel_ChromiumImpl(const TabStripModel* model, int index); \
  static TabRendererData FromTabInModel

#include "src/chrome/browser/ui/tabs/tab_renderer_data.h"  // IWYU pragma: export
#undef FromTabInModel

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_TABS_TAB_RENDERER_DATA_H_
