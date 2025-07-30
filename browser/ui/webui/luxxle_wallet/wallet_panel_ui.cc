/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_wallet/wallet_panel_ui.h"

#include <string>
#include <utility>

#include "base/command_line.h"
#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
#include "luxxle/browser/ui/webui/luxxle_wallet/wallet_common_ui.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/luxxle_wallet_panel/resources/grit/luxxle_wallet_panel_generated_map.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/webui/plural_string_handler.h"
#include "chrome/browser/ui/webui/sanitized_image_source.h"
#include "components/grit/luxxle_components_resources.h"
#include "components/grit/luxxle_components_strings.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/common/url_constants.h"
#include "ui/base/accelerators/accelerator.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/webui/web_ui_util.h"
#include "ui/webui/webui_util.h"

WalletPanelUI::WalletPanelUI(content::WebUI* web_ui)
    : TopChromeWebUIController(web_ui,
                               true /* Needed for webui browser tests */) {
  auto* profile = Profile::FromWebUI(web_ui);
  content::WebUIDataSource* source =
      content::WebUIDataSource::CreateAndAdd(profile, kWalletPanelHost);
  web_ui->AddRequestableScheme(content::kChromeUIUntrustedScheme);
  for (const auto& str : luxxle_wallet::kLocalizedStrings) {
    std::u16string l10n_str = l10n_util::GetStringUTF16(str.id);
    source->AddString(str.name, l10n_str);
  }
  auto plural_string_handler = std::make_unique<PluralStringHandler>();
  plural_string_handler->AddLocalizedString(
      "luxxleWalletExchangeNamePlusSteps",
      IDS_LUXXLE_WALLET_EXCHANGE_NAME_PLUS_STEPS);
  plural_string_handler->AddLocalizedString(
      "luxxleWalletPendingTransactions", IDS_LUXXLE_WALLET_PENDING_TRANSACTIONS);
  web_ui->AddMessageHandler(std::move(plural_string_handler));
  webui::SetupWebUIDataSource(source, base::span(kLuxxleWalletPanelGenerated),
                              IDR_WALLET_PANEL_HTML);
  source->AddString("luxxleWalletLedgerBridgeUrl", kUntrustedLedgerURL);
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FrameSrc,
      base::JoinString({"frame-src", kUntrustedTrezorURL, kUntrustedLedgerURL,
                        kUntrustedLineChartURL, kUntrustedNftURL,
                        base::StrCat({kUntrustedMarketURL, ";"})},
                       " "));
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ImgSrc,
      base::JoinString(
          {"img-src", "'self'", "chrome://resources",
           "chrome://erc-token-images", "chrome://favicon2", "chrome://image",
           "https://assets.cgproxy.luxxle.com", base::StrCat({"data:", ";"})},
          " "));
  source->AddString("luxxleWalletTrezorBridgeUrl", kUntrustedTrezorURL);
  source->AddString("luxxleWalletNftBridgeUrl", kUntrustedNftURL);
  source->AddString("luxxleWalletLineChartBridgeUrl", kUntrustedLineChartURL);
  source->AddString("luxxleWalletMarketUiBridgeUrl", kUntrustedMarketURL);
  source->AddBoolean("isAndroid", false);
  source->AddBoolean(luxxle_wallet::mojom::kP3ACountTestNetworksLoadTimeKey,
                     base::CommandLine::ForCurrentProcess()->HasSwitch(
                         luxxle_wallet::mojom::kP3ACountTestNetworksSwitch));
  content::URLDataSource::Add(profile,
                              std::make_unique<SanitizedImageSource>(profile));
  luxxle_wallet::AddBlockchainTokenImageSource(profile);
  active_web_contents_ = luxxle_wallet::GetActiveWebContents();
}

WalletPanelUI::~WalletPanelUI() = default;

WEB_UI_CONTROLLER_TYPE_IMPL(WalletPanelUI)

void WalletPanelUI::BindInterface(
    mojo::PendingReceiver<luxxle_wallet::mojom::PanelHandlerFactory> receiver) {
  panel_factory_receiver_.reset();
  panel_factory_receiver_.Bind(std::move(receiver));
}

void WalletPanelUI::SetDeactivationCallback(
    base::RepeatingCallback<void(bool)> deactivation_callback) {
  deactivation_callback_ = std::move(deactivation_callback);
}

void WalletPanelUI::CreatePanelHandler(
    mojo::PendingReceiver<luxxle_wallet::mojom::PanelHandler> panel_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::WalletHandler> wallet_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::JsonRpcService>
        json_rpc_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::BitcoinWalletService>
        bitcoin_wallet_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::ZCashWalletService>
        zcash_wallet_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::CardanoWalletService>
        cardano_wallet_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::SwapService>
        swap_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::SimulationService>
        simulation_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::AssetRatioService>
        asset_ratio_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::KeyringService>
        keyring_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::BlockchainRegistry>
        blockchain_registry_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::TxService> tx_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::EthTxManagerProxy>
        eth_tx_manager_proxy_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::SolanaTxManagerProxy>
        solana_tx_manager_proxy_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::FilTxManagerProxy>
        filecoin_tx_manager_proxy_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::BtcTxManagerProxy>
        bitcoin_tx_manager_proxy_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::LuxxleWalletService>
        luxxle_wallet_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::LuxxleWalletP3A>
        luxxle_wallet_p3a_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::IpfsService>
        luxxle_wallet_ipfs_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::MeldIntegrationService>
        meld_integration_service) {
  auto* profile = Profile::FromWebUI(web_ui());
  DCHECK(profile);

  panel_handler_ = std::make_unique<WalletPanelHandler>(
      std::move(panel_receiver), this, active_web_contents_,
      std::move(deactivation_callback_));
  wallet_handler_ = std::make_unique<luxxle_wallet::WalletHandler>(
      std::move(wallet_receiver), profile);

  if (auto* wallet_service =
          luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForContext(
              profile)) {
    wallet_service->Bind(std::move(luxxle_wallet_service_receiver));
    wallet_service->Bind(std::move(json_rpc_service_receiver));
    wallet_service->Bind(std::move(bitcoin_wallet_service_receiver));
    wallet_service->Bind(std::move(zcash_wallet_service_receiver));
    wallet_service->Bind(std::move(cardano_wallet_service_receiver));
    wallet_service->Bind(std::move(keyring_service_receiver));
    wallet_service->Bind(std::move(tx_service_receiver));
    wallet_service->Bind(std::move(eth_tx_manager_proxy_receiver));
    wallet_service->Bind(std::move(solana_tx_manager_proxy_receiver));
    wallet_service->Bind(std::move(filecoin_tx_manager_proxy_receiver));
    wallet_service->Bind(std::move(bitcoin_tx_manager_proxy_receiver));
    wallet_service->Bind(std::move(luxxle_wallet_p3a_receiver));
  }

  luxxle_wallet::SwapServiceFactory::BindForContext(
      profile, std::move(swap_service_receiver));
  luxxle_wallet::SimulationServiceFactory::BindForContext(
      profile, std::move(simulation_service_receiver));
  luxxle_wallet::AssetRatioServiceFactory::BindForContext(
      profile, std::move(asset_ratio_service_receiver));
  luxxle_wallet::MeldIntegrationServiceFactory::BindForContext(
      profile, std::move(meld_integration_service));
  luxxle_wallet::LuxxleWalletIpfsServiceFactory::BindForContext(
      profile, std::move(luxxle_wallet_ipfs_service_receiver));

  auto* blockchain_registry = luxxle_wallet::BlockchainRegistry::GetInstance();
  if (blockchain_registry) {
    blockchain_registry->Bind(std::move(blockchain_registry_receiver));
  }
}

WalletPanelUIConfig::WalletPanelUIConfig()
    : DefaultTopChromeWebUIConfig(content::kChromeUIScheme, kWalletPanelHost) {}

bool WalletPanelUIConfig::IsWebUIEnabled(
    content::BrowserContext* browser_context) {
  return luxxle_wallet::IsAllowedForContext(browser_context);
}

bool WalletPanelUIConfig::ShouldAutoResizeHost() {
  return true;
}
