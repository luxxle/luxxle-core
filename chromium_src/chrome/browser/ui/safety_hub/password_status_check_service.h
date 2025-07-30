/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_SAFETY_HUB_PASSWORD_STATUS_CHECK_SERVICE_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_SAFETY_HUB_PASSWORD_STATUS_CHECK_SERVICE_H_

#define GetPasswordCardData                         \
  GetPasswordCardData_ChromiumImpl(bool signed_in); \
  base::Value::Dict GetPasswordCardData

#include "src/chrome/browser/ui/safety_hub/password_status_check_service.h"  // IWYU pragma: export

#undef GetPasswordCardData

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_SAFETY_HUB_PASSWORD_STATUS_CHECK_SERVICE_H_
