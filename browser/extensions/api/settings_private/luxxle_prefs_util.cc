/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/api/settings_private/luxxle_prefs_util.h"

#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#include "luxxle/components/ai_chat/core/common/pref_names.h"
#include "luxxle/components/luxxle_news/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/luxxle_shields/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/luxxle_wayback_machine/buildflags/buildflags.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/de_amp/common/pref_names.h"
#include "luxxle/components/debounce/core/common/pref_names.h"
#include "luxxle/components/decentralized_dns/core/pref_names.h"
#include "luxxle/components/ntp_background_images/common/pref_names.h"
#include "luxxle/components/omnibox/browser/luxxle_omnibox_prefs.h"
#include "luxxle/components/playlist/common/buildflags/buildflags.h"
#include "luxxle/components/request_otr/common/pref_names.h"
#include "luxxle/components/speedreader/common/buildflags/buildflags.h"
#include "luxxle/components/tor/buildflags/buildflags.h"
#include "luxxle/components/web_discovery/buildflags/buildflags.h"
#include "luxxle/components/webcompat_reporter/common/pref_names.h"
#include "chrome/browser/extensions/api/settings_private/prefs_util.h"
#include "chrome/common/extensions/api/settings_private.h"
#include "chrome/common/pref_names.h"
#include "components/browsing_data/core/pref_names.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/omnibox/browser/omnibox_prefs.h"
#include "components/search_engines/search_engines_pref_names.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_LUXXLE_WAYBACK_MACHINE)
#include "luxxle/components/luxxle_wayback_machine/pref_names.h"
#endif

#if defined(TOOLKIT_VIEWS)
#include "luxxle/components/sidebar/browser/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "luxxle/components/speedreader/speedreader_pref_names.h"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "luxxle/components/tor/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
#include "luxxle/components/playlist/browser/pref_names.h"
#endif

namespace extensions {

using ntp_background_images::prefs::kNewTabPageShowBackgroundImage;
using ntp_background_images::prefs::
    kNewTabPageShowSponsoredImagesBackgroundImage;
using ntp_background_images::prefs::kNewTabPageSuperReferralThemesOption;

namespace settings_api = api::settings_private;

const PrefsUtil::TypedPrefMap& LuxxlePrefsUtil::GetAllowlistedKeys() {
  // Static cache, similar to parent class
  static PrefsUtil::TypedPrefMap* s_luxxle_allowlist = nullptr;
  if (s_luxxle_allowlist) {
    return *s_luxxle_allowlist;
  }
  s_luxxle_allowlist = new PrefsUtil::TypedPrefMap();
  // Start with parent class allowlist
  const auto chromium_prefs = PrefsUtil::GetAllowlistedKeys();
  s_luxxle_allowlist->insert(chromium_prefs.begin(), chromium_prefs.end());
  // Add Luxxle values to the allowlist
  // import data
  (*s_luxxle_allowlist)[kImportDialogExtensions] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kImportDialogPayments] =
      settings_api::PrefType::kBoolean;
  // Default Luxxle shields
  (*s_luxxle_allowlist)[kShieldsAdvancedViewEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kShieldsStatsBadgeVisible] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kAdControlType] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNoScriptControlType] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kGoogleLoginControlType] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_shields::prefs::kFBEmbedControlType] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_shields::prefs::kTwitterEmbedControlType] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_shields::prefs::kLinkedInEmbedControlType] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_shields::prefs::kReduceLanguageEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_shields::prefs::kAdBlockDeveloperMode] =
      settings_api::PrefType::kBoolean;

  // Webcompat Reporter
  (*s_luxxle_allowlist)[webcompat_reporter::prefs::kContactInfoSaveFlagPrefs] =
      settings_api::PrefType::kBoolean;

  // Rewards/Ads prefs
  (*s_luxxle_allowlist)[luxxle_rewards::prefs::kEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_rewards::prefs::kShowLocationBarButton] =
      settings_api::PrefType::kBoolean;

  // Search engine prefs
  (*s_luxxle_allowlist)[prefs::kAddOpenSearchEngines] =
      settings_api::PrefType::kBoolean;

  // autofill prefs
  (*s_luxxle_allowlist)[kLuxxleAutofillPrivateWindows] =
      settings_api::PrefType::kBoolean;

  // appearance prefs
  (*s_luxxle_allowlist)[kShowBookmarksButton] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kShowSidePanelButton] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_news::prefs::kShouldShowToolbarButton] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kLocationBarIsWide] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[omnibox::kAutocompleteEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[omnibox::kTopSuggestionsEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[omnibox::kHistorySuggestionsEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[omnibox::kBookmarkSuggestionsEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[omnibox::kCommanderSuggestionsEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kAskEnableWidvine] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNewTabPageSuperReferralThemesOption] =
      settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[kTabsSearchShow] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_tabs::kTabHoverMode] =
      settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[kTabMuteIndicatorNotClickable] =
      settings_api::PrefType::kBoolean;
#if BUILDFLAG(IS_MAC)
  (*s_luxxle_allowlist)[prefs::kShowFullscreenToolbar] =
      settings_api::PrefType::kBoolean;
#endif
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  (*s_luxxle_allowlist)[luxxle_vpn::prefs::kLuxxleVPNShowButton] =
      settings_api::PrefType::kBoolean;
#if BUILDFLAG(ENABLE_LUXXLE_VPN_WIREGUARD)
  (*s_luxxle_allowlist)[luxxle_vpn::prefs::kLuxxleVPNWireguardEnabled] =
      settings_api::PrefType::kBoolean;
#endif
#endif
#if defined(TOOLKIT_VIEWS)
  (*s_luxxle_allowlist)[sidebar::kSidebarShowOption] =
      settings_api::PrefType::kNumber;
#endif
#if BUILDFLAG(ENABLE_SPEEDREADER)
  (*s_luxxle_allowlist)[speedreader::kSpeedreaderPrefEnabled] =
      settings_api::PrefType::kBoolean;
#endif
  // De-AMP feature
  (*s_luxxle_allowlist)[de_amp::kDeAmpPrefEnabled] =
      settings_api::PrefType::kBoolean;
  // Debounce feature
  (*s_luxxle_allowlist)[debounce::prefs::kDebounceEnabled] =
      settings_api::PrefType::kBoolean;
  // new tab prefs
  (*s_luxxle_allowlist)[kNewTabPageShowSponsoredImagesBackgroundImage] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNewTabPageShowBackgroundImage] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNewTabPageShowClock] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNewTabPageShowStats] = settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNewTabPageShowRewards] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNewTabPageShowLuxxleTalk] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNewTabPageShowLuxxleVPN] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kNewTabPageShowsOptions] =
      settings_api::PrefType::kNumber;
#if BUILDFLAG(ENABLE_EXTENSIONS) || BUILDFLAG(ENABLE_WEB_DISCOVERY_NATIVE)
  // Web discovery prefs
  (*s_luxxle_allowlist)[kWebDiscoveryEnabled] = settings_api::PrefType::kBoolean;
#endif
  // Clear browsing data on exit prefs.
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteBrowsingHistoryOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteDownloadHistoryOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteCacheOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteCookiesOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeletePasswordsOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteFormDataOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteSiteSettingsOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteHostedAppsDataOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteLuxxleLeoHistory] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[browsing_data::prefs::kDeleteLuxxleLeoHistoryOnExit] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kAlwaysShowBookmarkBarOnNTP] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kMRUCyclingEnabled] = settings_api::PrefType::kBoolean;
  // WebTorrent pref
  (*s_luxxle_allowlist)[kWebTorrentEnabled] = settings_api::PrefType::kBoolean;
#if BUILDFLAG(ENABLE_LUXXLE_WAYBACK_MACHINE)
  (*s_luxxle_allowlist)[kLuxxleWaybackMachineEnabled] =
      settings_api::PrefType::kBoolean;
#endif
  (*s_luxxle_allowlist)[kEnableWindowClosingConfirm] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kEnableClosingLastTab] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kShowFullscreenReminder] =
      settings_api::PrefType::kBoolean;

  // Luxxle Wallet pref
  (*s_luxxle_allowlist)[kLuxxleWalletSelectedNetworks] =
      settings_api::PrefType::kDictionary;
  (*s_luxxle_allowlist)[kDefaultEthereumWallet] =
      settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[kDefaultSolanaWallet] = settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[kDefaultBaseCurrency] = settings_api::PrefType::kString;
  (*s_luxxle_allowlist)[kDefaultBaseCryptocurrency] =
      settings_api::PrefType::kString;
  (*s_luxxle_allowlist)[kShowWalletIconOnToolbar] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kLuxxleWalletAutoLockMinutes] =
      settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[kLuxxleWalletTransactionSimulationOptInStatus] =
      settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[kLuxxleWalletNftDiscoveryEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[kLuxxleWalletPrivateWindowsEnabled] =
      settings_api::PrefType::kBoolean;

  // Leo Assistant pref
  (*s_luxxle_allowlist)[ai_chat::prefs::kLuxxleChatStorageEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[ai_chat::prefs::kLuxxleChatAutocompleteProviderEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[ai_chat::prefs::kLuxxleAIChatContextMenuEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[ai_chat::prefs::kLuxxleAIChatShowToolbarButton] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[ai_chat::prefs::kLuxxleAIChatTabOrganizationEnabled] =
      settings_api::PrefType::kBoolean;

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
  // Push Messaging Pref
  (*s_luxxle_allowlist)[kLuxxleGCMChannelStatus] =
      settings_api::PrefType::kBoolean;
#endif
  // Omnibox pref
  (*s_luxxle_allowlist)[omnibox::kPreventUrlElisionsInOmnibox] =
      settings_api::PrefType::kBoolean;
#if BUILDFLAG(ENABLE_TOR)
  (*s_luxxle_allowlist)[tor::prefs::kOnionOnlyInTorWindows] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[tor::prefs::kBridgesConfig] =
      settings_api::PrefType::kDictionary;
#endif
  (*s_luxxle_allowlist)[prefs::kWebRTCIPHandlingPolicy] =
      settings_api::PrefType::kString;
  // Request OTR feature
  (*s_luxxle_allowlist)[request_otr::kRequestOTRActionOption] =
      settings_api::PrefType::kNumber;

  (*s_luxxle_allowlist)[decentralized_dns::kUnstoppableDomainsResolveMethod] =
      settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[decentralized_dns::kENSResolveMethod] =
      settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[decentralized_dns::kEnsOffchainResolveMethod] =
      settings_api::PrefType::kNumber;
  (*s_luxxle_allowlist)[decentralized_dns::kSnsResolveMethod] =
      settings_api::PrefType::kNumber;

  // Media router pref
  (*s_luxxle_allowlist)[kEnableMediaRouterOnRestart] =
      settings_api::PrefType::kBoolean;

#if defined(TOOLKIT_VIEWS)
  // Vertical tab strip prefs
  (*s_luxxle_allowlist)[luxxle_tabs::kVerticalTabsEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_tabs::kVerticalTabsFloatingEnabled] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_tabs::kVerticalTabsShowTitleOnWindow] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_tabs::kVerticalTabsOnRight] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_tabs::kVerticalTabsShowScrollbar] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[luxxle_tabs::kVerticalTabsExpandedStatePerWindow] =
      settings_api::PrefType::kBoolean;
#endif

#if BUILDFLAG(ENABLE_PLAYLIST)
  (*s_luxxle_allowlist)[playlist::kPlaylistEnabledPref] =
      settings_api::PrefType::kBoolean;
  (*s_luxxle_allowlist)[playlist::kPlaylistCacheByDefault] =
      settings_api::PrefType::kBoolean;
#endif

#if !BUILDFLAG(IS_ANDROID)
  (*s_luxxle_allowlist)[luxxle_tabs::kSharedPinnedTab] =
      settings_api::PrefType::kBoolean;
#endif

  return *s_luxxle_allowlist;
}

}  // namespace extensions
