/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_wallet/wallet_page_ui.h"

#include <string>
#include <utility>

#include "base/command_line.h"
#include "base/files/file_path.h"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
#include "luxxle/browser/ui/webui/luxxle_wallet/wallet_common_ui.h"
#include "luxxle/browser/ui/webui/navigation_bar_data_provider.h"
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
#include "luxxle/components/luxxle_wallet_page/resources/grit/luxxle_wallet_page_generated_map.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
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

WalletPageUI::WalletPageUI(content::WebUI* web_ui)
    : ui::MojoWebUIController(web_ui,
                              true /* Needed for webui browser tests */) {
  auto* profile = Profile::FromWebUI(web_ui);
  content::WebUIDataSource* source =
      content::WebUIDataSource::CreateAndAdd(profile, kWalletPageHost);
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
  NavigationBarDataProvider::Initialize(source, profile);
  webui::SetupWebUIDataSource(source, base::span(kLuxxleWalletPageGenerated),
                              IDR_WALLET_PAGE_HTML);
  source->AddString("luxxleWalletLedgerBridgeUrl", kUntrustedLedgerURL);
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ImgSrc,
      "img-src 'self' data: chrome://resources chrome://erc-token-images "
      "chrome://image;");
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FrameSrc,
      std::string("frame-src ") + kUntrustedTrezorURL + " " +
          kUntrustedLedgerURL + " " + kUntrustedNftURL + " " +
          kUntrustedLineChartURL + " " + kUntrustedMarketURL + ";");
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
}

WalletPageUI::~WalletPageUI() = default;
WEB_UI_CONTROLLER_TYPE_IMPL(WalletPageUI)

void WalletPageUI::BindInterface(
    mojo::PendingReceiver<luxxle_wallet::mojom::PageHandlerFactory> receiver) {
  page_factory_receiver_.reset();
  page_factory_receiver_.Bind(std::move(receiver));
}

void WalletPageUI::CreatePageHandler(
    mojo::PendingReceiver<luxxle_wallet::mojom::PageHandler> page_receiver,
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
        ipfs_service_receiver,
    mojo::PendingReceiver<luxxle_wallet::mojom::MeldIntegrationService>
        meld_integration_service) {
  auto* profile = Profile::FromWebUI(web_ui());
  DCHECK(profile);

  page_handler_ =
      std::make_unique<WalletPageHandler>(std::move(page_receiver), profile);
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
  luxxle_wallet::AssetRatioServiceFactory::BindForContext(
      profile, std::move(asset_ratio_service_receiver));
  luxxle_wallet::MeldIntegrationServiceFactory::BindForContext(
      profile, std::move(meld_integration_service));
  luxxle_wallet::LuxxleWalletIpfsServiceFactory::BindForContext(
      profile, std::move(ipfs_service_receiver));

  auto* blockchain_registry = luxxle_wallet::BlockchainRegistry::GetInstance();
  if (blockchain_registry) {
    blockchain_registry->Bind(std::move(blockchain_registry_receiver));
  }
}
