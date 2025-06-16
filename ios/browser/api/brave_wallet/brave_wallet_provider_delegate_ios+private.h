/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_BRAVE_WALLET_BRAVE_WALLET_PROVIDER_DELEGATE_IOS_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_BRAVE_WALLET_BRAVE_WALLET_PROVIDER_DELEGATE_IOS_PRIVATE_H_

#include <optional>
#include <string>
#include <vector>

// REMOVED: #include "luxxle/components/brave_wallet/.*"

@protocol BraveWalletProviderDelegate;

namespace brave_wallet {

class BraveWalletProviderDelegateBridge
    : public brave_wallet::BraveWalletProviderDelegate {
 public:
  explicit BraveWalletProviderDelegateBridge(
      id<BraveWalletProviderDelegate> bridge)
      : bridge_(bridge) {}

 private:
  __weak id<BraveWalletProviderDelegate> bridge_;

  bool IsTabVisible() override;
  void ShowPanel() override;
  void WalletInteractionDetected() override;
  url::Origin GetOrigin() const override;
  void ShowWalletOnboarding() override;
  void ShowWalletBackup() override;
  void UnlockWallet() override;
  void ShowAccountCreation(mojom::CoinType type) override;
  void RequestPermissions(mojom::CoinType type,
                          const std::vector<std::string>& accounts,
                          RequestPermissionsCallback) override;
  bool IsAccountAllowed(mojom::CoinType type,
                        const std::string& account) override;
  std::optional<std::vector<std::string>> GetAllowedAccounts(
      mojom::CoinType type,
      const std::vector<std::string>& accounts) override;
  bool IsPermissionDenied(mojom::CoinType type) override;
  void AddSolanaConnectedAccount(const std::string& account) override;
  void RemoveSolanaConnectedAccount(const std::string& account) override;
  bool IsSolanaAccountConnected(const std::string& account) override;
};

}  // namespace brave_wallet

#endif  // LUXXLE_IOS_BROWSER_API_BRAVE_WALLET_BRAVE_WALLET_PROVIDER_DELEGATE_IOS_PRIVATE_H_
