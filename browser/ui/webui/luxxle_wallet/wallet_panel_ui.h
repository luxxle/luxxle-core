/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_WALLET_PANEL_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_WALLET_PANEL_UI_H_

#include <memory>
#include <string>

#include "base/memory/raw_ptr.h"
#include "luxxle/browser/ui/webui/luxxle_wallet/common_handler/wallet_handler.h"
#include "luxxle/browser/ui/webui/luxxle_wallet/panel_handler/wallet_panel_handler.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "chrome/browser/ui/webui/top_chrome/top_chrome_web_ui_controller.h"
#include "chrome/browser/ui/webui/top_chrome/top_chrome_webui_config.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace content {
class BrowserContext;
}

class WalletPanelUI : public TopChromeWebUIController,
                      public luxxle_wallet::mojom::PanelHandlerFactory {
 public:
  explicit WalletPanelUI(content::WebUI* web_ui);
  WalletPanelUI(const WalletPanelUI&) = delete;
  WalletPanelUI& operator=(const WalletPanelUI&) = delete;
  ~WalletPanelUI() override;

  // Instantiates the implementor of the mojom::PanelHandlerFactory mojo
  // interface passing the pending receiver that will be internally bound.
  void BindInterface(
      mojo::PendingReceiver<luxxle_wallet::mojom::PanelHandlerFactory> receiver);
  // The bubble disappears by default when Trezor opens a popup window
  // from the wallet panel bubble. In order to prevent it we set a callback
  // to modify panel deactivation flag when necessary.
  void SetDeactivationCallback(
      base::RepeatingCallback<void(bool)> deactivation_callback);

  static constexpr std::string GetWebUIName() { return "WalletPanel"; }

 private:
  // luxxle_wallet::mojom::PanelHandlerFactory:
  void CreatePanelHandler(
      mojo::PendingReceiver<luxxle_wallet::mojom::PanelHandler> panel_receiver,
      mojo::PendingReceiver<luxxle_wallet::mojom::WalletHandler> wallet_receiver,
      mojo::PendingReceiver<luxxle_wallet::mojom::JsonRpcService>
          json_rpc_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::BitcoinWalletService>
          bitcoin_rpc_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::ZCashWalletService>
          zcash_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::CardanoWalletService>
          cardano_wallet_service_receiver,
      mojo::PendingReceiver<luxxle_wallet::mojom::SwapService> swap_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::SimulationService>
          simulation_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::AssetRatioService>
          asset_ratio_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::KeyringService>
          keyring_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::BlockchainRegistry>
          blockchain_registry,
      mojo::PendingReceiver<luxxle_wallet::mojom::TxService> tx_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::EthTxManagerProxy>
          eth_tx_manager_proxy,
      mojo::PendingReceiver<luxxle_wallet::mojom::SolanaTxManagerProxy>
          solana_tx_manager_proxy,
      mojo::PendingReceiver<luxxle_wallet::mojom::FilTxManagerProxy>
          fil_tx_manager_proxy,
      mojo::PendingReceiver<luxxle_wallet::mojom::BtcTxManagerProxy>
          bitcoin_tx_manager_proxy_receiver,
      mojo::PendingReceiver<luxxle_wallet::mojom::LuxxleWalletService>
          luxxle_wallet_service,
      mojo::PendingReceiver<luxxle_wallet::mojom::LuxxleWalletP3A>
          luxxle_wallet_p3a,
      mojo::PendingReceiver<luxxle_wallet::mojom::IpfsService>
          luxxle_wallet_ipfs_service_receiver,
      mojo::PendingReceiver<luxxle_wallet::mojom::MeldIntegrationService>
          meld_integration_service) override;

  std::unique_ptr<WalletPanelHandler> panel_handler_;
  std::unique_ptr<luxxle_wallet::WalletHandler> wallet_handler_;
  raw_ptr<content::WebContents> active_web_contents_ = nullptr;

  base::RepeatingCallback<void(bool)> deactivation_callback_;
  mojo::Receiver<luxxle_wallet::mojom::PanelHandlerFactory>
      panel_factory_receiver_{this};

  WEB_UI_CONTROLLER_TYPE_DECL();
};

class WalletPanelUIConfig : public DefaultTopChromeWebUIConfig<WalletPanelUI> {
 public:
  WalletPanelUIConfig();

  // WebUIConfig::
  bool IsWebUIEnabled(content::BrowserContext* browser_context) override;

  // TopChromeWebUIConfig::
  bool ShouldAutoResizeHost() override;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_WALLET_PANEL_UI_H_
