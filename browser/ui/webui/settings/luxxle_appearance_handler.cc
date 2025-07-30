/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_appearance_handler.h"

#include "base/functional/bind.h"
#include "base/metrics/histogram_macros.h"
#include "luxxle/browser/new_tab/new_tab_shows_options.h"
#include "luxxle/browser/profiles/profile_util.h"
#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/ntp_background_images/common/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search/instant_service.h"
#include "chrome/browser/search/instant_service_factory.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"

LuxxleAppearanceHandler::LuxxleAppearanceHandler() {
  local_state_change_registrar_.Init(g_browser_process->local_state());
  local_state_change_registrar_.Add(
      kLuxxleDarkMode,
      base::BindRepeating(&LuxxleAppearanceHandler::OnLuxxleDarkModeChanged,
                          base::Unretained(this)));
}

LuxxleAppearanceHandler::~LuxxleAppearanceHandler() = default;

// TODO(simonhong): Use separate handler for NTP settings.
void LuxxleAppearanceHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  profile_state_change_registrar_.Init(profile_->GetPrefs());
  profile_state_change_registrar_.Add(
      kNewTabPageShowsOptions,
      base::BindRepeating(&LuxxleAppearanceHandler::OnPreferenceChanged,
                          base::Unretained(this)));
  profile_state_change_registrar_.Add(
      prefs::kHomePageIsNewTabPage,
      base::BindRepeating(&LuxxleAppearanceHandler::OnPreferenceChanged,
                          base::Unretained(this)));
  profile_state_change_registrar_.Add(
      prefs::kHomePage,
      base::BindRepeating(&LuxxleAppearanceHandler::OnPreferenceChanged,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setLuxxleThemeType",
      base::BindRepeating(&LuxxleAppearanceHandler::SetLuxxleThemeType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getLuxxleThemeType",
      base::BindRepeating(&LuxxleAppearanceHandler::GetLuxxleThemeType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNewTabShowsOptionsList",
      base::BindRepeating(&LuxxleAppearanceHandler::GetNewTabShowsOptionsList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "shouldShowNewTabDashboardSettings",
      base::BindRepeating(
          &LuxxleAppearanceHandler::ShouldShowNewTabDashboardSettings,
          base::Unretained(this)));
}

void LuxxleAppearanceHandler::SetLuxxleThemeType(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(args[0].is_int());
  AllowJavascript();

  int int_type = args[0].GetInt();
  dark_mode::SetLuxxleDarkModeType(
      static_cast<dark_mode::LuxxleDarkModeType>(int_type));
}

void LuxxleAppearanceHandler::GetLuxxleThemeType(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  // GetLuxxleThemeType() should be used because settings option displays all
  // available options including default.
  ResolveJavascriptCallback(
      args[0],
      base::Value(static_cast<int>(dark_mode::GetLuxxleDarkModeType())));
}

void LuxxleAppearanceHandler::OnLuxxleDarkModeChanged() {
  // GetLuxxleThemeType() should be used because settings option displays all
  // available options including default.
  if (IsJavascriptAllowed()) {
    FireWebUIListener(
        "luxxle-theme-type-changed",
        base::Value(static_cast<int>(dark_mode::GetLuxxleDarkModeType())));
  }
}

void LuxxleAppearanceHandler::OnPreferenceChanged(const std::string& pref_name) {
  if (IsJavascriptAllowed()) {
    if (pref_name == kNewTabPageShowsOptions || pref_name == prefs::kHomePage ||
        pref_name == prefs::kHomePageIsNewTabPage) {
      FireWebUIListener(
          "show-new-tab-dashboard-settings-changed",
          base::Value(luxxle::ShouldNewTabShowDashboard(profile_)));
      return;
    }
  }
}

void LuxxleAppearanceHandler::GetNewTabShowsOptionsList(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0], base::Value(luxxle::GetNewTabShowsOptionsList(profile_)));
}

void LuxxleAppearanceHandler::ShouldShowNewTabDashboardSettings(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0], base::Value(luxxle::ShouldNewTabShowDashboard(profile_)));
}
