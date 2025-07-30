/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/luxxle_extension_provider.h"

#include <string>

#include "extensions/common/constants.h"

namespace extensions {

LuxxleExtensionProvider::LuxxleExtensionProvider() = default;

LuxxleExtensionProvider::~LuxxleExtensionProvider() = default;

std::string LuxxleExtensionProvider::GetDebugPolicyProviderName() const {
#if defined(NDEBUG)
  NOTREACHED();
#else
  return "Luxxle Extension Provider";
#endif
}

bool LuxxleExtensionProvider::MustRemainInstalled(const Extension* extension,
                                                 std::u16string* error) const {
  return extension->id() == luxxle_extension_id;
}

}  // namespace extensions
