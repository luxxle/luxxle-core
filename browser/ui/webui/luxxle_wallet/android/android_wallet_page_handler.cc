// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/luxxle_wallet/android/android_wallet_page_handler.h"

#include <utility>

// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"

AndroidWalletPageHandler::AndroidWalletPageHandler(
    mojo::PendingReceiver<luxxle_wallet::mojom::PageHandler> receiver,
    Profile* profile,
    ui::MojoWebUIController* webui_controller)
    : WalletPageHandler(std::move(receiver), profile),
      webui_controller_(webui_controller) {
  DCHECK(webui_controller_);
}
AndroidWalletPageHandler::~AndroidWalletPageHandler() = default;

void AndroidWalletPageHandler::ShowApprovePanelUI() {
  if (!webui_controller_) {
    return;
  }

  ::luxxle_wallet::ShowPanel(webui_controller_->web_ui()->GetWebContents());
}

void AndroidWalletPageHandler::ShowWalletBackupUI() {
  ::luxxle_wallet::ShowWalletBackup();
}

void AndroidWalletPageHandler::UnlockWalletUI() {
  ::luxxle_wallet::UnlockWallet();
}
