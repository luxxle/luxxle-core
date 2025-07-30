/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_UTIL_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_UTIL_H_

#define GetInstallPromptPermissionSetForExtension \
  GetInstallPromptPermissionSetForExtension_ChromiumImpl
#include "src/chrome/browser/extensions/extension_util.h"  // IWYU pragma: export
#undef GetInstallPromptPermissionSetForExtension

namespace extensions {
namespace util {
std::unique_ptr<const PermissionSet> GetInstallPromptPermissionSetForExtension(
    const Extension* extension,
    Profile* profile);
}  // namespace util
}  // namespace extensions

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_UTIL_H_
