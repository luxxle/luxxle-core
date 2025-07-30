/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/luxxle_extensions_browser_api_provider.h"

#include "luxxle/browser/extensions/api/generated_api_registration.h"
#include "extensions/browser/extension_function_registry.h"

namespace extensions {

LuxxleExtensionsBrowserAPIProvider::LuxxleExtensionsBrowserAPIProvider() =
    default;
LuxxleExtensionsBrowserAPIProvider::~LuxxleExtensionsBrowserAPIProvider() =
    default;

void LuxxleExtensionsBrowserAPIProvider::RegisterExtensionFunctions(
    ExtensionFunctionRegistry* registry) {
  // Generated APIs from Luxxle.
  api::LuxxleGeneratedFunctionRegistry::RegisterAll(registry);
}

}  // namespace extensions
