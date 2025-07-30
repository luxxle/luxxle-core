/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/themes/theme_service_factory.h"

#include "luxxle/browser/themes/luxxle_theme_service.h"

#if !BUILDFLAG(IS_LINUX)
#define LUXXLE_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR \
  using ThemeService = LuxxleThemeService;
#else
// On Linux ThemeServiceAuraLinux derives from LuxxleThemeService instead.
#define LUXXLE_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR
#endif

#include "src/chrome/browser/themes/theme_service_factory.cc"

#undef LUXXLE_THEMESERVICEFACTORY_BUILDSERVICEINSTANCEFOR
