// Copyright (c) 2021 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/brave_wallet/common_handler/wallet_handler.h"

#include <utility>

// REMOVED: #include "luxxle/browser/brave_wallet/.*"
// REMOVED: #include "luxxle/components/brave_wallet/.*"
// REMOVED: #include "luxxle/components/brave_wallet/.*"
// REMOVED: #include "luxxle/components/brave_wallet/.*"
// REMOVED: #include "luxxle/components/brave_wallet/.*"
#include "chrome/browser/profiles/profile.h"

namespace brave_wallet {

WalletHandler::WalletHandler(
    mojo::PendingReceiver<mojom::WalletHandler> receiver,
    Profile* profile)
    : receiver_(this, std::move(receiver)),
      brave_wallet_service_(
          BraveWalletServiceFactory::GetServiceForContext(profile)) {}

WalletHandler::~WalletHandler() = default;

// TODO(apaymyshev): this is the only method in WalletHandler. Should be merged
// into BraveWalletService.
void WalletHandler::GetWalletInfo(GetWalletInfoCallback callback) {
  if (!brave_wallet_service_) {
    std::move(callback).Run(nullptr);
    return;
  }

  auto* keyring_service = brave_wallet_service_->keyring_service();

  std::move(callback).Run(mojom::WalletInfo::New(
      keyring_service->IsWalletCreatedSync(), keyring_service->IsLockedSync(),
      keyring_service->IsWalletBackedUpSync(), IsBitcoinEnabled(),
      IsBitcoinImportEnabled(), IsBitcoinLedgerEnabled(), IsZCashEnabled(),
      IsAnkrBalancesEnabled(), IsTransactionSimulationsEnabled(),
      IsZCashShieldedTransactionsEnabled(), IsCardanoEnabled(),
      GetEnabledCoins()));
}

}  // namespace brave_wallet
