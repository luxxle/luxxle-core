// Copyright (c) 2019 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_GRIT_THEME_RESOURCES_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_GRIT_THEME_RESOURCES_H_

#include "build/build_config.h"

#if !BUILDFLAG(IS_ANDROID)
#include "luxxle/grit/luxxle_theme_resources.h"  // IWYU pragma: export
#endif

#include "../gen/chrome/grit/theme_resources.h"  // IWYU pragma: export

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_GRIT_THEME_RESOURCES_H_
