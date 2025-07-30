/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PREDICTION_BASED_PERMISSION_UI_SELECTOR_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PREDICTION_BASED_PERMISSION_UI_SELECTOR_H_

#include "components/permissions/permission_ui_selector.h"

#define IsPermissionRequestSupported          \
  IsPermissionRequestSupported_ChromiumImpl(  \
      permissions::RequestType request_type); \
  bool IsPermissionRequestSupported

#include "src/chrome/browser/permissions/prediction_based_permission_ui_selector.h"  // IWYU pragma: export
#undef IsPermissionRequestSupported

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PREDICTION_BASED_PERMISSION_UI_SELECTOR_H_
