/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_SERVICE_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_SERVICE_H_

#define BlocklistStatesInteractionUnitTest \
  BlocklistStatesInteractionUnitTest;      \
  friend class LuxxleExtensionService

#include "src/chrome/browser/extensions/extension_service.h"  // IWYU pragma: export
#undef BlocklistStatesInteractionUnitTest

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_EXTENSION_SERVICE_H_
