/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_COMMON_WEBUI_URL_CONSTANTS_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_COMMON_WEBUI_URL_CONSTANTS_H_

#define kPerformanceSubPage kPerformanceSubPage_UnUsed
#include "src/chrome/common/webui_url_constants.h"  // IWYU pragma: export
#undef kPerformanceSubPage

namespace chrome {

inline constexpr char kPerformanceSubPage[] = "system";
inline constexpr char kLuxxleUISyncHost[] = "sync";

}  // namespace chrome

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_COMMON_WEBUI_URL_CONSTANTS_H_
