/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/navigation_bar_data_provider.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/buildflags.h"
#include "content/public/browser/web_ui_data_source.h"

namespace {

void LuxxleCustomizeHistoryDataSource(content::WebUIDataSource* source,
                                     Profile* profile) {
  NavigationBarDataProvider::Initialize(source, profile);
}

}  // namespace

#define LUXXLE_CREATE_HISTORY_UI_HTML_SOURCE \
  LuxxleCustomizeHistoryDataSource(source, profile);

#include "src/chrome/browser/ui/webui/history/history_ui.cc"
#undef LUXXLE_CREATE_HISTORY_UI_HTML_SOURCE
