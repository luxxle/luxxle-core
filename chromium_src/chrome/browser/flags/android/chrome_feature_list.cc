/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/feature_override.h"
#include "luxxle/browser/android/safe_browsing/features.h"
#include "luxxle/browser/android/youtube_script_injector/features.h"
#include "luxxle/browser/luxxle_browser_features.h"
#include "luxxle/components/ai_chat/core/common/features.h"
#include "luxxle/components/luxxle_ads/core/public/ad_units/new_tab_page_ad/new_tab_page_ad_feature.h"
#include "luxxle/components/luxxle_news/common/features.h"
#include "luxxle/components/luxxle_rewards/core/features.h"
#include "luxxle/components/luxxle_search_conversion/features.h"
#include "luxxle/components/luxxle_shields/core/common/features.h"
#include "luxxle/components/luxxle_vpn/common/features.h"
#include "luxxle/components/luxxle_wallet/common/features.h"
#include "luxxle/components/debounce/core/common/features.h"
#include "luxxle/components/google_sign_in_permission/features.h"
#include "luxxle/components/playlist/common/features.h"
#include "luxxle/components/request_otr/common/features.h"
#include "luxxle/components/speedreader/common/features.h"
#include "luxxle/components/web_discovery/buildflags/buildflags.h"
#include "luxxle/components/webcompat/core/common/features.h"
#include "net/base/features.h"
#include "third_party/blink/public/common/features.h"

#define LUXXLE_AI_CHAT_FLAGS \
  &ai_chat::features::kAIChat, &ai_chat::features::kAIChatHistory,

#if BUILDFLAG(ENABLE_WEB_DISCOVERY_NATIVE)
#include "luxxle/components/web_discovery/common/features.h"
#define LUXXLE_WEB_DISCOVERY_FLAG \
  &web_discovery::features::kLuxxleWebDiscoveryNative,
#else
#define LUXXLE_WEB_DISCOVERY_FLAG
#endif

// clang-format off
#define kForceWebContentsDarkMode kForceWebContentsDarkMode,            \
    LUXXLE_AI_CHAT_FLAGS                                                 \
    LUXXLE_WEB_DISCOVERY_FLAG                                            \
    &luxxle_rewards::features::kLuxxleRewards,                            \
    &luxxle_search_conversion::features::kOmniboxBanner,                 \
    &luxxle_vpn::features::kLuxxleVPNLinkSubscriptionAndroidUI,           \
    &luxxle_wallet::features::kNativeLuxxleWalletFeature,                 \
    &playlist::features::kPlaylist,                                     \
    &download::features::kParallelDownloading,                          \
    &preferences::features::kLuxxleBackgroundVideoPlayback,              \
    &preferences::features::kLuxxlePictureInPictureForYouTubeVideos,     \
    &request_otr::features::kLuxxleRequestOTRTab,                        \
    &safe_browsing::features::kLuxxleAndroidSafeBrowsing,                \
    &speedreader::kSpeedreaderFeature,                                  \
    &debounce::features::kLuxxleDebounce,                                \
    &webcompat::features::kLuxxleWebcompatExceptionsService,             \
    &net::features::kLuxxleHttpsByDefault,                               \
    &net::features::kLuxxleFallbackDoHProvider,                          \
    &google_sign_in_permission::features::kLuxxleGoogleSignInPermission, \
    &net::features::kLuxxleForgetFirstPartyStorage,                      \
    &luxxle_shields::features::kLuxxleShowStrictFingerprintingMode,       \
    &luxxle_shields::features::kLuxxleLocalhostAccessPermission,          \
    &luxxle_shields::features::kBlockAllCookiesToggle,                   \
    &luxxle_shields::features::kLuxxleShieldsElementPicker,               \
    &features::kNewAndroidOnboarding,                                   \
    &luxxle_ads::kNewTabPageAdFeature

// clang-format on

#include "src/chrome/browser/flags/android/chrome_feature_list.cc"
#undef kForceWebContentsDarkMode
#undef LUXXLE_AI_CHAT_FLAGS
#undef LUXXLE_WEB_DISCOVERY_FLAG

namespace chrome {
namespace android {

OVERRIDE_FEATURE_DEFAULT_STATES({{
    {kMagicStackAndroid, base::FEATURE_DISABLED_BY_DEFAULT},
    {kAdaptiveButtonInTopToolbarCustomizationV2,
     base::FEATURE_DISABLED_BY_DEFAULT},
    {kClearBrowsingDataAndroidSurvey, base::FEATURE_DISABLED_BY_DEFAULT},
}});

}  // namespace android
}  // namespace chrome
