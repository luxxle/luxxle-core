/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// It has caller in the same file
#define LUXXLE_GET_SETTINGS_URL \
  GURL GetSettingsUrl_ChromiumImpl(std::string_view sub_page) {
// LUXXLE_GET_SETTINGS_URL

#include "src/chrome/browser/ui/chrome_pages.cc"

#undef LUXXLE_GET_SETTINGS_URL

#include "luxxle/components/constants/webui_url_constants.h"

namespace chrome {

GURL GetSettingsUrl(std::string_view sub_page) {
  if (sub_page == chrome::kSyncSetupSubPage)
    return chrome::GetSettingsUrl(kLuxxleSyncSetupPath);
  return GetSettingsUrl_ChromiumImpl(sub_page);
}

}  // namespace chrome
