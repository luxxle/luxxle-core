/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_THEMES_LUXXLE_DARK_MODE_UTILS_H_
#define LUXXLE_BROWSER_THEMES_LUXXLE_DARK_MODE_UTILS_H_

#include <string>

#include "base/values.h"
#include "build/build_config.h"

class PrefRegistrySimple;

namespace dark_mode {

enum class LuxxleDarkModeType {
  // DEFAULT type acts as two ways depends on system theme mode.
  // If system dark mode is not supported, we override it with channel based
  // policy. In this case, user can see dark or light option in settings.
  // Otherwise, it acts like system dark mode mode. It respects system's dark
  // mode. In this case, user can see all three options in theme settings.
  LUXXLE_DARK_MODE_TYPE_DEFAULT,
  LUXXLE_DARK_MODE_TYPE_DARK,
  LUXXLE_DARK_MODE_TYPE_LIGHT,
};

// APIs for prefs.
void RegisterLuxxleDarkModeLocalStatePrefs(PrefRegistrySimple* registry);

std::string GetStringFromLuxxleDarkModeType(LuxxleDarkModeType type);
base::Value::List GetLuxxleDarkModeTypeList();
void SetLuxxleDarkModeType(const std::string& type);
void SetLuxxleDarkModeType(LuxxleDarkModeType type);
// Returns current effective theme type. dark or light.
LuxxleDarkModeType GetActiveLuxxleDarkModeType();
// Returns current theme type.
// dark/light will be returned if platform doesn't support system dark mode.
// Otherwise, returns default/dark/light.
LuxxleDarkModeType GetLuxxleDarkModeType();
bool SystemDarkModeEnabled();
void SetUseSystemDarkModeEnabledForTest(bool enabled);

// When system supports system per-application system theme changing, set it.
// Currently, only MacOS support it.
// Otherewise, we need to overrides from native theme level and explicitly
// notifying to let observers know.
// By overriding, base ui components also use same luxxle theme type.
void SetSystemDarkMode(LuxxleDarkModeType type);

#if BUILDFLAG(IS_LINUX)
// Cache system preference from DarkModeManagerLinux.
// This cached value is used whenever user chooses "Same as Linux" option.
void CacheSystemDarkModePrefs(bool prefer_dark_theme);
bool HasCachedSystemDarkModeType();
#endif

}  // namespace dark_mode

#endif  // LUXXLE_BROWSER_THEMES_LUXXLE_DARK_MODE_UTILS_H_
