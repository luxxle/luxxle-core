/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/toolbar/luxxle_bookmark_sub_menu_model.h"

#include "chrome/app/chrome_command_ids.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/grit/generated_resources.h"

LuxxleBookmarkSubMenuModel::LuxxleBookmarkSubMenuModel(
    ui::SimpleMenuModel::Delegate* delegate,
    Browser* browser)
    : BookmarkSubMenuModel(delegate, browser) {
  Build(browser);
}

LuxxleBookmarkSubMenuModel::~LuxxleBookmarkSubMenuModel() = default;

void LuxxleBookmarkSubMenuModel::Build(Browser* browser) {
  auto index = GetIndexOfCommandId(IDC_SHOW_BOOKMARK_BAR);
  if (!index.has_value())
    return;
  RemoveItemAt(index.value());
  luxxle_bookmarks_submenu_model_ =
      std::make_unique<BookmarkBarSubMenuModel>(browser->profile());
  InsertSubMenuWithStringIdAt(index.value(), IDC_LUXXLE_BOOKMARK_BAR_SUBMENU,
                              IDS_SHOW_BOOKMARK_BAR,
                              luxxle_bookmarks_submenu_model_.get());
}
