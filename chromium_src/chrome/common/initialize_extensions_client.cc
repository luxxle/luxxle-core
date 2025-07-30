// Copyright (c) 2021 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "luxxle/common/extensions/luxxle_extensions_client.h"

#define ChromeExtensionsClient LuxxleExtensionsClient
#include "src/chrome/common/initialize_extensions_client.cc"
#undef ChromeExtensionsClient
