/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/webui/settings/site_settings_helper.h"

#include <string_view>
#include <vector>

#include "luxxle/components/luxxle_shields/core/common/luxxle_shield_constants.h"
#include "luxxle/components/content_settings/core/browser/luxxle_content_settings_pref_provider.h"
#include "components/content_settings/core/common/content_settings_pattern.h"

#define HasRegisteredGroupName HasRegisteredGroupName_ChromiumImpl
#define GetVisiblePermissionCategories \
  GetVisiblePermissionCategories_ChromiumImpl

// clang-format off
#define LUXXLE_CONTENT_SETTINGS_TYPE_GROUP_NAMES_LIST                  \
  {ContentSettingsType::LUXXLE_ADS, nullptr},                          \
  {ContentSettingsType::LUXXLE_COSMETIC_FILTERING, nullptr},           \
  {ContentSettingsType::LUXXLE_TRACKERS, nullptr},                     \
  {ContentSettingsType::LUXXLE_HTTP_UPGRADABLE_RESOURCES, nullptr},    \
  {ContentSettingsType::LUXXLE_FINGERPRINTING_V2, nullptr},            \
  {ContentSettingsType::LUXXLE_SHIELDS, luxxle_shields::kLuxxleShields}, \
  {ContentSettingsType::LUXXLE_REFERRERS, nullptr},                    \
  {ContentSettingsType::LUXXLE_COOKIES, nullptr},                      \
  {ContentSettingsType::LUXXLE_SPEEDREADER, nullptr},                  \
  {ContentSettingsType::LUXXLE_ETHEREUM, "ethereum"},                  \
  {ContentSettingsType::LUXXLE_SOLANA, "solana"},                      \
  {ContentSettingsType::LUXXLE_GOOGLE_SIGN_IN, "googleSignIn"},        \
  {ContentSettingsType::LUXXLE_HTTPS_UPGRADE, nullptr},                \
  {ContentSettingsType::LUXXLE_REMEMBER_1P_STORAGE, nullptr},          \
  {ContentSettingsType::LUXXLE_LOCALHOST_ACCESS, "localhostAccess"},   \
  {ContentSettingsType::LUXXLE_OPEN_AI_CHAT, "luxxleOpenAIChat"},       \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_NONE, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_AUDIO, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_CANVAS, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_DEVICE_MEMORY, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_EVENT_SOURCE_POOL, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_FONT, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_HARDWARE_CONCURRENCY, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_KEYBOARD, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_LANGUAGE, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_MEDIA_DEVICES, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_PLUGINS, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_SCREEN, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_SPEECH_SYNTHESIS, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_USB_DEVICE_SERIAL_NUMBER, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_USER_AGENT, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_WEBGL, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_WEBGL2, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_WEB_SOCKETS_POOL, nullptr}, \
  {ContentSettingsType::LUXXLE_WEBCOMPAT_ALL, nullptr}, \
  {ContentSettingsType::LUXXLE_SHIELDS_METADATA, nullptr}, \
  {ContentSettingsType::LUXXLE_CARDANO, "cardano"},
// clang-format on

#define LUXXLE_SITE_SETTINGS_HELPER_CONTENT_SETTINGS_TYPE_FROM_GROUP_NAME \
  if (name == "autoplay")                                                \
    return ContentSettingsType::AUTOPLAY;

#define LUXXLE_SITE_SETTINGS_HELPER_CONTENT_SETTINGS_TYPE_TO_GROUP_NAME \
  if (type == ContentSettingsType::AUTOPLAY)                           \
    return "autoplay";

#define LUXXLE_PROVIDER_TYPE_TO_SITE_SETTINGS_SOURCE \
  case ProviderType::kRemoteListProvider:           \
    return SiteSettingSource::kRemoteList;

#define LUXXLE_PROVIDER_TO_DEFAULT_SETTINGS_STRING \
  case ProviderType::kRemoteListProvider:         \
    return "remote_list";

#define LUXXLE_GET_EXCEPTION_FOR_PAGE                                  \
  LuxxleGetExceptionForPage(content_type, profile, incognito, pattern, \
                           secondary_pattern, setting, exception);

#define kNumSources     \
  kRemoteList:          \
  return "remote-list"; \
  case SiteSettingSource::kNumSources

namespace {
// Forward declaration.
void LuxxleGetExceptionForPage(ContentSettingsType type,
                              Profile* profile,
                              bool incognito,
                              const ContentSettingsPattern& pattern,
                              const ContentSettingsPattern& secondary_pattern,
                              const ContentSetting& setting,
                              base::Value::Dict& exception);
}  // namespace

#include "src/chrome/browser/ui/webui/settings/site_settings_helper.cc"

#undef kNumSources
#undef LUXXLE_PROVIDER_TYPE_TO_SITE_SETTINGS_SOURCE
#undef LUXXLE_PROVIDER_TO_DEFAULT_SETTINGS_STRING
#undef LUXXLE_CONTENT_SETTINGS_TYPE_GROUP_NAMES_LIST
#undef LUXXLE_SITE_SETTINGS_HELPER_CONTENT_SETTINGS_TYPE_FROM_GROUP_NAME
#undef LUXXLE_SITE_SETTINGS_HELPER_CONTENT_SETTINGS_TYPE_TO_GROUP_NAME
#undef GetVisiblePermissionCategories
#undef HasRegisteredGroupName
#undef LUXXLE_GET_EXCEPTION_FOR_PAGE

namespace site_settings {

bool HasRegisteredGroupName(ContentSettingsType type) {
  if (type == ContentSettingsType::AUTOPLAY) {
    return true;
  }
  if (type == ContentSettingsType::LUXXLE_GOOGLE_SIGN_IN) {
    return true;
  }
  if (type == ContentSettingsType::LUXXLE_LOCALHOST_ACCESS) {
    return true;
  }
  if (type == ContentSettingsType::LUXXLE_ETHEREUM) {
    return true;
  }
  if (type == ContentSettingsType::LUXXLE_SOLANA) {
    return true;
  }
  if (type == ContentSettingsType::LUXXLE_CARDANO) {
    return true;
  }
  if (type == ContentSettingsType::LUXXLE_SHIELDS) {
    return true;
  }
  return HasRegisteredGroupName_ChromiumImpl(type);
}

std::vector<ContentSettingsType> GetVisiblePermissionCategories(
    const std::string& origin,
    Profile* profile) {
  static constexpr ContentSettingsType extra_types[] = {
      ContentSettingsType::AUTOPLAY,
      ContentSettingsType::LUXXLE_ETHEREUM,
      ContentSettingsType::LUXXLE_SOLANA,
      ContentSettingsType::LUXXLE_GOOGLE_SIGN_IN,
      ContentSettingsType::LUXXLE_LOCALHOST_ACCESS,
      ContentSettingsType::LUXXLE_OPEN_AI_CHAT,
      // TODO(cypt4): Enable ContentSettingsType::LUXXLE_CARDANO,
  };

  auto types = GetVisiblePermissionCategories_ChromiumImpl(origin, profile);

  types.insert(std::end(types), std::begin(extra_types), std::end(extra_types));
  return types;
}
}  // namespace site_settings

namespace {
void LuxxleGetExceptionForPage(ContentSettingsType type,
                              Profile* profile,
                              bool incognito,
                              const ContentSettingsPattern& pattern,
                              const ContentSettingsPattern& secondary_pattern,
                              const ContentSetting& setting,
                              base::Value::Dict& exception) {
  constexpr char kLuxxleCookieType[] = "luxxleCookieType";

  // Update the RawSiteException.luxxleCookieType declaration in
  // site_settings_prefs_browser_proxy.ts if you want to change or add values.
  constexpr char kShieldsDown[] = "shields down";
  constexpr char kShieldsSettings[] = "shields settings";
  constexpr char kGoogleSignIn[] = "google sign-in";

  if (type == ContentSettingsType::COOKIES) {
    auto* map = HostContentSettingsMapFactory::GetForProfile(profile);
    auto* provider = static_cast<content_settings::LuxxlePrefProvider*>(
        map->GetPrefProvider());
    switch (provider->GetCookieType(pattern, secondary_pattern, setting,
                                    incognito)) {
      case content_settings::LuxxlePrefProvider::CookieType::kRegularCookie:
        break;
      case content_settings::LuxxlePrefProvider::CookieType::kShieldsDownCookie:
        exception.Set(kLuxxleCookieType, kShieldsDown);
        break;
      case content_settings::LuxxlePrefProvider::CookieType::
          kCustomShieldsCookie:
        exception.Set(kLuxxleCookieType, kShieldsSettings);
        break;
      case content_settings::LuxxlePrefProvider::CookieType::kGoogleSignInCookie:
        exception.Set(kLuxxleCookieType, kGoogleSignIn);
        break;
    }
  }
}

}  // namespace
