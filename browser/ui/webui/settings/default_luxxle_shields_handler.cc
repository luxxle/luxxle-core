/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/default_luxxle_shields_handler.h"

#include <utility>

#include "base/functional/bind.h"
#include "base/values.h"
#include "luxxle/browser/webcompat_reporter/webcompat_reporter_service_factory.h"
#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_util.h"
#include "luxxle/components/luxxle_shields/core/common/features.h"
#include "luxxle/components/webcompat_reporter/browser/webcompat_reporter_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/cookie_settings_factory.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/content_settings/core/browser/cookie_settings.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "content/public/browser/web_ui.h"
#include "url/gurl.h"

using luxxle_shields::ControlType;
using luxxle_shields::ControlTypeFromString;
using luxxle_shields::ControlTypeToString;

DefaultLuxxleShieldsHandler::DefaultLuxxleShieldsHandler() = default;
DefaultLuxxleShieldsHandler::~DefaultLuxxleShieldsHandler() = default;

void DefaultLuxxleShieldsHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());
  web_ui()->RegisterMessageCallback(
      "isAdControlEnabled",
      base::BindRepeating(&DefaultLuxxleShieldsHandler::IsAdControlEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setAdControlType",
      base::BindRepeating(&DefaultLuxxleShieldsHandler::SetAdControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isFirstPartyCosmeticFilteringEnabled",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::IsFirstPartyCosmeticFilteringEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setCosmeticFilteringControlType",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::SetCosmeticFilteringControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getCookieControlType",
      base::BindRepeating(&DefaultLuxxleShieldsHandler::GetCookieControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setCookieControlType",
      base::BindRepeating(&DefaultLuxxleShieldsHandler::SetCookieControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getFingerprintingControlType",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::GetFingerprintingControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setFingerprintingControlType",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::SetFingerprintingControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getFingerprintingBlockEnabled",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::GetFingerprintingBlockEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setFingerprintingBlockEnabled",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::SetFingerprintingBlockEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getHttpsUpgradeControlType",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::GetHttpsUpgradeControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setHttpsUpgradeControlType",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::SetHttpsUpgradeControlType,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setNoScriptControlType",
      base::BindRepeating(&DefaultLuxxleShieldsHandler::SetNoScriptControlType,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getForgetFirstPartyStorageEnabled",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::GetForgetFirstPartyStorageEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setForgetFirstPartyStorageEnabled",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::SetForgetFirstPartyStorageEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setContactInfoSaveFlag",
      base::BindRepeating(&DefaultLuxxleShieldsHandler::SetContactInfoSaveFlag,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getContactInfo",
      base::BindRepeating(&DefaultLuxxleShieldsHandler::GetContactInfo,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getHideBlockAllCookieTogle",
      base::BindRepeating(
          &DefaultLuxxleShieldsHandler::GetHideBlockAllCookieFlag,
          base::Unretained(this)));

  content_settings_observation_.Observe(
      HostContentSettingsMapFactory::GetForProfile(profile_));
  cookie_settings_observation_.Observe(
      CookieSettingsFactory::GetForProfile(profile_).get());
}

void DefaultLuxxleShieldsHandler::OnContentSettingChanged(
    const ContentSettingsPattern& primary_pattern,
    const ContentSettingsPattern& secondary_pattern,
    ContentSettingsTypeSet content_type_set) {
  if (!content_type_set.Contains(ContentSettingsType::COOKIES) &&
      !content_type_set.Contains(
          ContentSettingsType::LUXXLE_COSMETIC_FILTERING) &&
      !content_type_set.Contains(ContentSettingsType::LUXXLE_TRACKERS) &&
      !content_type_set.Contains(
          ContentSettingsType::LUXXLE_HTTP_UPGRADABLE_RESOURCES) &&
      !content_type_set.Contains(
          ContentSettingsType::LUXXLE_FINGERPRINTING_V2) &&
      !content_type_set.Contains(ContentSettingsType::LUXXLE_SHIELDS) &&
      !content_type_set.Contains(ContentSettingsType::LUXXLE_HTTPS_UPGRADE) &&
      !content_type_set.Contains(
          ContentSettingsType::LUXXLE_REMEMBER_1P_STORAGE)) {
    return;
  }

  if (primary_pattern != ContentSettingsPattern::Wildcard() &&
      secondary_pattern != ContentSettingsPattern::Wildcard()) {
    return;
  }

  if (!IsJavascriptAllowed()) {
    return;
  }
  FireWebUIListener("luxxle-shields-settings-changed");
}

void DefaultLuxxleShieldsHandler::OnThirdPartyCookieBlockingChanged(
    bool block_third_party_cookies) {
  if (!IsJavascriptAllowed()) {
    return;
  }
  FireWebUIListener("luxxle-shields-settings-changed");
}

void DefaultLuxxleShieldsHandler::IsAdControlEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);

  ControlType setting = luxxle_shields::GetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_), GURL());

  AllowJavascript();
  ResolveJavascriptCallback(args[0],
                            base::Value(setting == ControlType::BLOCK));
}

void DefaultLuxxleShieldsHandler::SetAdControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  bool value = args[0].GetBool();

  luxxle_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value ? ControlType::BLOCK : ControlType::ALLOW, GURL(),
      g_browser_process->local_state());
}

void DefaultLuxxleShieldsHandler::IsFirstPartyCosmeticFilteringEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);

  bool enabled = luxxle_shields::IsFirstPartyCosmeticFilteringEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile_), GURL());

  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(enabled));
}

void DefaultLuxxleShieldsHandler::SetCosmeticFilteringControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  std::string value = args[0].GetString();

  luxxle_shields::SetCosmeticFilteringControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value), GURL(), g_browser_process->local_state(),
      profile_->GetPrefs());
}

void DefaultLuxxleShieldsHandler::GetCookieControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);

  const ControlType setting = luxxle_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      CookieSettingsFactory::GetForProfile(profile_).get(), GURL());

  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(ControlTypeToString(setting)));
}

void DefaultLuxxleShieldsHandler::GetHideBlockAllCookieFlag(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  const ControlType setting = luxxle_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      CookieSettingsFactory::GetForProfile(profile_).get(), GURL());

  const bool block_all_cookies_feature_enabled = base::FeatureList::IsEnabled(
      luxxle_shields::features::kBlockAllCookiesToggle);

  AllowJavascript();
  ResolveJavascriptCallback(args[0],
                            base::Value(setting != ControlType::BLOCK &&
                                        !block_all_cookies_feature_enabled));
}

void DefaultLuxxleShieldsHandler::SetCookieControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  std::string value = args[0].GetString();

  luxxle_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      profile_->GetPrefs(), ControlTypeFromString(value), GURL(),
      g_browser_process->local_state());
}

void DefaultLuxxleShieldsHandler::GetFingerprintingControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);

  ControlType setting = luxxle_shields::GetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_), GURL());

  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(ControlTypeToString(setting)));
}

void DefaultLuxxleShieldsHandler::SetFingerprintingControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  std::string value = args[0].GetString();

  luxxle_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value), GURL(), g_browser_process->local_state(),
      profile_->GetPrefs());
}

void DefaultLuxxleShieldsHandler::GetFingerprintingBlockEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);

  ControlType setting = luxxle_shields::GetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_), GURL());
  bool result = setting != ControlType::ALLOW;
  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(result));
}

void DefaultLuxxleShieldsHandler::SetFingerprintingBlockEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  bool value = args[0].GetBool();

  luxxle_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value ? ControlType::DEFAULT : ControlType::ALLOW, GURL(),
      g_browser_process->local_state());
}

void DefaultLuxxleShieldsHandler::GetHttpsUpgradeControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);

  ControlType setting = luxxle_shields::GetHttpsUpgradeControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_), GURL());

  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(ControlTypeToString(setting)));
}

void DefaultLuxxleShieldsHandler::SetHttpsUpgradeControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  std::string value = args[0].GetString();

  luxxle_shields::SetHttpsUpgradeControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      ControlTypeFromString(value), GURL(), g_browser_process->local_state());
}

void DefaultLuxxleShieldsHandler::SetNoScriptControlType(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  bool value = args[0].GetBool();

  luxxle_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(profile_),
      value ? ControlType::BLOCK : ControlType::ALLOW, GURL(),
      g_browser_process->local_state());
}

void DefaultLuxxleShieldsHandler::SetContactInfoSaveFlag(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  if (!args[0].is_bool()) {
    return;
  }
  bool value = args[0].GetBool();

  auto* webcompat_reporter_service =
      webcompat_reporter::WebcompatReporterServiceFactory::GetServiceForContext(
          profile_);
  if (webcompat_reporter_service) {
    webcompat_reporter_service->SetContactInfoSaveFlag(value);
  }
}

void DefaultLuxxleShieldsHandler::GetContactInfo(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);
  AllowJavascript();

  auto* webcompat_reporter_service =
      webcompat_reporter::WebcompatReporterServiceFactory::GetServiceForContext(
          profile_);
  if (!webcompat_reporter_service) {
    base::Value::Dict params_dict;
    params_dict.Set("contactInfo", "");
    params_dict.Set("contactInfoSaveFlag", false);
    ResolveJavascriptCallback(args[0], std::move(params_dict));
    return;
  }

  webcompat_reporter_service->GetContactInfo(
      base::BindOnce(&DefaultLuxxleShieldsHandler::OnGetContactInfo,
                     weak_ptr_factory_.GetWeakPtr(), args[0].Clone()));
}
void DefaultLuxxleShieldsHandler::OnGetContactInfo(
    base::Value javascript_callback,
    const std::optional<std::string>& contact_info,
    const bool contact_info_save_flag) {
  base::Value::Dict params_dict;
  params_dict.Set("contactInfo", contact_info.value_or(""));
  params_dict.Set("contactInfoSaveFlag", contact_info_save_flag);
  ResolveJavascriptCallback(javascript_callback, std::move(params_dict));
}

void DefaultLuxxleShieldsHandler::SetForgetFirstPartyStorageEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);

  bool value = args[0].GetBool();

  luxxle_shields::SetForgetFirstPartyStorageEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile_), value, GURL(),
      g_browser_process->local_state());
}

void DefaultLuxxleShieldsHandler::GetForgetFirstPartyStorageEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  CHECK(profile_);

  const bool result = luxxle_shields::GetForgetFirstPartyStorageEnabled(
      HostContentSettingsMapFactory::GetForProfile(profile_), GURL());

  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(result));
}
