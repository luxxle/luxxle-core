/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/navigation_bar_data_provider.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/grit/generated_resources.h"

namespace {

void LuxxleCustomizeDownloadsDataSource(content::WebUIDataSource* source,
                                       Profile* profile) {
  NavigationBarDataProvider::Initialize(source, profile);
}

}  // namespace

#if !defined(IDS_DOWNLOAD_HISTORY_TITLE)
#error IDS_DOWNLOAD_HISTORY_TITLE is no longer defined in Chromium
#endif
#undef IDS_DOWNLOAD_HISTORY_TITLE
#define IDS_DOWNLOAD_HISTORY_TITLE IDS_DOWNLOADS_TITLE

#define LUXXLE_CREATE_DOWNLOADS_UI_HTML_SOURCE \
  LuxxleCustomizeDownloadsDataSource(source, profile);

#include "src/chrome/browser/ui/webui/downloads/downloads_ui.cc"
#undef LUXXLE_CREATE_DOWNLOADS_UI_HTML_SOURCE
#undef IDS_DOWNLOAD_HISTORY_TITLE
