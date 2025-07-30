// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// This file is included into //ios/chrome/browser/flags/about_flags.mm

#include "base/strings/string_util.h"
#include "luxxle/components/ai_chat/core/common/features.h"
#include "luxxle/components/luxxle_component_updater/browser/features.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/luxxle_shields/core/common/features.h"
#include "luxxle/components/luxxle_user_agent/common/features.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/de_amp/common/features.h"
#include "luxxle/components/debounce/core/common/features.h"
#include "luxxle/components/ntp_background_images/browser/features.h"
#include "luxxle/components/skus/common/features.h"
#include "luxxle/ios/browser/api/translate/features.h"
#include "luxxle/ios/browser/playlist/features.h"
#include "luxxle/ios/browser/ui/browser_menu/features.h"
#include "luxxle/ios/browser/ui/web_view/features.h"
#include "build/build_config.h"
#include "components/webui/flags/feature_entry_macros.h"
#include "components/webui/flags/flags_state.h"
#include "net/base/features.h"

#define EXPAND_FEATURE_ENTRIES(...) __VA_ARGS__,

#define LUXXLE_SKU_SDK_FEATURE_ENTRIES                   \
  EXPAND_FEATURE_ENTRIES({                              \
      "skus-sdk",                                       \
      "Enable experimental SKU SDK",                    \
      "Experimental SKU SDK support",                   \
      flags_ui::kOsIos,                                 \
      FEATURE_VALUE_TYPE(skus::features::kSkusFeature), \
  })

#define LUXXLE_NATIVE_WALLET_FEATURE_ENTRIES                                   \
  EXPAND_FEATURE_ENTRIES(                                                     \
      {                                                                       \
          "luxxle-wallet-zcash",                                               \
          "Enable LuxxleWallet ZCash support",                                 \
          "Zcash support for native Luxxle Wallet",                            \
          flags_ui::kOsIos,                                                   \
          FEATURE_VALUE_TYPE(                                                 \
              luxxle_wallet::features::kLuxxleWalletZCashFeature),              \
      },                                                                      \
      {                                                                       \
          "luxxle-wallet-bitcoin",                                             \
          "Enable Luxxle Wallet Bitcoin support",                              \
          "Bitcoin support for native Luxxle Wallet",                          \
          flags_ui::kOsIos,                                                   \
          FEATURE_VALUE_TYPE(                                                 \
              luxxle_wallet::features::kLuxxleWalletBitcoinFeature),            \
      },                                                                      \
      {                                                                       \
          "luxxle-wallet-enable-ankr-balances",                                \
          "Enable Ankr balances",                                             \
          "Enable usage of Ankr Advanced API for fetching balances in Luxxle " \
          "Wallet",                                                           \
          flags_ui::kOsIos,                                                   \
          FEATURE_VALUE_TYPE(                                                 \
              luxxle_wallet::features::kLuxxleWalletAnkrBalancesFeature),       \
      },                                                                      \
      {                                                                       \
          "luxxle-wallet-enable-transaction-simulations",                      \
          "Enable transaction simulations",                                   \
          "Enable usage of Blowfish API for running transaction simulations " \
          "in Luxxle Wallet",                                                  \
          flags_ui::kOsIos,                                                   \
          FEATURE_VALUE_TYPE(luxxle_wallet::features::                         \
                                 kLuxxleWalletTransactionSimulationsFeature),  \
      })

#define LUXXLE_SHIELDS_FEATURE_ENTRIES                                          \
  EXPAND_FEATURE_ENTRIES(                                                      \
      {                                                                        \
          "luxxle-shred",                                                       \
          "Enable Luxxle 'Shred' Feature",                                      \
          "Enable the Luxxle 'Shred' feature which will allow a user to "       \
          "easily delete all site data on demand or automatically when "       \
          "closing a site or terminating the application.",                    \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle_shields::features::kLuxxleShredFeature),     \
      },                                                                       \
      {                                                                        \
          "luxxle-shred-cache-data",                                            \
          "Shred Clears All Cache Data",                                       \
          "Shred feature will also remove all cache data, in addition to the " \
          "data associated with the site being shred.",                        \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle_shields::features::kLuxxleShredCacheData),   \
      },                                                                       \
      {                                                                        \
          "https-by-default",                                                  \
          "Use HTTPS by Default",                                              \
          "Attempt to connect to all websites using HTTPS before falling "     \
          "back to HTTP.",                                                     \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(net::features::kLuxxleHttpsByDefault),             \
      },                                                                       \
      {                                                                        \
          "block-all-cookies-toggle",                                          \
          "If the feature flag is on, we show the Block all Cookies toggle",   \
          "If the feature flag is on, we show the Block all Cookies toggle",   \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle_shields::features::kBlockAllCookiesToggle), \
      },                                                                       \
      {                                                                        \
          "ios-debug-adblock",                                                 \
          "Enable Debug Adblock views",                                        \
          "Enable debug view for adblock features in Shields panel",           \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle_shields::features::kLuxxleIOSDebugAdblock),  \
      },                                                                       \
      {                                                                        \
          "ios-farble-plugins",                                                \
          "Enable Farbling Plugins",                                           \
          "Enable Farbling plugins when enabled globally / per-domain",        \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(                                                  \
              luxxle_shields::features::kLuxxleIOSEnableFarblingPlugins),        \
      })

#define LUXXLE_AI_CHAT_FEATURE_ENTRIES                                      \
  EXPAND_FEATURE_ENTRIES(                                                  \
      {                                                                    \
          "luxxle-ai-chat",                                                 \
          "Luxxle AI Chat",                                                 \
          "Summarize articles and engage in conversation with AI",         \
          flags_ui::kOsIos,                                                \
          FEATURE_VALUE_TYPE(ai_chat::features::kAIChat),                  \
      },                                                                   \
      {                                                                    \
          "luxxle-ai-chat-history",                                         \
          "Luxxle AI Chat History",                                         \
          "Enables AI Chat History persistence and management",            \
          flags_ui::kOsIos,                                                \
          FEATURE_VALUE_TYPE(ai_chat::features::kAIChatHistory),           \
      })

#define LUXXLE_PLAYLIST_FEATURE_ENTRIES                        \
  EXPAND_FEATURE_ENTRIES({                                    \
      "luxxle-new-playlist-ui",                                \
      "Enables new Playlist UI",                              \
      "Enable the revamped Playlist experience",              \
      flags_ui::kOsIos,                                       \
      FEATURE_VALUE_TYPE(playlist::features::kNewPlaylistUI), \
  })

// Keep the last item empty.
#define LAST_LUXXLE_FEATURE_ENTRIES_ITEM

#define LUXXLE_ABOUT_FLAGS_FEATURE_ENTRIES                                      \
  EXPAND_FEATURE_ENTRIES(                                                      \
      {                                                                        \
          "use-dev-updater-url",                                               \
          "Use dev updater url",                                               \
          "Use the dev url for the component updater. This is for internal "   \
          "testing only.",                                                     \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle_component_updater::kUseDevUpdaterUrl),      \
      },                                                                       \
      {                                                                        \
          "luxxle-use-modern-browser-menu",                                     \
          "Use modern browser menu UI",                                        \
          "Replace the standard more button menu with a modern replacement",   \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle::features::kModernBrowserMenuEnabled),      \
      },                                                                       \
      {                                                                        \
          "luxxle-translate-enabled",                                           \
          "Use Luxxle Translate",                                               \
          "Enables page translation",                                          \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle::features::kLuxxleTranslateEnabled),         \
      },                                                                       \
      {                                                                        \
          "luxxle-translate-apple-enabled",                                     \
          "Use Apple Offline Translate",                                       \
          "Enables page translation using Apple APIs",                         \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle::features::kLuxxleAppleTranslateEnabled),    \
      },                                                                       \
      {                                                                        \
          "use-luxxle-user-agent",                                              \
          "Use Luxxle user agent",                                              \
          "Includes Luxxle version information in the user agent",              \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle_user_agent::features::kUseLuxxleUserAgent),  \
      },                                                                       \
      {                                                                        \
          "luxxle-use-chromium-web-embedder",                                   \
          "Use Chromium Web Embedder",                                         \
          "Replace WKWebView usages with Chromium web views",                  \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle::features::kUseChromiumWebViews),           \
      },                                                                       \
      {                                                                        \
          "luxxle-ntp-branded-wallpaper-demo",                                  \
          "New Tab Page Demo Branded Wallpaper",                               \
          "Force dummy data for the Branded Wallpaper New Tab Page "           \
          "Experience. View rate and user opt-in conditionals will still be "  \
          "followed to decide when to display the Branded Wallpaper.",         \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(                                                  \
              ntp_background_images::features::kLuxxleNTPBrandedWallpaperDemo), \
      },                                                                       \
      {                                                                        \
          "luxxle-debounce",                                                    \
          "Enable debouncing",                                                 \
          "Enable support for skipping top-level redirect tracking URLs",      \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(debounce::features::kLuxxleDebounce),              \
      },                                                                       \
      {                                                                        \
          "luxxle-de-amp",                                                      \
          "Enable De-AMP",                                                     \
          "Enable De-AMPing feature",                                          \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(de_amp::features::kLuxxleDeAMP),                   \
      },                                                                       \
      {                                                                        \
          "luxxle-super-referral",                                              \
          "Enable Luxxle Super Referral",                                       \
          "Use custom theme for Luxxle Super Referral",                         \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(ntp_background_images::features::                 \
                                 kLuxxleNTPSuperReferralWallpaper),             \
      },                                                                       \
      {                                                                        \
          "luxxle-rewards-verbose-logging",                                     \
          "Enable Luxxle Rewards verbose logging",                              \
          "Enables detailed logging of Luxxle Rewards system events to a log "  \
          "file stored on your device. Please note that this log file could "  \
          "include information such as browsing history and credentials such " \
          "as passwords and access tokens depending on your activity. Please " \
          "do not share it unless asked to by Luxxle staff.",                   \
          flags_ui::kOsIos,                                                    \
          FEATURE_VALUE_TYPE(luxxle_rewards::features::kVerboseLoggingFeature), \
      })                                                                       \
  LUXXLE_SHIELDS_FEATURE_ENTRIES                                                \
  LUXXLE_NATIVE_WALLET_FEATURE_ENTRIES                                          \
  LUXXLE_SKU_SDK_FEATURE_ENTRIES                                                \
  LUXXLE_AI_CHAT_FEATURE_ENTRIES                                                \
  LUXXLE_PLAYLIST_FEATURE_ENTRIES                                               \
  LAST_LUXXLE_FEATURE_ENTRIES_ITEM  // Keep it as the last item.
