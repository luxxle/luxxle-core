/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/profile/model/luxxle_keyed_service_factories.h"

#include "luxxle/ios/browser/api/ai_chat/ai_chat_service_factory.h"
#include "luxxle/ios/browser/api/ai_chat/model_service_factory.h"
#include "luxxle/ios/browser/api/webcompat_reporter/webcompat_reporter_service_factory.h"
#include "luxxle/ios/browser/luxxle_ads/ads_service_factory_ios.h"
#include "luxxle/ios/browser/luxxle_wallet/asset_ratio_service_factory.h"
#include "luxxle/ios/browser/luxxle_wallet/luxxle_wallet_ipfs_service_factory.h"
#include "luxxle/ios/browser/luxxle_wallet/luxxle_wallet_service_factory.h"
#include "luxxle/ios/browser/luxxle_wallet/meld_integration_service_factory.h"
#include "luxxle/ios/browser/luxxle_wallet/swap_service_factory.h"
#include "luxxle/ios/browser/debounce/debounce_service_factory+private.h"
#include "luxxle/ios/browser/favicon/luxxle_ios_favicon_loader_factory.h"
#include "luxxle/ios/browser/skus/skus_service_factory.h"
#include "luxxle/ios/browser/url_sanitizer/url_sanitizer_service_factory+private.h"

namespace luxxle {

void EnsureProfileKeyedServiceFactoriesBuilt() {
  ai_chat::ModelServiceFactory::GetInstance();
  ai_chat::AIChatServiceFactory::GetInstance();
  luxxle_ads::AdsServiceFactoryIOS::GetInstance();
  luxxle_favicon::LuxxleIOSFaviconLoaderFactory::GetInstance();
  luxxle_wallet::AssetRatioServiceFactory::GetInstance();
  luxxle_wallet::LuxxleWalletIpfsServiceFactory::GetInstance();
  luxxle_wallet::LuxxleWalletServiceFactory::GetInstance();
  luxxle_wallet::MeldIntegrationServiceFactory::GetInstance();
  luxxle_wallet::SwapServiceFactory::GetInstance();
  skus::SkusServiceFactory::GetInstance();
  luxxle::URLSanitizerServiceFactory::GetInstance();
  debounce::DebounceServiceFactory::GetInstance();
  webcompat_reporter::WebcompatReporterServiceFactory::GetInstance();
}

}  // namespace luxxle
