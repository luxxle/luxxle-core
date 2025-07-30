/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PERMISSION_MANAGER_FACTORY_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PERMISSION_MANAGER_FACTORY_H_

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace luxxle_wallet {
class EthereumProviderImplUnitTest;
class SolanaProviderImplUnitTest;
class LuxxleWalletServiceUnitTest;
}  // namespace luxxle_wallet

namespace permissions {
class LuxxleWalletPermissionContextUnitTest;
}

#define BuildServiceInstanceForBrowserContext               \
  BuildServiceInstanceForBrowserContext_ChromiumImpl(       \
      content::BrowserContext* profile) const;              \
  friend luxxle_wallet::EthereumProviderImplUnitTest;        \
  friend luxxle_wallet::SolanaProviderImplUnitTest;          \
  friend luxxle_wallet::LuxxleWalletServiceUnitTest;          \
  friend permissions::LuxxleWalletPermissionContextUnitTest; \
  std::unique_ptr<KeyedService> BuildServiceInstanceForBrowserContext

#include "src/chrome/browser/permissions/permission_manager_factory.h"  // IWYU pragma: export
#undef BuildServiceInstanceForBrowserContext

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_PERMISSIONS_PERMISSION_MANAGER_FACTORY_H_
