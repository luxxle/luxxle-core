/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/webui/chrome_web_ui_configs.h"

#include "luxxle/browser/ui/webui/ai_chat/ai_chat_ui.h"
#include "luxxle/components/ai_chat/core/common/features.h"
#include "luxxle/components/luxxle_education/buildflags.h"
#include "content/public/browser/webui_config_map.h"

#define RegisterChromeWebUIConfigs RegisterChromeWebUIConfigs_ChromiumImpl

#include "src/chrome/browser/ui/webui/chrome_web_ui_configs.cc"
#undef RegisterChromeWebUIConfigs

#if !BUILDFLAG(IS_ANDROID)
#include "luxxle/browser/ui/webui/luxxle_rewards/rewards_page_top_ui.h"
#include "luxxle/browser/ui/webui/luxxle_settings_ui.h"
#include "luxxle/browser/ui/webui/luxxle_shields/cookie_list_opt_in_ui.h"
#include "luxxle/browser/ui/webui/luxxle_shields/shields_panel_ui.h"
#include "luxxle/browser/ui/webui/luxxle_wallet/wallet_panel_ui.h"
#include "luxxle/browser/ui/webui/private_new_tab_page/luxxle_private_new_tab_ui.h"
#include "luxxle/browser/ui/webui/speedreader/speedreader_toolbar_ui.h"
#include "luxxle/browser/ui/webui/webcompat_reporter/webcompat_reporter_ui.h"
#else  // !BUILDFLAG(IS_ANDROID)
#include "luxxle/browser/ui/webui/new_tab_takeover/android/new_tab_takeover_ui_config.h"
#endif  // !BUILDFLAG(IS_ANDROID)

#include "luxxle/browser/ui/webui/luxxle_adblock_internals_ui.h"
#include "luxxle/browser/ui/webui/luxxle_adblock_ui.h"

#if BUILDFLAG(ENABLE_LUXXLE_EDUCATION)
#include "luxxle/browser/ui/webui/luxxle_education/luxxle_education_page_ui.h"
#endif

namespace {

#if !BUILDFLAG(IS_ANDROID)
const GURL GetWebUIConfigURL(const char* scheme, const char* host) {
  return GURL(base::StrCat({scheme, url::kStandardSchemeSeparator, host}));
}
#endif  // !BUILDFLAG(IS_ANDROID)

void RemoveOverridenWebUIs(content::WebUIConfigMap& map) {
#if !BUILDFLAG(IS_ANDROID)
  // Remove NewTabUIConfig. It will be replaced with LuxxlePrivateNewTabUIConfig.
  // Note that this only handles new tab for private profiles (Private, Tor,
  // Guest). For regular profile the handling is still done in
  // LuxxleWebUIControllerFactory. We will need to follow up on transitioning
  // LuxxleNewTabUI to using WebUIConfig. Currently, we can't add both
  // LuxxlePrivateNewTabUI and LuxxleNewTabUI configs to the map because they
  // use the same origin (content::kChromeUIScheme +
  // chrome::kChromeUINewTabHost).
  map.RemoveConfig(
      GetWebUIConfigURL(content::kChromeUIScheme, chrome::kChromeUINewTabHost));
  // Remove SettingsUIConfig. It will be replaced with LuxxleSettingsUIConfig.
  map.RemoveConfig(GetWebUIConfigURL(content::kChromeUIScheme,
                                     chrome::kChromeUISettingsHost));
#endif  // !BUILDFLAG(IS_ANDROID)
}

}  // namespace

void RegisterChromeWebUIConfigs() {
  RegisterChromeWebUIConfigs_ChromiumImpl();

  auto& map = content::WebUIConfigMap::GetInstance();
  // Remove configs for Chromium WebUIs that we replace with our own WebUIs.
  // The map doesn't allow for multiple entries for the same origin, so the
  // upstream configs must be removed before we can add our own configs.
  RemoveOverridenWebUIs(map);

#if !BUILDFLAG(IS_ANDROID)
  map.AddWebUIConfig(std::make_unique<luxxle_rewards::RewardsPageTopUIConfig>());
  map.AddWebUIConfig(std::make_unique<LuxxlePrivateNewTabUIConfig>());
  map.AddWebUIConfig(std::make_unique<LuxxleSettingsUIConfig>());
  map.AddWebUIConfig(std::make_unique<CookieListOptInUIConfig>());
  map.AddWebUIConfig(std::make_unique<ShieldsPanelUIConfig>());
  map.AddWebUIConfig(std::make_unique<SpeedreaderToolbarUIConfig>());
  map.AddWebUIConfig(std::make_unique<WalletPanelUIConfig>());
  map.AddWebUIConfig(
      std::make_unique<webcompat_reporter::WebcompatReporterUIConfig>());
#else   // !BUILDFLAG(IS_ANDROID)
  map.AddWebUIConfig(std::make_unique<NewTabTakeoverUIConfig>());
#endif  // !BUILDFLAG(IS_ANDROID)
  map.AddWebUIConfig(std::make_unique<LuxxleAdblockUIConfig>());
  map.AddWebUIConfig(std::make_unique<LuxxleAdblockInternalsUIConfig>());

  if (ai_chat::features::IsAIChatEnabled()) {
    map.AddWebUIConfig(std::make_unique<AIChatUIConfig>());
  }

#if BUILDFLAG(ENABLE_LUXXLE_EDUCATION)
  map.AddWebUIConfig(std::make_unique<LuxxleEducationPageUIConfig>());
#endif
}
