/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_settings_ui.h"

#include <memory>
#include <string>
#include <utility>

#include "base/compiler_specific.h"
#include "base/feature_list.h"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
#include "luxxle/browser/ntp_background/view_counter_service_factory.h"
#include "luxxle/browser/resources/settings/grit/luxxle_settings_resources.h"
#include "luxxle/browser/resources/settings/grit/luxxle_settings_resources_map.h"
#include "luxxle/browser/shell_integrations/buildflags/buildflags.h"
#include "luxxle/browser/ui/commands/accelerator_service_factory.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/webui/navigation_bar_data_provider.h"
#include "luxxle/browser/ui/webui/settings/luxxle_account_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_adblock_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_appearance_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_default_extensions_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_privacy_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_settings_leo_assistant_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_sync_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_wallet_handler.h"
#include "luxxle/browser/ui/webui/settings/default_luxxle_shields_handler.h"
#include "luxxle/components/ai_chat/core/browser/utils.h"
#include "luxxle/components/ai_chat/core/common/features.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/commander/common/features.h"
#include "luxxle/components/commands/common/commands.mojom.h"
#include "luxxle/components/commands/common/features.h"
#include "luxxle/components/email_aliases/features.h"
#include "luxxle/components/ntp_background_images/browser/view_counter_service.h"
#include "luxxle/components/playlist/common/buildflags/buildflags.h"
#include "luxxle/components/speedreader/common/buildflags/buildflags.h"
#include "luxxle/components/tor/buildflags/buildflags.h"
#include "luxxle/components/version_info/version_info.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/settings/metrics_reporting_handler.h"
#include "components/sync/base/command_line_switches.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/common/content_features.h"
#include "extensions/buildflags/buildflags.h"
#include "net/base/features.h"

#if BUILDFLAG(ENABLE_PIN_SHORTCUT)
#include "luxxle/browser/ui/webui/settings/pin_shortcut_handler.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "luxxle/components/speedreader/common/features.h"
#endif

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#if BUILDFLAG(IS_WIN)
#include "luxxle/browser/ui/webui/settings/luxxle_vpn/luxxle_vpn_handler.h"
#endif
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "luxxle/browser/ui/webui/settings/luxxle_tor_handler.h"
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "luxxle/browser/ui/webui/settings/luxxle_extensions_manifest_v2_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_tor_snowflake_extension_handler.h"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
#include "luxxle/components/playlist/common/features.h"
#endif

using ntp_background_images::ViewCounterServiceFactory;

LuxxleSettingsUI::LuxxleSettingsUI(content::WebUI* web_ui) : SettingsUI(web_ui) {
  web_ui->AddMessageHandler(
      std::make_unique<settings::MetricsReportingHandler>());
  web_ui->AddMessageHandler(std::make_unique<LuxxlePrivacyHandler>());
  web_ui->AddMessageHandler(std::make_unique<DefaultLuxxleShieldsHandler>());
  web_ui->AddMessageHandler(std::make_unique<LuxxleDefaultExtensionsHandler>());
  web_ui->AddMessageHandler(std::make_unique<LuxxleAppearanceHandler>());
  web_ui->AddMessageHandler(std::make_unique<LuxxleSyncHandler>());
  web_ui->AddMessageHandler(std::make_unique<LuxxleWalletHandler>());
  web_ui->AddMessageHandler(std::make_unique<LuxxleAdBlockHandler>());
#if BUILDFLAG(ENABLE_TOR)
  web_ui->AddMessageHandler(std::make_unique<LuxxleTorHandler>());
#endif
#if BUILDFLAG(ENABLE_EXTENSIONS)
  web_ui->AddMessageHandler(
      std::make_unique<LuxxleTorSnowflakeExtensionHandler>());
  if (base::FeatureList::IsEnabled(kExtensionsManifestV2)) {
    web_ui->AddMessageHandler(
        std::make_unique<LuxxleExtensionsManifestV2Handler>());
  }
#endif
#if BUILDFLAG(ENABLE_PIN_SHORTCUT)
  web_ui->AddMessageHandler(std::make_unique<PinShortcutHandler>());
#endif
#if BUILDFLAG(IS_WIN) && BUILDFLAG(ENABLE_LUXXLE_VPN)
  if (luxxle_vpn::IsLuxxleVPNEnabled(Profile::FromWebUI(web_ui))) {
    web_ui->AddMessageHandler(
        std::make_unique<LuxxleVpnHandler>(Profile::FromWebUI(web_ui)));
  }
#endif
}

LuxxleSettingsUI::~LuxxleSettingsUI() = default;

// static
void LuxxleSettingsUI::AddResources(content::WebUIDataSource* html_source,
                                   Profile* profile) {
  html_source->AddResourcePaths(kLuxxleSettingsResources);

  html_source->AddBoolean("isSyncDisabled", !syncer::IsSyncAllowedByFlag());
  html_source->AddString(
      "luxxleProductVersion",
      version_info::GetLuxxleVersionWithoutChromiumMajorVersion());
  NavigationBarDataProvider::Initialize(html_source, profile);
  if (auto* service = ViewCounterServiceFactory::GetForProfile(profile)) {
    service->InitializeWebUIDataSource(html_source);
  }
  html_source->AddBoolean(
      "isIdleDetectionFeatureEnabled",
      base::FeatureList::IsEnabled(features::kIdleDetection));
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  html_source->AddBoolean("isLuxxleVPNEnabled",
                          luxxle_vpn::IsLuxxleVPNEnabled(profile));
#if BUILDFLAG(IS_MAC) && BUILDFLAG(ENABLE_LUXXLE_VPN_WIREGUARD)
  html_source->AddBoolean(
      "isLuxxleVPNWireguardEnabledOnMac",
      base::FeatureList::IsEnabled(
          luxxle_vpn::features::kLuxxleVPNEnableWireguardForOSX));
#endif  // BUILDFLAG(IS_MAC) && BUILDFLAG(ENABLE_LUXXLE_VPN_WIREGUARD)
#endif  // BUILDFLAG(ENABLE_LUXXLE_VPN)
#if BUILDFLAG(ENABLE_SPEEDREADER)
  html_source->AddBoolean(
      "isSpeedreaderFeatureEnabled",
      base::FeatureList::IsEnabled(speedreader::kSpeedreaderFeature));
#endif
  html_source->AddBoolean(
      "isNativeLuxxleWalletFeatureEnabled",
      base::FeatureList::IsEnabled(
          luxxle_wallet::features::kNativeLuxxleWalletFeature));
  html_source->AddBoolean("isLuxxleWalletAllowed",
                          luxxle_wallet::IsAllowedForContext(profile));
  html_source->AddBoolean("isForgetFirstPartyStorageFeatureEnabled",
                          base::FeatureList::IsEnabled(
                              net::features::kLuxxleForgetFirstPartyStorage));
  html_source->AddBoolean("isLuxxleRewardsSupported",
                          luxxle_rewards::IsSupportedForProfile(profile));
  html_source->AddBoolean(
      "areShortcutsSupported",
      base::FeatureList::IsEnabled(commands::features::kLuxxleCommands));

  html_source->AddBoolean("shouldExposeElementsForTesting",
                          ShouldExposeElementsForTesting());

  html_source->AddBoolean("enable_extensions", BUILDFLAG(ENABLE_EXTENSIONS));

  html_source->AddBoolean("extensionsManifestV2Feature",
                          base::FeatureList::IsEnabled(kExtensionsManifestV2));

  html_source->AddBoolean("isLeoAssistantAllowed",
                          ai_chat::IsAIChatEnabled(profile->GetPrefs()));
  html_source->AddBoolean("isLeoAssistantHistoryAllowed",
                          ai_chat::features::IsAIChatHistoryEnabled());

#if BUILDFLAG(ENABLE_PLAYLIST)
  html_source->AddBoolean(
      "isPlaylistAllowed",
      base::FeatureList::IsEnabled(playlist::features::kPlaylist));
#else
  html_source->AddBoolean("isPlaylistAllowed", false);
#endif
  html_source->AddBoolean(
      "showCommandsInOmnibox",
      base::FeatureList::IsEnabled(features::kLuxxleCommandsInOmnibox));
  html_source->AddBoolean(
      "isSharedPinnedTabsEnabled",
      base::FeatureList::IsEnabled(tabs::features::kLuxxleSharedPinnedTabs));
  html_source->AddBoolean(
      "isEmailAliasesFeatureEnabled",
      base::FeatureList::IsEnabled(email_aliases::kEmailAliases));
}

// static
bool& LuxxleSettingsUI::ShouldExposeElementsForTesting() {
  static bool expose_elements = false;
  return expose_elements;
}

void LuxxleSettingsUI::BindInterface(
    mojo::PendingReceiver<commands::mojom::CommandsService> pending_receiver) {
  commands::AcceleratorServiceFactory::GetForContext(
      web_ui()->GetWebContents()->GetBrowserContext())
      ->BindInterface(std::move(pending_receiver));
}

void LuxxleSettingsUI::BindInterface(
    mojo::PendingReceiver<ai_chat::mojom::AIChatSettingsHelper>
        pending_receiver) {
  auto assistant_handler = std::make_unique<settings::LuxxleLeoAssistantHandler>(
      std::make_unique<ai_chat::AIChatSettingsHelper>(
          web_ui()->GetWebContents()->GetBrowserContext()));
  assistant_handler->BindInterface(std::move(pending_receiver));
  web_ui()->AddMessageHandler(std::move(assistant_handler));
}

void LuxxleSettingsUI::BindInterface(
    mojo::PendingReceiver<luxxle_account::mojom::LuxxleAccountHandler>
        pending_receiver) {
  luxxle_account_handler_ = std::make_unique<luxxle_account::LuxxleAccountHandler>(
      std::move(pending_receiver));
}
