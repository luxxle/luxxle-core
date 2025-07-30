// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "features.h"

#include "base/memory/raw_ptr.h"
#include "base/strings/sys_string_conversions.h"
#include "luxxle/components/ai_chat/core/common/features.h"
#include "luxxle/components/luxxle_component_updater/browser/features.h"
#include "luxxle/components/luxxle_news/common/features.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/luxxle_search/common/features.h"
#include "luxxle/components/luxxle_search_conversion/features.h"
#include "luxxle/components/luxxle_shields/core/common/features.h"
#include "luxxle/components/luxxle_sync/features.h"
#include "luxxle/components/luxxle_user_agent/common/features.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/de_amp/common/features.h"
#include "luxxle/components/debounce/core/common/features.h"
#include "luxxle/components/ntp_background_images/browser/features.h"
#include "luxxle/components/p3a/features.h"
#include "luxxle/components/playlist/common/features.h"
#include "luxxle/components/skus/common/features.h"
#include "luxxle/ios/browser/api/translate/features.h"
#include "luxxle/ios/browser/playlist/features.h"
#include "luxxle/ios/browser/ui/browser_menu/features.h"
#include "luxxle/ios/browser/ui/web_view/features.h"
#import "build/blink_buildflags.h"
#include "build/build_config.h"
#include "net/base/features.h"

@interface Feature () {
  raw_ptr<const base::Feature> _feature;
}
@end

@implementation Feature
- (instancetype)initWithFeature:(const base::Feature*)feature {
  if ((self = [super init])) {
    _feature = feature;
  }
  return self;
}

- (NSString*)name {
  return base::SysUTF8ToNSString(_feature->name);
}

- (bool)enabled {
  return base::FeatureList::IsEnabled(std::cref(*_feature));
}

//- (void)setEnabled:(bool)enabled {
//  std::vector<base::FeatureList::FeatureOverrideInfo> overrides = {
//    {
//        std::cref(*_value), enabled ?
//        base::FeatureList::OverrideState::OVERRIDE_ENABLE_FEATURE :
//        base::FeatureList::OverrideState::OVERRIDE_DISABLE_FEATURE
//    }
//  };
//  base::FeatureList::GetInstance()->RegisterExtraFeatureOverrides(overrides);
//}
@end

@implementation FeatureList

// MARK: - Luxxle Features

+ (Feature*)kAIChat {
  return [[Feature alloc] initWithFeature:&ai_chat::features::kAIChat];
}

+ (Feature*)kAIChatHistory {
  return [[Feature alloc] initWithFeature:&ai_chat::features::kAIChatHistory];
}

+ (Feature*)kAdblockOverrideRegexDiscardPolicy {
  return
      [[Feature alloc] initWithFeature:&luxxle_shields::features::
                                           kAdblockOverrideRegexDiscardPolicy];
}

+ (Feature*)kAllowUnsupportedWalletProvidersFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_rewards::features::
                          kAllowUnsupportedWalletProvidersFeature];
}

+ (Feature*)kLuxxleAdblockCnameUncloaking {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleAdblockCnameUncloaking];
}

+ (Feature*)kLuxxleAdblockCollapseBlockedElements {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::
                          kLuxxleAdblockCollapseBlockedElements];
}

+ (Feature*)kLuxxleAdblockCookieListDefault {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleAdblockCookieListDefault];
}

+ (Feature*)kLuxxleAdblockCookieListOptIn {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleAdblockCookieListOptIn];
}

+ (Feature*)kLuxxleAdblockCosmeticFiltering {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleAdblockCosmeticFiltering];
}

+ (Feature*)kLuxxleAdblockCspRules {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleAdblockCspRules];
}

+ (Feature*)kLuxxleAdblockDefault1pBlocking {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleAdblockDefault1pBlocking];
}

+ (Feature*)kLuxxleAdblockMobileNotificationsListDefault {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::
                          kLuxxleAdblockMobileNotificationsListDefault];
}

+ (Feature*)kLuxxleAdblockScriptletDebugLogs {
  return [[Feature alloc] initWithFeature:&luxxle_shields::features::
                                              kLuxxleAdblockScriptletDebugLogs];
}

+ (Feature*)kLuxxleDarkModeBlock {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleDarkModeBlock];
}

+ (Feature*)kLuxxleDeAMP {
  return [[Feature alloc] initWithFeature:&de_amp::features::kLuxxleDeAMP];
}

+ (Feature*)kLuxxleDebounce {
  return [[Feature alloc] initWithFeature:&debounce::features::kLuxxleDebounce];
}

+ (Feature*)kLuxxleDomainBlock {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleDomainBlock];
}

+ (Feature*)kLuxxleDomainBlock1PES {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleDomainBlock1PES];
}

+ (Feature*)kLuxxleLocalhostAccessPermission {
  return [[Feature alloc] initWithFeature:&luxxle_shields::features::
                                              kLuxxleLocalhostAccessPermission];
}

+ (Feature*)kLuxxleNTPBrandedWallpaper {
  return [[Feature alloc] initWithFeature:&ntp_background_images::features::
                                              kLuxxleNTPBrandedWallpaper];
}

+ (Feature*)kLuxxleNTPBrandedWallpaperDemo {
  return [[Feature alloc] initWithFeature:&ntp_background_images::features::
                                              kLuxxleNTPBrandedWallpaperDemo];
}

+ (Feature*)kLuxxleNTPSuperReferralWallpaper {
  return [[Feature alloc] initWithFeature:&ntp_background_images::features::
                                              kLuxxleNTPSuperReferralWallpaper];
}

+ (Feature*)kLuxxleNewsCardPeekFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_news::features::kLuxxleNewsCardPeekFeature];
}

+ (Feature*)kLuxxleNewsFeedUpdate {
  return [[Feature alloc]
      initWithFeature:&luxxle_news::features::kLuxxleNewsFeedUpdate];
}

+ (Feature*)kLuxxleReduceLanguage {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleReduceLanguage];
}

+ (Feature*)kLuxxleSearchDefaultAPIFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_search::features::kLuxxleSearchDefaultAPIFeature];
}

+ (Feature*)kLuxxleShredFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleShredFeature];
}

+ (Feature*)kLuxxleShredCacheData {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleShredCacheData];
}

+ (Feature*)kLuxxleIOSDebugAdblock {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleIOSDebugAdblock];
}

+ (Feature*)kLuxxleIOSEnableFarblingPlugins {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kLuxxleIOSEnableFarblingPlugins];
}

+ (Feature*)kLuxxleShowStrictFingerprintingMode {
  return
      [[Feature alloc] initWithFeature:&luxxle_shields::features::
                                           kLuxxleShowStrictFingerprintingMode];
}

+ (Feature*)kLuxxleSync {
  return [[Feature alloc] initWithFeature:&luxxle_sync::features::kLuxxleSync];
}

+ (Feature*)kLuxxleWalletAnkrBalancesFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_wallet::features::kLuxxleWalletAnkrBalancesFeature];
}

+ (Feature*)kLuxxleWalletBitcoinFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_wallet::features::kLuxxleWalletBitcoinFeature];
}

+ (Feature*)kLuxxleWalletZCashFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_wallet::features::kLuxxleWalletZCashFeature];
}

+ (Feature*)kConstellation {
  return [[Feature alloc] initWithFeature:&p3a::features::kConstellation];
}

+ (Feature*)kConstellationEnclaveAttestation {
  return [[Feature alloc]
      initWithFeature:&p3a::features::kConstellationEnclaveAttestation];
}

+ (Feature*)kCosmeticFilteringExtraPerfMetrics {
  return
      [[Feature alloc] initWithFeature:&luxxle_shields::features::
                                           kCosmeticFilteringExtraPerfMetrics];
}

+ (Feature*)kCosmeticFilteringJsPerformance {
  return [[Feature alloc] initWithFeature:&luxxle_shields::features::
                                              kCosmeticFilteringJsPerformance];
}

+ (Feature*)kCosmeticFilteringSyncLoad {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kCosmeticFilteringSyncLoad];
}

#if BUILDFLAG(ENABLE_GEMINI_WALLET)
+ (Feature*)kGeminiFeature {
  return [[Feature alloc] initWithFeature:&kGeminiFeature];
}
#else
+ (Feature*)kGeminiFeature {
  return nil;
}
#endif

+ (Feature*)kNTP {
  return [[Feature alloc]
      initWithFeature:&luxxle_search_conversion::features::kNTP];
}

+ (Feature*)kNativeLuxxleWalletFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_wallet::features::kNativeLuxxleWalletFeature];
}

+ (Feature*)kOtherJSONDeprecation {
  return
      [[Feature alloc] initWithFeature:&p3a::features::kOtherJSONDeprecation];
}

+ (Feature*)kSkusFeature {
  return [[Feature alloc] initWithFeature:&skus::features::kSkusFeature];
}

+ (Feature*)kTypicalJSONDeprecation {
  return
      [[Feature alloc] initWithFeature:&p3a::features::kTypicalJSONDeprecation];
}

+ (Feature*)kUseDevUpdaterUrl {
  return [[Feature alloc]
      initWithFeature:&luxxle_component_updater::kUseDevUpdaterUrl];
}

+ (Feature*)kVerboseLoggingFeature {
  return [[Feature alloc]
      initWithFeature:&luxxle_rewards::features::kVerboseLoggingFeature];
}

+ (Feature*)kNewPlaylistUI {
  return [[Feature alloc] initWithFeature:&playlist::features::kNewPlaylistUI];
}

+ (Feature*)kLuxxleHttpsByDefault {
  return [[Feature alloc] initWithFeature:&net::features::kLuxxleHttpsByDefault];
}

+ (Feature*)kBlockAllCookiesToggle {
  return [[Feature alloc]
      initWithFeature:&luxxle_shields::features::kBlockAllCookiesToggle];
}

+ (Feature*)kModernBrowserMenuEnabled {
  return [[Feature alloc]
      initWithFeature:&luxxle::features::kModernBrowserMenuEnabled];
}

+ (Feature*)kLuxxleTranslateEnabled {
  return [[Feature alloc]
      initWithFeature:&luxxle::features::kLuxxleTranslateEnabled];
}

+ (Feature*)kLuxxleAppleTranslateEnabled {
  return [[Feature alloc]
      initWithFeature:&luxxle::features::kLuxxleAppleTranslateEnabled];
}

+ (Feature*)kUseLuxxleUserAgent {
  return [[Feature alloc]
      initWithFeature:&luxxle_user_agent::features::kUseLuxxleUserAgent];
}

+ (Feature*)kUseChromiumWebViews {
  return
      [[Feature alloc] initWithFeature:&luxxle::features::kUseChromiumWebViews];
}

@end
