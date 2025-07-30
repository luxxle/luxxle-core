/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"

namespace dark_mode {

void RegisterLuxxleDarkModeLocalStatePrefs(PrefRegistrySimple* registry) {
}

bool SystemDarkModeEnabled() {
  return false;
}

void SetUseSystemDarkModeEnabledForTest(bool enabled) {
}

std::string GetStringFromLuxxleDarkModeType(LuxxleDarkModeType type) {
  return "Default";
}

void SetLuxxleDarkModeType(const std::string& type) {
}

void SetLuxxleDarkModeType(LuxxleDarkModeType type) {
}

LuxxleDarkModeType GetActiveLuxxleDarkModeType() {
  return LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_DEFAULT;
}

LuxxleDarkModeType GetLuxxleDarkModeType() {
  return LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_DEFAULT;
}

base::Value::List GetLuxxleDarkModeTypeList() {
  return base::Value::List();
}

void SetSystemDarkMode(LuxxleDarkModeType type) {
}

}  // namespace dark_mode
