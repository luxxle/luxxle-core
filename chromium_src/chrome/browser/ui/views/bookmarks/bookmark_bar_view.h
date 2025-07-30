/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_VIEW_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_VIEW_H_

#define UpdateOtherAndManagedButtonsVisibility     \
  UpdateOtherAndManagedButtonsVisibility_Unused(); \
  friend class LuxxleBookmarkBarView;               \
  virtual bool UpdateOtherAndManagedButtonsVisibility

#include "src/chrome/browser/ui/views/bookmarks/bookmark_bar_view.h"  // IWYU pragma: export

#undef UpdateOtherAndManagedButtonsVisibility

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_BAR_VIEW_H_
