// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_ANDROID_ANDROID_WALLET_PAGE_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_ANDROID_ANDROID_WALLET_PAGE_HANDLER_H_

#include "base/memory/raw_ptr.h"
#include "luxxle/browser/ui/webui/luxxle_wallet/page_handler/wallet_page_handler.h"

// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "ui/webui/mojo_web_ui_controller.h"

class AndroidWalletPageHandler : WalletPageHandler {
 public:
  AndroidWalletPageHandler(
      mojo::PendingReceiver<luxxle_wallet::mojom::PageHandler> receiver,
      Profile* profile,
      ui::MojoWebUIController* webui_controller);

  AndroidWalletPageHandler(const AndroidWalletPageHandler&) = delete;
  AndroidWalletPageHandler& operator=(const AndroidWalletPageHandler&) = delete;
  ~AndroidWalletPageHandler() override;

  void ShowApprovePanelUI() override;
  void ShowWalletBackupUI() override;
  void UnlockWalletUI() override;

 private:
  raw_ptr<ui::MojoWebUIController> const webui_controller_;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_WALLET_ANDROID_ANDROID_WALLET_PAGE_HANDLER_H_
