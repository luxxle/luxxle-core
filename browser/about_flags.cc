/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// This file is included into //chrome/browser/about_flags.cc.

#include <initializer_list>

#include "base/strings/string_util.h"
#include "luxxle/browser/ui/luxxle_ui_features.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "build/build_config.h"
#include "chrome/browser/buildflags.h"
#include "chrome/browser/ui/ui_features.h"
#include "components/content_settings/core/common/features.h"
#include "components/history/core/browser/features.h"
#include "components/omnibox/common/omnibox_features.h"
#include "components/translate/core/browser/translate_prefs.h"
#include "components/webui/flags/feature_entry.h"
#include "components/webui/flags/feature_entry_macros.h"
#include "components/webui/flags/flags_state.h"
#include "net/base/features.h"
#include "third_party/blink/public/common/features.h"

#if BUILDFLAG(IS_ANDROID)
#include "luxxle/browser/android/safe_browsing/features.h"
#include "luxxle/browser/android/youtube_script_injector/features.h"
#endif

#if BUILDFLAG(IS_WIN)
#include "sandbox/policy/features.h"
#endif

#if BUILDFLAG(IS_MAC) && BUILDFLAG(ENABLE_UPDATER)
#include "luxxle/browser/mac_features.h"
#endif

#define EXPAND_FEATURE_ENTRIES(...) __VA_ARGS__,

#define LUXXLE_MODULE_FILENAME_PATCH                                            \
  IF_BUILDFLAG(                                                                \
      IS_WIN,                                                                  \
      EXPAND_FEATURE_ENTRIES({                                                 \
          "luxxle-module-filename-patch",                                       \
          "Enable Module Filename patch",                                      \
          "Enables patching of executable's name from luxxle.exe to "           \
          "chrome.exe in sandboxed processes.",                                \
          kOsWin,                                                              \
          FEATURE_VALUE_TYPE(sandbox::policy::features::kModuleFileNamePatch), \
      }))

#define LUXXLE_WORKAROUND_NEW_WINDOW_FLASH                                  \
  IF_BUILDFLAG(                                                            \
      IS_WIN,                                                              \
      EXPAND_FEATURE_ENTRIES({                                             \
          "luxxle-workaround-new-window-flash",                             \
          "Workaround a white flash on new window creation",               \
          "Enable workaround to prevent new windows being created with a " \
          "white background",                                              \
          kOsWin,                                                          \
          FEATURE_VALUE_TYPE(::features::kLuxxleWorkaroundNewWindowFlash),  \
      }))

#if !BUILDFLAG(IS_ANDROID)
#define LUXXLE_COMMANDS_FEATURE_ENTRIES                                      \
  EXPAND_FEATURE_ENTRIES(                                                   \
      {                                                                     \
          "luxxle-commands",                                                 \
          "Luxxle Commands",                                                 \
          "Enable experimental page for viewing and executing commands in " \
          "Luxxle",                                                          \
          kOsWin | kOsMac | kOsLinux,                                       \
          FEATURE_VALUE_TYPE(features::kLuxxleCommands),                     \
      },                                                                    \
      {"luxxle-commands-omnibox", "Luxxle Commands in Omnibox",               \
       "Enable quick commands in the omnibox", kOsWin | kOsMac | kOsLinux,  \
       FEATURE_VALUE_TYPE(features::kLuxxleCommandsInOmnibox)})
#else
#define LUXXLE_COMMANDS_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IS_LINUX)
#define LUXXLE_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES               \
  EXPAND_FEATURE_ENTRIES({                                                    \
      "luxxle-change-active-tab-on-scroll-event",                              \
      "Change active tab on scroll event",                                    \
      "Change the active tab when scroll events occur on tab strip.",         \
      kOsLinux,                                                               \
      FEATURE_VALUE_TYPE(tabs::features::kLuxxleChangeActiveTabOnScrollEvent), \
  })
#else
#define LUXXLE_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IS_ANDROID)
#define LUXXLE_BACKGROUND_VIDEO_PLAYBACK_ANDROID                                \
  EXPAND_FEATURE_ENTRIES({                                                     \
      "luxxle-background-video-playback",                                       \
      "Background video playback",                                             \
      "Enables play audio from video in background when tab is not active or " \
      "device screen is turned off.",                                          \
      kOsAndroid,                                                              \
      FEATURE_VALUE_TYPE(                                                      \
          preferences::features::kLuxxleBackgroundVideoPlayback),               \
  })
#define LUXXLE_SAFE_BROWSING_ANDROID                                           \
  EXPAND_FEATURE_ENTRIES({                                                    \
      "luxxle-safe-browsing",                                                  \
      "Safe Browsing",                                                        \
      "Enables Google Safe Browsing for determining whether a URL has been "  \
      "marked as a known threat.",                                            \
      kOsAndroid,                                                             \
      FEATURE_VALUE_TYPE(safe_browsing::features::kLuxxleAndroidSafeBrowsing), \
  })
#else
#define LUXXLE_BACKGROUND_VIDEO_PLAYBACK_ANDROID
#define LUXXLE_SAFE_BROWSING_ANDROID
#endif  // BUILDFLAG(IS_ANDROID)

#if !BUILDFLAG(IS_ANDROID)
#define LUXXLE_TABS_FEATURE_ENTRIES                                         \
  EXPAND_FEATURE_ENTRIES(                                                  \
      {                                                                    \
          "luxxle-shared-pinned-tabs",                                      \
          "Shared pinned tab",                                             \
          "Pinned tabs are shared across windows",                         \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kLuxxleSharedPinnedTabs),      \
      },                                                                   \
      {                                                                    \
          "luxxle-horizontal-tabs-update",                                  \
          "Updated horizontal tabs design",                                \
          "Updates the look and feel or horizontal tabs",                  \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kLuxxleHorizontalTabsUpdate),  \
      },                                                                   \
      {                                                                    \
          "luxxle-compact-horizontal-tabs",                                 \
          "Compact horizontal tabs design",                                \
          "Reduces the height of horizontal tabs",                         \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kLuxxleCompactHorizontalTabs), \
      },                                                                   \
      {                                                                    \
          "luxxle-vertical-tab-scroll-bar",                                 \
          "Show scroll bar on vertical tab strip",                         \
          "Shows scroll bar on vertical tab strip when it overflows",      \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kLuxxleVerticalTabScrollBar),  \
      },                                                                   \
      {                                                                    \
          "luxxle-split-view",                                              \
          "Enable split view",                                             \
          "Enables split view",                                            \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kLuxxleSplitView),             \
      })
#else
#define LUXXLE_TABS_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
#define LUXXLE_MIDDLE_CLICK_AUTOSCROLL_FEATURE_ENTRY                      \
  EXPAND_FEATURE_ENTRIES({                                               \
      "middle-button-autoscroll",                                        \
      "Middle button autoscroll",                                        \
      "Enables autoscrolling when the middle mouse button is clicked",   \
      kOsMac | kOsLinux,                                                 \
      FEATURE_VALUE_TYPE(blink::features::kMiddleButtonClickAutoscroll), \
  })
#else
#define LUXXLE_MIDDLE_CLICK_AUTOSCROLL_FEATURE_ENTRY
#endif

#define LUXXLE_OMNIBOX_FEATURES                                                \
  EXPAND_FEATURE_ENTRIES(                                                     \
      {                                                                       \
          "luxxle-omnibox-tab-switch-by-default",                              \
          "Luxxle Tab Switch by Default",                                      \
          "Prefer switching to already open tabs, rather than navigating in " \
          "a "                                                                \
          "new tab",                                                          \
          kOsWin | kOsLinux | kOsMac,                                         \
          FEATURE_VALUE_TYPE(omnibox::kOmniboxTabSwitchByDefault),            \
      },                                                                      \
      {                                                                       \
          "luxxle-history-more-search-results",                                \
          "Luxxle More History",                                               \
          "Include more history in the omnibox search results",               \
          kOsWin | kOsLinux | kOsMac | kOsAndroid,                            \
          FEATURE_VALUE_TYPE(history::kHistoryMoreSearchResults),             \
      })

#if BUILDFLAG(IS_MAC) && BUILDFLAG(ENABLE_UPDATER)
#define LUXXLE_UPDATER_FEATURE_ENTRIES                  \
  EXPAND_FEATURE_ENTRIES({                             \
      "luxxle-use-omaha4-alpha",                        \
      "Use Omaha 4 Alpha",                             \
      "Use the new automatic update system",           \
      kOsDesktop | kOsMac,                             \
      FEATURE_VALUE_TYPE(luxxle::kLuxxleUseOmaha4Alpha), \
  })
#else
#define LUXXLE_UPDATER_FEATURE_ENTRIES
#endif

// Keep the last item empty.
#define LAST_LUXXLE_FEATURE_ENTRIES_ITEM

#define LUXXLE_ABOUT_FLAGS_FEATURE_ENTRIES                                      \
  EXPAND_FEATURE_ENTRIES(                                                      \
      {                                                                        \
          "luxxle-ntp-search-widget",                                           \
          "Luxxle Search Widget on the NTP",                                    \
          "Enables searching directly from the New Tab Page",                  \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(features::kLuxxleNtpSearchWidget),                 \
      },                                                                       \
      {                                                                        \
          "luxxle-ntp-refresh-enabled",                                         \
          "New Tab Page refresh",                                              \
          "Enables the refreshed version of the New Tab Page",                 \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(features::kLuxxleNewTabPageRefreshEnabled),        \
      },                                                                       \
      {                                                                        \
          "luxxle-ephemeral-storage",                                           \
          "Enable Ephemeral Storage",                                          \
          "Use ephemeral storage for third-party frames",                      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kLuxxleEphemeralStorage),           \
      },                                                                       \
      {                                                                        \
          "luxxle-ephemeral-storage-keep-alive",                                \
          "Ephemeral Storage Keep Alive",                                      \
          "Keep ephemeral storage partitions alive for a specified time "      \
          "after all tabs for that origin are closed",                         \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kLuxxleEphemeralStorageKeepAlive),  \
      },                                                                       \
      {                                                                        \
          "luxxle-first-party-ephemeral-storage",                               \
          "Enable First Party Ephemeral Storage",                              \
          "Enable support for First Party Ephemeral Storage using "            \
          "SESSION_ONLY cookie setting",                                       \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kLuxxleFirstPartyEphemeralStorage), \
      },                                                                       \
      {                                                                        \
          "luxxle-forget-first-party-storage",                                  \
          "Enable First Party Storage Cleanup support",                        \
          "Add cookie blocking mode which allows Luxxle to cleanup first "      \
          "party storage (Cookies, DOM Storage) on website close",             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kLuxxleForgetFirstPartyStorage),    \
      },                                                                       \
      {                                                                        \
          "file-system-access-api",                                            \
          "File System Access API",                                            \
          "Enables the File System Access API, giving websites access to the " \
          "file system",                                                       \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(blink::features::kFileSystemAccessAPI),           \
      },                                                                       \
      {                                                                        \
          "luxxle-web-bluetooth-api",                                           \
          "Web Bluetooth API",                                                 \
          "Enables the Web Bluetooth API, giving websites access to "          \
          "Bluetooth devices",                                                 \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kLuxxleWebBluetoothAPI),          \
      },                                                                       \
      {                                                                        \
          "navigator-connection-attribute",                                    \
          "Enable navigator.connection attribute",                             \
          "Enables the navigator.connection API. Enabling this API will "      \
          "allow sites to learn information about your network and internet "  \
          "connection. Trackers can use this information to fingerprint your " \
          "browser, or to infer when you are traveling or at home.",           \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kNavigatorConnectionAttribute),  \
      },                                                                       \
      {                                                                        \
          "restrict-websockets-pool",                                          \
          "Restrict WebSockets pool",                                          \
          "Limits simultaneous active WebSockets connections per eTLD+1",      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kRestrictWebSocketsPool),        \
      },                                                                       \
      {                                                                        \
          "allow-incognito-permission-inheritance",                            \
          "Allow permission inheritance in incognito profiles",                \
          "When enabled, most permissions set in a normal profile will be "    \
          "inherited in incognito profile if they are less permissive, for "   \
          "ex. Geolocation BLOCK will be automatically set to BLOCK in "       \
          "incognito.",                                                        \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              content_settings::kAllowIncognitoPermissionInheritance),         \
      },                                                                       \
      {                                                                        \
          "luxxle-block-screen-fingerprinting",                                 \
          "Block screen fingerprinting",                                       \
          "Prevents JavaScript and CSS from learning the user's screen "       \
          "dimensions or window position.",                                    \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              blink::features::kLuxxleBlockScreenFingerprinting),               \
      },                                                                       \
      {                                                                        \
          "luxxle-tor-windows-https-only",                                      \
          "Use HTTPS-Only Mode in Private Windows with Tor",                   \
          "Prevents Private Windows with Tor from making any insecure HTTP "   \
          "connections without warning the user first.",                       \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kLuxxleTorWindowsHttpsOnly),        \
      },                                                                       \
      {                                                                        \
          "luxxle-round-time-stamps",                                           \
          "Round time stamps",                                                 \
          "Prevents JavaScript from getting access to high-resolution clocks " \
          "by rounding all DOMHighResTimeStamps to the nearest millisecond.",  \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kLuxxleRoundTimeStamps),          \
      },                                                                       \
      {                                                                        \
          "translate",                                                         \
          "Enable Chromium Translate feature",                                 \
          "Should be used with luxxle-translate-go, see the description here.", \
          kOsDesktop | kOsAndroid,                                             \
          FEATURE_VALUE_TYPE(translate::kTranslate),                           \
      },                                                                       \
      {                                                                        \
          "restrict-event-source-pool",                                        \
          "Restrict Event Source Pool",                                        \
          "Limits simultaneous active WebSockets connections per eTLD+1",      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kRestrictEventSourcePool),       \
      },                                                                       \
      {                                                                        \
          "luxxle-copy-clean-link-by-default",                                  \
          "Override default copy hotkey with copy clean link",                 \
          "Sanitize url before copying, replaces default ctrl+c hotkey for "   \
          "url ",                                                              \
          kOsWin | kOsLinux | kOsMac,                                          \
          FEATURE_VALUE_TYPE(features::kLuxxleCopyCleanLinkByDefault),          \
      },                                                                       \
      {                                                                        \
          "luxxle-global-privacy-control-enabled",                              \
          "Enable Global Privacy Control",                                     \
          "Enable the Sec-GPC request header and the "                         \
          "navigator.globalPrivacyControl JS API",                             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kLuxxleGlobalPrivacyControl),     \
      },                                                                       \
      {                                                                        \
          "https-by-default",                                                  \
          "Use HTTPS by Default",                                              \
          "Attempt to connect to all websites using HTTPS before falling "     \
          "back to HTTP.",                                                     \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kLuxxleHttpsByDefault),             \
      },                                                                       \
      {                                                                        \
          "fallback-dns-over-https",                                           \
          "Use a fallback DoH provider",                                       \
          "In Automatic DoH mode, use a fallback DoH provider if the current " \
          "provider doesn't offer Secure DNS.",                                \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kLuxxleFallbackDoHProvider),        \
      },                                                                       \
      {                                                                        \
          "luxxle-override-download-danger-level",                              \
          "Override download danger level",                                    \
          "Disables download warnings for files which are considered "         \
          "dangerous when Safe Browsing is disabled. Use at your own risks. "  \
          "Not recommended.",                                                  \
          kOsWin | kOsLinux | kOsMac,                                          \
          FEATURE_VALUE_TYPE(features::kLuxxleOverrideDownloadDangerLevel),     \
      },                                                                       \
      {                                                                        \
          "luxxle-web-view-rounded-corners",                                    \
          "Use rounded corners on main content areas",                         \
          "Renders the main content area and sidebar panel with rounded "      \
          "corners, padding, and a drop shadow",                               \
          kOsWin | kOsLinux | kOsMac,                                          \
          FEATURE_VALUE_TYPE(features::kLuxxleWebViewRoundedCorners),           \
      })                                                                       \
  LUXXLE_MODULE_FILENAME_PATCH                                                  \
  LUXXLE_COMMANDS_FEATURE_ENTRIES                                               \
  LUXXLE_BACKGROUND_VIDEO_PLAYBACK_ANDROID                                      \
  LUXXLE_SAFE_BROWSING_ANDROID                                                  \
  LUXXLE_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES                      \
  LUXXLE_TABS_FEATURE_ENTRIES                                                   \
  LUXXLE_OMNIBOX_FEATURES                                                       \
  LUXXLE_MIDDLE_CLICK_AUTOSCROLL_FEATURE_ENTRY                                  \
  LUXXLE_WORKAROUND_NEW_WINDOW_FLASH                                            \
  LUXXLE_UPDATER_FEATURE_ENTRIES                                                \
  LAST_LUXXLE_FEATURE_ENTRIES_ITEM  // Keep it as the last item.
namespace flags_ui {
namespace {

// Unused function to reference Luxxle feature entries for clang checks.
[[maybe_unused]] void UseLuxxleAboutFlags() {
  // These vars are declared in anonymous namespace in
  // //chrome/browser/about_flags.cc. We declare them here manually to
  // instantiate LUXXLE_ABOUT_FLAGS_FEATURE_ENTRIES without errors.
  constexpr int kOsAll = 0;
  constexpr int kOsDesktop = 0;

  static_assert(
      std::initializer_list<FeatureEntry>{LUXXLE_ABOUT_FLAGS_FEATURE_ENTRIES}
          .size());
}

}  // namespace
}  // namespace flags_ui
