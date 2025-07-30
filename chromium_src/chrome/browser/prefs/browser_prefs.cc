/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/luxxle_local_state_prefs.h"
#include "luxxle/browser/luxxle_profile_prefs.h"
#include "luxxle/browser/luxxle_rewards/rewards_prefs_util.h"
#include "luxxle/browser/luxxle_stats/luxxle_stats_updater.h"
#include "luxxle/browser/misc_metrics/uptime_monitor_impl.h"
#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"
#include "luxxle/browser/translate/luxxle_translate_prefs_migration.h"
#include "luxxle/components/ai_chat/core/browser/model_service.h"
#include "luxxle/components/luxxle_adaptive_captcha/prefs_util.h"
#include "luxxle/components/luxxle_ads/core/public/prefs/obsolete_pref_util.h"
#include "luxxle/components/luxxle_news/browser/luxxle_news_p3a.h"
#include "luxxle/components/luxxle_news/common/p3a_pref_names.h"
#include "luxxle/components/luxxle_news/common/pref_names.h"
#include "luxxle/components/luxxle_search_conversion/p3a.h"
#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_p3a.h"
#include "luxxle/components/luxxle_sync/luxxle_sync_prefs.h"
#include "luxxle/components/luxxle_vpn/common/buildflags/buildflags.h"
#include "luxxle/components/luxxle_wallet/browser/luxxle_wallet_prefs.h"
#include "luxxle/components/luxxle_wallet/browser/keyring_service.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/decentralized_dns/core/utils.h"
#include "luxxle/components/ipfs/ipfs_prefs.h"
#include "luxxle/components/ntp_background_images/buildflags/buildflags.h"
#include "luxxle/components/ntp_background_images/common/view_counter_pref_registry.h"
#include "luxxle/components/omnibox/browser/luxxle_omnibox_prefs.h"
#include "luxxle/components/p3a/star_randomness_meta.h"
#include "luxxle/components/tor/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/channel_info.h"
#include "chrome/common/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/translate/core/browser/translate_prefs.h"
#include "extensions/buildflags/buildflags.h"
#include "third_party/widevine/cdm/buildflags.h"

#if !BUILDFLAG(IS_ANDROID)
#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#endif

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
#include "luxxle/components/luxxle_vpn/common/luxxle_vpn_utils.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "luxxle/components/tor/pref_names.h"
#include "luxxle/components/tor/tor_utils.h"
#endif

#if BUILDFLAG(ENABLE_WIDEVINE)
#include "luxxle/browser/widevine/widevine_utils.h"
#endif

#if !BUILDFLAG(ENABLE_EXTENSIONS)
#define CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_PROVIDER_H_
#endif  // !BUILDFLAG(ENABLE_EXTENSIONS)

#define MigrateObsoleteProfilePrefs MigrateObsoleteProfilePrefs_ChromiumImpl
#define MigrateObsoleteLocalStatePrefs \
  MigrateObsoleteLocalStatePrefs_ChromiumImpl
#include "src/chrome/browser/prefs/browser_prefs.cc"
#undef MigrateObsoleteProfilePrefs
#undef MigrateObsoleteLocalStatePrefs

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "luxxle/browser/gcm_driver/luxxle_gcm_utils.h"
#endif

#if BUILDFLAG(ENABLE_CUSTOM_BACKGROUND)
#include "luxxle/browser/ntp_background/ntp_background_prefs.h"
#endif

#if defined(TOOLKIT_VIEWS)
#include "luxxle/components/sidebar/browser/pref_names.h"
#endif

// This method should be periodically pruned of year+ old migrations.
void MigrateObsoleteProfilePrefs(PrefService* profile_prefs,
                                 const base::FilePath& profile_path) {
  DCHECK(profile_prefs);
  // BEGIN_MIGRATE_OBSOLETE_PROFILE_PREFS
#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  // Added 02/2020.
  // Must be called before ChromiumImpl because it's migrating a Chromium pref
  // to Luxxle pref.
  gcm::MigrateGCMPrefs(profile_prefs);
#endif

  MigrateObsoleteProfilePrefs_ChromiumImpl(profile_prefs, profile_path);

  luxxle_sync::MigrateLuxxleSyncPrefs(profile_prefs);

#if !BUILDFLAG(IS_ANDROID)
  // Added 10/2022
  profile_prefs->ClearPref(kDefaultBrowserLaunchingCount);
#endif

#if BUILDFLAG(ENABLE_EXTENSIONS)
  // Added 11/2022
  profile_prefs->ClearPref(kDontAskEnableWebDiscovery);
  profile_prefs->ClearPref(kLuxxleSearchVisitCount);
#endif

  luxxle_wallet::MigrateObsoleteProfilePrefs(profile_prefs);

  // Added 05/2021
  profile_prefs->ClearPref(kLuxxleNewsIntroDismissed);
  // Added 07/2021
  profile_prefs->ClearPref(prefs::kNetworkPredictionOptions);

  // Added 01/2022
  luxxle_rewards::MigrateObsoleteProfilePrefs(profile_prefs);

  // Added 05/2022
  translate::ClearMigrationLuxxleProfilePrefs(profile_prefs);

  // Added 06/2022
#if BUILDFLAG(ENABLE_CUSTOM_BACKGROUND)
  NTPBackgroundPrefs(profile_prefs).MigrateOldPref();
#endif

  // Added 24/11/2022: https://github.com/luxxle/luxxle-core/pull/16027
#if !BUILDFLAG(IS_IOS) && !BUILDFLAG(IS_ANDROID)
  profile_prefs->ClearPref(kFTXAccessToken);
  profile_prefs->ClearPref(kFTXOauthHost);
  profile_prefs->ClearPref(kFTXNewTabPageShowFTX);
  profile_prefs->ClearPref(kCryptoDotComNewTabPageShowCryptoDotCom);
  profile_prefs->ClearPref(kCryptoDotComHasBoughtCrypto);
  profile_prefs->ClearPref(kCryptoDotComHasInteracted);
  profile_prefs->ClearPref(kGeminiAccessToken);
  profile_prefs->ClearPref(kGeminiRefreshToken);
  profile_prefs->ClearPref(kNewTabPageShowGemini);
#endif

  // Added 24/11/2022: https://github.com/luxxle/luxxle-core/pull/16027
#if !BUILDFLAG(IS_IOS)
  profile_prefs->ClearPref(kBinanceAccessToken);
  profile_prefs->ClearPref(kBinanceRefreshToken);
  profile_prefs->ClearPref(kNewTabPageShowBinance);
  profile_prefs->ClearPref(kLuxxleSuggestedSiteSuggestionsEnabled);
#endif

  // Added 03/2024
#if BUILDFLAG(ENABLE_TOR)
  profile_prefs->ClearPref(tor::prefs::kAutoOnionRedirect);
#endif

#if defined(TOOLKIT_VIEWS)
  // Added May 2023
  if (profile_prefs->GetBoolean(sidebar::kSidebarAlignmentChangedTemporarily)) {
    // If temporarily changed, it means sidebar is set to right.
    // Just clear alignment prefs as default alignment is changed to right.
    profile_prefs->ClearPref(prefs::kSidePanelHorizontalAlignment);
  }

  profile_prefs->ClearPref(sidebar::kSidebarAlignmentChangedTemporarily);
#endif

  luxxle_news::p3a::prefs::MigrateObsoleteProfileNewsMetricsPrefs(profile_prefs);

  // Added 2023-09
  ntp_background_images::MigrateObsoleteProfilePrefs(profile_prefs);

  // Added 2023-11
  luxxle_ads::MigrateObsoleteProfilePrefs(profile_prefs);

  luxxle_shields::MigrateObsoleteProfilePrefs(profile_prefs);

#if !BUILDFLAG(IS_ANDROID)
  // Added 2024-01
  luxxle_tabs::MigrateLuxxleProfilePrefs(profile_prefs);
#endif  // !BUILDFLAG(IS_ANDROID)

  // Added 2024-04
  ai_chat::ModelService::MigrateProfilePrefs(profile_prefs);

  // Added 2024-05
  ipfs::ClearDeprecatedIpfsPrefs(profile_prefs);

  // Added 2024-07
  profile_prefs->ClearPref(kHangoutsEnabled);

  // Added 2024-10
  luxxle_adaptive_captcha::MigrateObsoleteProfilePrefs(profile_prefs);

#if BUILDFLAG(IS_ANDROID)
  // Added 27/11/2024: https://github.com/luxxle/luxxle-core/pull/26719
  profile_prefs->ClearPref(kSafetynetCheckFailed);
  profile_prefs->ClearPref(kSafetynetStatus);
#endif  // BUILDFLAG(IS_ANDROID)

  // Added 2025-05
#if !BUILDFLAG(IS_IOS) && !BUILDFLAG(IS_ANDROID)
  profile_prefs->ClearPref(kWebTorrentEnabled);
#endif

  // END_MIGRATE_OBSOLETE_PROFILE_PREFS
}

// This method should be periodically pruned of year+ old migrations.
void MigrateObsoleteLocalStatePrefs(PrefService* local_state) {
  // BEGIN_MIGRATE_OBSOLETE_LOCAL_STATE_PREFS
  MigrateObsoleteLocalStatePrefs_ChromiumImpl(local_state);

#if BUILDFLAG(ENABLE_TOR)
  // Added 4/2021.
  tor::MigrateLastUsedProfileFromLocalStatePrefs(local_state);
#endif

  decentralized_dns::MigrateObsoleteLocalStatePrefs(local_state);

#if !BUILDFLAG(IS_ANDROID)
  // Added 10/2022
  local_state->ClearPref(kDefaultBrowserPromptEnabled);
#endif

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  // Added 09/2024
  luxxle_vpn::MigrateLocalStatePrefs(local_state);
#endif

  misc_metrics::UptimeMonitorImpl::MigrateObsoletePrefs(local_state);
  luxxle_search_conversion::p3a::MigrateObsoleteLocalStatePrefs(local_state);
  luxxle_stats::MigrateObsoleteLocalStatePrefs(local_state);
  p3a::StarRandomnessMeta::MigrateObsoleteLocalStatePrefs(local_state);

  // END_MIGRATE_OBSOLETE_LOCAL_STATE_PREFS
}

#if !BUILDFLAG(ENABLE_EXTENSIONS)
#undef CHROME_BROWSER_WEB_APPLICATIONS_WEB_APP_PROVIDER_H_
#endif  // !BUILDFLAG(ENABLE_EXTENSIONS)
