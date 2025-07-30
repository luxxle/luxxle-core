/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/luxxle_wallet/luxxle_wallet_provider_delegate_ios.h"

#include <optional>

#include "base/strings/sys_string_conversions.h"
#include "luxxle/base/mac/conversions.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/ios/browser/api/luxxle_wallet/luxxle_wallet_provider_delegate_ios+private.h"
#include "luxxle/ios/browser/api/url/url_origin_ios+private.h"
#include "net/base/apple/url_conversions.h"

namespace luxxle_wallet {

bool LuxxleWalletProviderDelegateBridge::IsTabVisible() {
  return [bridge_ isTabVisible];
}

void LuxxleWalletProviderDelegateBridge::ShowPanel() {
  [bridge_ showPanel];
}

void LuxxleWalletProviderDelegateBridge::WalletInteractionDetected() {
  [bridge_ walletInteractionDetected];
}

url::Origin LuxxleWalletProviderDelegateBridge::GetOrigin() const {
  return url::Origin([[bridge_ getOrigin] underlyingOrigin]);
}

void LuxxleWalletProviderDelegateBridge::ShowWalletOnboarding() {
  [bridge_ showWalletOnboarding];
}

void LuxxleWalletProviderDelegateBridge::ShowWalletBackup() {
  [bridge_ showWalletBackup];
}

void LuxxleWalletProviderDelegateBridge::UnlockWallet() {
  [bridge_ unlockWallet];
}

void LuxxleWalletProviderDelegateBridge::ShowAccountCreation(
    mojom::CoinType type) {
  [bridge_ showAccountCreation:static_cast<LuxxleWalletCoinType>(type)];
}

void LuxxleWalletProviderDelegateBridge::RequestPermissions(
    mojom::CoinType type,
    const std::vector<std::string>& accounts,
    RequestPermissionsCallback callback) {
  auto completion = [callback = std::make_shared<decltype(callback)>(std::move(
                         callback))](LuxxleWalletRequestPermissionsError error,
                                     NSArray<NSString*>* _Nullable results) {
    if (!callback) {
      return;
    }
    if (results == nil) {
      std::move(*callback).Run(
          static_cast<mojom::RequestPermissionsError>(error), std::nullopt);
      return;
    }
    std::vector<std::string> v;
    for (NSString* result in results) {
      v.push_back(base::SysNSStringToUTF8(result));
    }
    std::move(*callback).Run(static_cast<mojom::RequestPermissionsError>(error),
                             v);
  };
  [bridge_ requestPermissions:static_cast<LuxxleWalletCoinType>(type)
                     accounts:luxxle::vector_to_ns(accounts)
                   completion:completion];
}

bool LuxxleWalletProviderDelegateBridge::IsAccountAllowed(
    mojom::CoinType type,
    const std::string& account) {
  return [bridge_ isAccountAllowed:static_cast<LuxxleWalletCoinType>(type)
                           account:base::SysUTF8ToNSString(account)];
}

std::optional<std::vector<std::string>>
LuxxleWalletProviderDelegateBridge::GetAllowedAccounts(
    mojom::CoinType type,
    const std::vector<std::string>& accounts) {
  NSArray<NSString*>* results =
      [bridge_ getAllowedAccounts:static_cast<LuxxleWalletCoinType>(type)
                         accounts:luxxle::vector_to_ns(accounts)];
  if (!results) {
    return std::nullopt;
  }

  return luxxle::ns_to_vector<std::string>(results);
}

bool LuxxleWalletProviderDelegateBridge::IsPermissionDenied(
    mojom::CoinType type) {
  return [bridge_ isPermissionDenied:static_cast<LuxxleWalletCoinType>(type)];
}

void LuxxleWalletProviderDelegateBridge::AddSolanaConnectedAccount(
    const std::string& account) {
  [bridge_ addSolanaConnectedAccount:base::SysUTF8ToNSString(account)];
}

void LuxxleWalletProviderDelegateBridge::RemoveSolanaConnectedAccount(
    const std::string& account) {
  [bridge_ removeSolanaConnectedAccount:base::SysUTF8ToNSString(account)];
}

bool LuxxleWalletProviderDelegateBridge::IsSolanaAccountConnected(
    const std::string& account) {
  return [bridge_ isSolanaAccountConnected:base::SysUTF8ToNSString(account)];
}

}  // namespace luxxle_wallet
