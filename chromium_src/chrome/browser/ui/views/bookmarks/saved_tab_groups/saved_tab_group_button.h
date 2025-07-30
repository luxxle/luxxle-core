/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_SAVED_TAB_GROUPS_SAVED_TAB_GROUP_BUTTON_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_SAVED_TAB_GROUPS_SAVED_TAB_GROUP_BUTTON_H_

#define UpdateButtonLayout               \
  UpdateButtonLayout_Unused() {}         \
  friend class LuxxleSavedTabGroupButton; \
  virtual void UpdateButtonLayout

#include "src/chrome/browser/ui/views/bookmarks/saved_tab_groups/saved_tab_group_button.h"  // IWYU pragma: export

#undef UpdateButtonLayout

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_BOOKMARKS_SAVED_TAB_GROUPS_SAVED_TAB_GROUP_BUTTON_H_
