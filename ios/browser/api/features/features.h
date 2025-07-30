// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_API_FEATURES_FEATURES_H_
#define LUXXLE_IOS_BROWSER_API_FEATURES_FEATURES_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface Feature : NSObject
@property(nonatomic, readonly) NSString* name;
@property(nonatomic, readonly) bool enabled;
@end

OBJC_EXPORT
@interface FeatureList : NSObject
// MARK: - Luxxle Features

@property(class, nonatomic, readonly) Feature* kAIChat;
@property(class, nonatomic, readonly) Feature* kAIChatHistory;
@property(class, nonatomic, readonly)
    Feature* kAdblockOverrideRegexDiscardPolicy;
@property(class, nonatomic, readonly)
    Feature* kAllowUnsupportedWalletProvidersFeature;
@property(class, nonatomic, readonly) Feature* kLuxxleAdblockCnameUncloaking;
@property(class, nonatomic, readonly)
    Feature* kLuxxleAdblockCollapseBlockedElements;
@property(class, nonatomic, readonly) Feature* kLuxxleAdblockCookieListDefault;
@property(class, nonatomic, readonly) Feature* kLuxxleAdblockCookieListOptIn;
@property(class, nonatomic, readonly) Feature* kLuxxleAdblockCosmeticFiltering;
@property(class, nonatomic, readonly) Feature* kLuxxleAdblockCspRules;
@property(class, nonatomic, readonly) Feature* kLuxxleAdblockDefault1pBlocking;
@property(class, nonatomic, readonly)
    Feature* kLuxxleAdblockMobileNotificationsListDefault;
@property(class, nonatomic, readonly) Feature* kLuxxleAdblockScriptletDebugLogs;
@property(class, nonatomic, readonly) Feature* kLuxxleDarkModeBlock;
@property(class, nonatomic, readonly) Feature* kLuxxleDeAMP;
@property(class, nonatomic, readonly) Feature* kLuxxleDebounce;
@property(class, nonatomic, readonly) Feature* kLuxxleDomainBlock;
@property(class, nonatomic, readonly) Feature* kLuxxleDomainBlock1PES;
@property(class, nonatomic, readonly) Feature* kLuxxleLocalhostAccessPermission;
@property(class, nonatomic, readonly) Feature* kLuxxleNTPBrandedWallpaper;
@property(class, nonatomic, readonly) Feature* kLuxxleNTPBrandedWallpaperDemo;
@property(class, nonatomic, readonly) Feature* kLuxxleNTPSuperReferralWallpaper;
@property(class, nonatomic, readonly) Feature* kLuxxleNewsCardPeekFeature;
@property(class, nonatomic, readonly) Feature* kLuxxleNewsFeedUpdate;
@property(class, nonatomic, readonly) Feature* kLuxxleReduceLanguage;
@property(class, nonatomic, readonly) Feature* kLuxxleSearchDefaultAPIFeature;
@property(class, nonatomic, readonly) Feature* kLuxxleShredFeature;
@property(class, nonatomic, readonly) Feature* kLuxxleShredCacheData;
@property(class, nonatomic, readonly) Feature* kLuxxleIOSDebugAdblock;
@property(class, nonatomic, readonly) Feature* kLuxxleIOSEnableFarblingPlugins;
@property(class, nonatomic, readonly)
    Feature* kLuxxleShowStrictFingerprintingMode;
@property(class, nonatomic, readonly) Feature* kLuxxleSync;
@property(class, nonatomic, readonly) Feature* kLuxxleWalletAnkrBalancesFeature;
@property(class, nonatomic, readonly) Feature* kLuxxleWalletBitcoinFeature;
@property(class, nonatomic, readonly) Feature* kLuxxleWalletZCashFeature;
@property(class, nonatomic, readonly) Feature* kConstellation;
@property(class, nonatomic, readonly) Feature* kConstellationEnclaveAttestation;
@property(class, nonatomic, readonly)
    Feature* kCosmeticFilteringExtraPerfMetrics;
@property(class, nonatomic, readonly) Feature* kCosmeticFilteringJsPerformance;
@property(class, nonatomic, readonly) Feature* kCosmeticFilteringSyncLoad;
@property(class, nonatomic, readonly, nullable) Feature* kGeminiFeature;
@property(class, nonatomic, readonly) Feature* kNTP;
@property(class, nonatomic, readonly) Feature* kNativeLuxxleWalletFeature;
@property(class, nonatomic, readonly) Feature* kOtherJSONDeprecation;
@property(class, nonatomic, readonly) Feature* kSkusFeature;
@property(class, nonatomic, readonly) Feature* kTypicalJSONDeprecation;
@property(class, nonatomic, readonly) Feature* kUseDevUpdaterUrl;
@property(class, nonatomic, readonly) Feature* kVerboseLoggingFeature;
@property(class, nonatomic, readonly) Feature* kNewPlaylistUI;
@property(class, nonatomic, readonly) Feature* kLuxxleHttpsByDefault;
@property(class, nonatomic, readonly) Feature* kBlockAllCookiesToggle;
@property(class, nonatomic, readonly) Feature* kModernBrowserMenuEnabled;
@property(class, nonatomic, readonly) Feature* kLuxxleTranslateEnabled;
@property(class, nonatomic, readonly) Feature* kLuxxleAppleTranslateEnabled;
@property(class, nonatomic, readonly) Feature* kUseLuxxleUserAgent;
@property(class, nonatomic, readonly) Feature* kUseChromiumWebViews;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_FEATURES_FEATURES_H_
