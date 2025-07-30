/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_TOOLBAR_VIEW_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_TOOLBAR_VIEW_H_

#include "chrome/browser/ui/views/frame/browser_root_view.h"
#include "chrome/browser/ui/views/location_bar/custom_tab_bar_view.h"

#define Init virtual Init
#define Update virtual Update
#define ShowBookmarkBubble virtual ShowBookmarkBubble
#define LoadImages               \
  LoadImagesUnused();            \
  friend class LuxxleToolbarView; \
  virtual void LoadImages

#include "src/chrome/browser/ui/views/toolbar/toolbar_view.h"  // IWYU pragma: export

#undef Init
#undef Update
#undef ShowBookmarkBubble
#undef LoadImages

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TOOLBAR_TOOLBAR_VIEW_H_
