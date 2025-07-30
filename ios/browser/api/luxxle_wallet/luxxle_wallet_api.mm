/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/luxxle_wallet/luxxle_wallet_api.h"

#include "base/memory/raw_ptr.h"
#include "base/strings/sys_string_conversions.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/ios/browser/api/luxxle_wallet/luxxle_wallet.mojom.objc+private.h"
#include "luxxle/ios/browser/api/luxxle_wallet/luxxle_wallet_provider_delegate_ios+private.h"
#include "luxxle/ios/browser/api/luxxle_wallet/luxxle_wallet_provider_delegate_ios.h"
#include "luxxle/ios/browser/luxxle_wallet/luxxle_wallet_service_factory.h"
#include "components/grit/luxxle_components_resources.h"
#include "ios/chrome/browser/content_settings/model/host_content_settings_map_factory.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/chrome/browser/shared/model/profile/profile_manager_ios.h"
#include "ios/web/public/thread/web_thread.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "ui/base/resource/resource_bundle.h"

LuxxleWalletProviderScriptKey const LuxxleWalletProviderScriptKeyEthereum =
    @"ethereum_provider.js";
LuxxleWalletProviderScriptKey const LuxxleWalletProviderScriptKeySolana =
    @"solana_provider.js";
LuxxleWalletProviderScriptKey const LuxxleWalletProviderScriptKeySolanaWeb3 =
    @"solana_web3.js";
LuxxleWalletProviderScriptKey const LuxxleWalletProviderScriptKeyWalletStandard =
    @"wallet_standard.js";

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@implementation LuxxleWallet
@end

@implementation LuxxleWalletAPI {
  raw_ptr<ProfileIOS> _profile;  // NOT OWNED
  NSMutableDictionary<NSNumber* /* LuxxleWalletCoinType */,
                      NSDictionary<LuxxleWalletProviderScriptKey, NSString*>*>*
      _providerScripts;
}

- (instancetype)initWithBrowserState:(ProfileIOS*)profile {
  if ((self = [super init])) {
    _profile = profile;
    _providerScripts = [[NSMutableDictionary alloc] init];
  }
  return self;
}

+ (id<LuxxleWalletBlockchainRegistry>)blockchainRegistry {
  auto* registry = luxxle_wallet::BlockchainRegistry::GetInstance();
  return [[LuxxleWalletBlockchainRegistryMojoImpl alloc]
      initWithBlockchainRegistry:registry->MakeRemote()];
}

- (nullable id<LuxxleWalletEthereumProvider>)
    ethereumProviderWithDelegate:(id<LuxxleWalletProviderDelegate>)delegate
               isPrivateBrowsing:(bool)isPrivateBrowsing {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* profile = _profile.get();
  if (isPrivateBrowsing) {
    profile = profile->GetOffTheRecordProfile();
  }

  auto* luxxle_wallet_service =
      luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForState(profile);
  if (!luxxle_wallet_service) {
    return nil;
  }

  auto provider = std::make_unique<luxxle_wallet::EthereumProviderImpl>(
      ios::HostContentSettingsMapFactory::GetForProfile(profile),
      luxxle_wallet_service,
      std::make_unique<luxxle_wallet::LuxxleWalletProviderDelegateBridge>(
          delegate),
      profile->GetPrefs());
  return [[LuxxleWalletEthereumProviderMojoImpl alloc]
      initWithEthereumProviderImpl:std::move(provider)];
}

- (nullable id<LuxxleWalletSolanaProvider>)
    solanaProviderWithDelegate:(id<LuxxleWalletProviderDelegate>)delegate
             isPrivateBrowsing:(bool)isPrivateBrowsing {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* profile = _profile.get();
  if (isPrivateBrowsing) {
    profile = profile->GetOffTheRecordProfile();
  }

  auto* luxxle_wallet_service =
      luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForState(profile);
  if (!luxxle_wallet_service) {
    return nil;
  }

  auto* host_content_settings_map =
      ios::HostContentSettingsMapFactory::GetForProfile(profile);
  if (!host_content_settings_map) {
    return nil;
  }

  auto provider = std::make_unique<luxxle_wallet::SolanaProviderImpl>(
      *host_content_settings_map, luxxle_wallet_service,
      std::make_unique<luxxle_wallet::LuxxleWalletProviderDelegateBridge>(
          delegate));
  return [[LuxxleWalletSolanaProviderMojoImpl alloc]
      initWithSolanaProviderImpl:std::move(provider)];
}

- (NSString*)resourceForID:(int)resource_id {
  // The resource bundle is not available until after WebMainParts is setup
  auto& resource_bundle = ui::ResourceBundle::GetSharedInstance();
  std::string resource_string = "";
  if (resource_bundle.IsGzipped(resource_id)) {
    resource_string =
        std::string(resource_bundle.LoadDataResourceString(resource_id));
  } else {
    resource_string =
        std::string(resource_bundle.GetRawDataResource(resource_id));
  }
  return base::SysUTF8ToNSString(resource_string);
}

- (NSDictionary<LuxxleWalletProviderScriptKey, NSString*>*)
    providerScriptsForCoinType:(LuxxleWalletCoinType)coinType {
  auto cachedScript = _providerScripts[@(coinType)];
  if (cachedScript) {
    return cachedScript;
  }
  auto resource_ids =
      ^std::vector<std::pair<LuxxleWalletProviderScriptKey, int>> {
    switch (coinType) {
      case LuxxleWalletCoinTypeEth:
        return {std::make_pair(
            LuxxleWalletProviderScriptKeyEthereum,
            /* REMOVED: IDR_LUXXLE_WALLET_SCRIPT_ETHEREUM_PROVIDER_SCRIPT_BUNDLE_JS */ 0)};
      case LuxxleWalletCoinTypeSol:
        return {std::make_pair(
                    LuxxleWalletProviderScriptKeySolana,
                    /* REMOVED: IDR_LUXXLE_WALLET_SCRIPT_SOLANA_PROVIDER_SCRIPT_BUNDLE_JS */ 0),
                std::make_pair(LuxxleWalletProviderScriptKeySolanaWeb3,
                               /* REMOVED: IDR_LUXXLE_WALLET_SOLANA_WEB3_JS */ 0),
                std::make_pair(LuxxleWalletProviderScriptKeyWalletStandard,
                               /* REMOVED: IDR_LUXXLE_WALLET_STANDARD_JS */ 0)};
      case LuxxleWalletCoinTypeFil:
        // Currently not supported
        return {std::make_pair(@"", 0)};
      case LuxxleWalletCoinTypeBtc:
        // Currently not supported
        return {std::make_pair(@"", 0)};
      case LuxxleWalletCoinTypeZec:
        // Currently not supported
        return {std::make_pair(@"", 0)};
      case LuxxleWalletCoinTypeAda:
        // Currently not supported
        return {std::make_pair(@"", 0)};
    }
    return {std::make_pair(@"", 0)};
  }
  ();
  const auto scripts = [[NSMutableDictionary alloc] init];
  for (auto resource : resource_ids) {
    auto key = resource.first;
    auto resource_id = resource.second;
    scripts[key] = [self resourceForID:resource_id];
  }
  _providerScripts[@(coinType)] = [scripts copy];
  return scripts;
}

- (nullable id<LuxxleWalletLuxxleWalletP3A>)walletP3A {
  auto* service =
      luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForState(_profile);
  if (!service) {
    return nil;
  }

  mojo::PendingRemote<luxxle_wallet::mojom::LuxxleWalletP3A> pending_remote;
  service->Bind(pending_remote.InitWithNewPipeAndPassReceiver());
  return [[LuxxleWalletLuxxleWalletP3AMojoImpl alloc]
      initWithLuxxleWalletP3A:std::move(pending_remote)];
}

@end
