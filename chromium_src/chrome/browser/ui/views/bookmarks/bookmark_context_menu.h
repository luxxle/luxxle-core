/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_H_

#include "luxxle/browser/ui/toolbar/luxxle_bookmark_context_menu_controller.h"

#define BookmarkContextMenuController LuxxleBookmarkContextMenuController
#define close_on_remove_ \
  close_on_remove_;      \
  friend class LuxxleBookmarkContextMenu
#include "src/chrome/browser/ui/views/bookmarks/bookmark_context_menu.h"  // IWYU pragma: export
#undef BookmarkContextMenuController
#undef close_on_remove_

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_BOOKMARK_CONTEXT_MENU_H_
