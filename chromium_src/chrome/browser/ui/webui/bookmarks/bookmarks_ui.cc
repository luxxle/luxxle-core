/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/navigation_bar_data_provider.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/buildflags.h"
#include "content/public/browser/web_ui_data_source.h"

namespace {

void LuxxleAddBookmarksResources(content::WebUIDataSource* source,
                                Profile* profile) {
  NavigationBarDataProvider::Initialize(source, profile);
  source->AddLocalizedString("emptyList",
                             IDS_LUXXLE_BOOKMARK_MANAGER_EMPTY_LIST);
}

}  // namespace

#define LUXXLE_CREATE_BOOKMARKS_UI_HTML_SOURCE \
  LuxxleAddBookmarksResources(source, profile);

#include "src/chrome/browser/ui/webui/bookmarks/bookmarks_ui.cc"

#undef LUXXLE_CREATE_BOOKMARKS_UI_HTML_SOURCE
