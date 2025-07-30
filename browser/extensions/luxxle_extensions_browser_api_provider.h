/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_EXTENSIONS_LUXXLE_EXTENSIONS_BROWSER_API_PROVIDER_H_
#define LUXXLE_BROWSER_EXTENSIONS_LUXXLE_EXTENSIONS_BROWSER_API_PROVIDER_H_

#include "extensions/browser/extensions_browser_api_provider.h"

namespace extensions {

class LuxxleExtensionsBrowserAPIProvider : public ExtensionsBrowserAPIProvider {
 public:
  LuxxleExtensionsBrowserAPIProvider();
  LuxxleExtensionsBrowserAPIProvider(const LuxxleExtensionsBrowserAPIProvider&) =
      delete;
  LuxxleExtensionsBrowserAPIProvider& operator=(
      const LuxxleExtensionsBrowserAPIProvider&) = delete;
  ~LuxxleExtensionsBrowserAPIProvider() override;

  void RegisterExtensionFunctions(ExtensionFunctionRegistry* registry) override;
};

}  // namespace extensions

#endif  // LUXXLE_BROWSER_EXTENSIONS_LUXXLE_EXTENSIONS_BROWSER_API_PROVIDER_H_
