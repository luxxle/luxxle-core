/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_WALLET_PAGE_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_WALLET_PAGE_UI_H_

#include <memory>

#include "luxxle/browser/ui/webui/luxxle_wallet/common_handler/wallet_handler.h"
#include "luxxle/browser/ui/webui/luxxle_wallet/page_handler/wallet_page_handler.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_web_ui_controller.h"

class WalletPageUI : public ui::MojoWebUIController,
                     public luxxle_wallet::mojom::PageHandlerFactory {
 public:
  explicit WalletPageUI(content::WebUI* web_ui);
  WalletPageUI(const WalletPageUI&) = delete;
  WalletPageUI& operator=(const WalletPageUI&) = delete;
  ~WalletPageUI() override;

  // Instantiates the implementor of the mojom::PageHandlerFactory mojo
  // interface passing the pending receiver that will be internally bound.
  void BindInterface(
      mojo::PendingReceiver<luxxle_wallet::mojom::PageHandlerFactory> receiver);

 private:
  // luxxle_wallet::mojom::PageHandlerFactory:
  void CreatePageHandler(
      mojo::PendingReceiver<luxxle_wallet::mojom::PageHandler> page_receiver,
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
          filecoin_tx_manager_proxy,
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

  std::unique_ptr<WalletPageHandler> page_handler_;
  std::unique_ptr<luxxle_wallet::WalletHandler> wallet_handler_;

  mojo::Receiver<luxxle_wallet::mojom::PageHandlerFactory>
      page_factory_receiver_{this};

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_WALLET_PAGE_UI_H_
