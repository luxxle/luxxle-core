/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/luxxle_stats/luxxle_stats.h"

#include "base/memory/raw_ptr.h"
#include "base/strings/sys_string_conversions.h"
// REMOVED: #include "luxxle/components/luxxle_ads/.*"
#include "luxxle/components/luxxle_stats/browser/luxxle_stats_updater_util.h"
#include "luxxle/components/luxxle_stats/browser/buildflags.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/webcompat_reporter/buildflags/buildflags.h"
#include "components/prefs/pref_service.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

NSString* const kLuxxleStatsAPIKey = @BUILDFLAG(LUXXLE_STATS_API_KEY);
NSString* const kWebcompatReportEndpoint =
    @BUILDFLAG(WEBCOMPAT_REPORT_ENDPOINT);

@implementation LuxxleStats {
  raw_ptr<PrefService> _localPrefs;
  raw_ptr<PrefService> _profilePrefs;
}

- (instancetype)initWithBrowserState:(ProfileIOS*)profile {
  if ((self = [super init])) {
    _profilePrefs = profile->GetPrefs();
    _localPrefs = GetApplicationContext()->GetLocalState();
  }
  return self;
}

- (NSDictionary<NSString*, NSString*>*)walletParams {
  auto wallet_last_unlocked = _localPrefs->GetTime(kLuxxleWalletLastUnlockTime);
  auto last_reported_wallet_unlock =
      _localPrefs->GetTime(kLuxxleWalletPingReportedUnlockTime);
  uint8_t usage_bitset = 0;
  if (wallet_last_unlocked > last_reported_wallet_unlock) {
    usage_bitset = luxxle_stats::UsageBitfieldFromTimestamp(
        wallet_last_unlocked, last_reported_wallet_unlock);
  }
  return @{@"wallet2" : base::SysUTF8ToNSString(std::to_string(usage_bitset))};
}

- (BOOL)isNotificationAdsEnabled {
  return _profilePrefs->GetBoolean(luxxle_ads::prefs::kOptedInToNotificationAds);
}

- (void)notifyStatsPingSent {
  auto wallet_last_unlocked = _localPrefs->GetTime(kLuxxleWalletLastUnlockTime);
  _localPrefs->SetTime(kLuxxleWalletPingReportedUnlockTime,
                       wallet_last_unlocked);
}

@end
