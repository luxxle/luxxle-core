/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "build/build_config.h"

#if !BUILDFLAG(IS_ANDROID)

#include "luxxle/grit/luxxle_theme_resources_map.h"

#define LUXXLE_RESOURCES_UTIL                              \
  for (size_t i = 0; i < kLuxxleThemeResourcesSize; ++i) { \
    storage.emplace_back(kLuxxleThemeResources[i].path,    \
                         kLuxxleThemeResources[i].id);     \
  }

#else
#define LUXXLE_RESOURCES_UTIL
#endif  // !BUILDFLAG(IS_ANDROID)

#include "src/chrome/browser/resources_util.cc"
#undef LUXXLE_RESOURCES_UTIL
