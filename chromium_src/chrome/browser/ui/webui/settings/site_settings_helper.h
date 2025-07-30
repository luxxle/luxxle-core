/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_SITE_SETTINGS_HELPER_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_SITE_SETTINGS_HELPER_H_

#include "components/content_settings/core/common/content_settings.h"

#define kAdsFilterBlocklist kAdsFilterBlocklist, kRemoteList

#include "src/chrome/browser/ui/webui/settings/site_settings_helper.h"  // IWYU pragma: export

#undef kAdsFilterBlocklist

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_WEBUI_SETTINGS_SITE_SETTINGS_HELPER_H_
