/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// This file is included into //chrome/browser/about_flags.cc.

#include <initializer_list>

#include "base/strings/string_util.h"
#include "luxxle/browser/ui/brave_ui_features.h"
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

#define BRAVE_MODULE_FILENAME_PATCH                                            \
  IF_BUILDFLAG(                                                                \
      IS_WIN,                                                                  \
      EXPAND_FEATURE_ENTRIES({                                                 \
          "brave-module-filename-patch",                                       \
          "Enable Module Filename patch",                                      \
          "Enables patching of executable's name from brave.exe to "           \
          "chrome.exe in sandboxed processes.",                                \
          kOsWin,                                                              \
          FEATURE_VALUE_TYPE(sandbox::policy::features::kModuleFileNamePatch), \
      }))

#define BRAVE_WORKAROUND_NEW_WINDOW_FLASH                                  \
  IF_BUILDFLAG(                                                            \
      IS_WIN,                                                              \
      EXPAND_FEATURE_ENTRIES({                                             \
          "brave-workaround-new-window-flash",                             \
          "Workaround a white flash on new window creation",               \
          "Enable workaround to prevent new windows being created with a " \
          "white background",                                              \
          kOsWin,                                                          \
          FEATURE_VALUE_TYPE(::features::kBraveWorkaroundNewWindowFlash),  \
      }))

#if !BUILDFLAG(IS_ANDROID)
#define BRAVE_COMMANDS_FEATURE_ENTRIES                                      \
  EXPAND_FEATURE_ENTRIES(                                                   \
      {                                                                     \
          "brave-commands",                                                 \
          "Brave Commands",                                                 \
          "Enable experimental page for viewing and executing commands in " \
          "Brave",                                                          \
          kOsWin | kOsMac | kOsLinux,                                       \
          FEATURE_VALUE_TYPE(features::kBraveCommands),                     \
      },                                                                    \
      {"brave-commands-omnibox", "Brave Commands in Omnibox",               \
       "Enable quick commands in the omnibox", kOsWin | kOsMac | kOsLinux,  \
       FEATURE_VALUE_TYPE(features::kBraveCommandsInOmnibox)})
#else
#define BRAVE_COMMANDS_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IS_LINUX)
#define BRAVE_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES               \
  EXPAND_FEATURE_ENTRIES({                                                    \
      "brave-change-active-tab-on-scroll-event",                              \
      "Change active tab on scroll event",                                    \
      "Change the active tab when scroll events occur on tab strip.",         \
      kOsLinux,                                                               \
      FEATURE_VALUE_TYPE(tabs::features::kBraveChangeActiveTabOnScrollEvent), \
  })
#else
#define BRAVE_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IS_ANDROID)
#define BRAVE_BACKGROUND_VIDEO_PLAYBACK_ANDROID                                \
  EXPAND_FEATURE_ENTRIES({                                                     \
      "brave-background-video-playback",                                       \
      "Background video playback",                                             \
      "Enables play audio from video in background when tab is not active or " \
      "device screen is turned off.",                                          \
      kOsAndroid,                                                              \
      FEATURE_VALUE_TYPE(                                                      \
          preferences::features::kBraveBackgroundVideoPlayback),               \
  })
#define BRAVE_SAFE_BROWSING_ANDROID                                           \
  EXPAND_FEATURE_ENTRIES({                                                    \
      "brave-safe-browsing",                                                  \
      "Safe Browsing",                                                        \
      "Enables Google Safe Browsing for determining whether a URL has been "  \
      "marked as a known threat.",                                            \
      kOsAndroid,                                                             \
      FEATURE_VALUE_TYPE(safe_browsing::features::kBraveAndroidSafeBrowsing), \
  })
#else
#define BRAVE_BACKGROUND_VIDEO_PLAYBACK_ANDROID
#define BRAVE_SAFE_BROWSING_ANDROID
#endif  // BUILDFLAG(IS_ANDROID)

#if !BUILDFLAG(IS_ANDROID)
#define BRAVE_TABS_FEATURE_ENTRIES                                         \
  EXPAND_FEATURE_ENTRIES(                                                  \
      {                                                                    \
          "brave-shared-pinned-tabs",                                      \
          "Shared pinned tab",                                             \
          "Pinned tabs are shared across windows",                         \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kBraveSharedPinnedTabs),      \
      },                                                                   \
      {                                                                    \
          "brave-horizontal-tabs-update",                                  \
          "Updated horizontal tabs design",                                \
          "Updates the look and feel or horizontal tabs",                  \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kBraveHorizontalTabsUpdate),  \
      },                                                                   \
      {                                                                    \
          "brave-compact-horizontal-tabs",                                 \
          "Compact horizontal tabs design",                                \
          "Reduces the height of horizontal tabs",                         \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kBraveCompactHorizontalTabs), \
      },                                                                   \
      {                                                                    \
          "brave-vertical-tab-scroll-bar",                                 \
          "Show scroll bar on vertical tab strip",                         \
          "Shows scroll bar on vertical tab strip when it overflows",      \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kBraveVerticalTabScrollBar),  \
      },                                                                   \
      {                                                                    \
          "brave-split-view",                                              \
          "Enable split view",                                             \
          "Enables split view",                                            \
          kOsWin | kOsMac | kOsLinux,                                      \
          FEATURE_VALUE_TYPE(tabs::features::kBraveSplitView),             \
      })
#else
#define BRAVE_TABS_FEATURE_ENTRIES
#endif

#if BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
#define BRAVE_MIDDLE_CLICK_AUTOSCROLL_FEATURE_ENTRY                      \
  EXPAND_FEATURE_ENTRIES({                                               \
      "middle-button-autoscroll",                                        \
      "Middle button autoscroll",                                        \
      "Enables autoscrolling when the middle mouse button is clicked",   \
      kOsMac | kOsLinux,                                                 \
      FEATURE_VALUE_TYPE(blink::features::kMiddleButtonClickAutoscroll), \
  })
#else
#define BRAVE_MIDDLE_CLICK_AUTOSCROLL_FEATURE_ENTRY
#endif

#define BRAVE_OMNIBOX_FEATURES                                                \
  EXPAND_FEATURE_ENTRIES(                                                     \
      {                                                                       \
          "brave-omnibox-tab-switch-by-default",                              \
          "Brave Tab Switch by Default",                                      \
          "Prefer switching to already open tabs, rather than navigating in " \
          "a "                                                                \
          "new tab",                                                          \
          kOsWin | kOsLinux | kOsMac,                                         \
          FEATURE_VALUE_TYPE(omnibox::kOmniboxTabSwitchByDefault),            \
      },                                                                      \
      {                                                                       \
          "brave-history-more-search-results",                                \
          "Brave More History",                                               \
          "Include more history in the omnibox search results",               \
          kOsWin | kOsLinux | kOsMac | kOsAndroid,                            \
          FEATURE_VALUE_TYPE(history::kHistoryMoreSearchResults),             \
      })

#if BUILDFLAG(IS_MAC) && BUILDFLAG(ENABLE_UPDATER)
#define BRAVE_UPDATER_FEATURE_ENTRIES                  \
  EXPAND_FEATURE_ENTRIES({                             \
      "brave-use-omaha4-alpha",                        \
      "Use Omaha 4 Alpha",                             \
      "Use the new automatic update system",           \
      kOsDesktop | kOsMac,                             \
      FEATURE_VALUE_TYPE(brave::kBraveUseOmaha4Alpha), \
  })
#else
#define BRAVE_UPDATER_FEATURE_ENTRIES
#endif

// Keep the last item empty.
#define LAST_BRAVE_FEATURE_ENTRIES_ITEM

#define BRAVE_ABOUT_FLAGS_FEATURE_ENTRIES                                      \
  EXPAND_FEATURE_ENTRIES(                                                      \
      {                                                                        \
          "brave-ntp-search-widget",                                           \
          "Brave Search Widget on the NTP",                                    \
          "Enables searching directly from the New Tab Page",                  \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(features::kBraveNtpSearchWidget),                 \
      },                                                                       \
      {                                                                        \
          "brave-ntp-refresh-enabled",                                         \
          "New Tab Page refresh",                                              \
          "Enables the refreshed version of the New Tab Page",                 \
          kOsDesktop,                                                          \
          FEATURE_VALUE_TYPE(features::kBraveNewTabPageRefreshEnabled),        \
      },                                                                       \
      {                                                                        \
          "brave-ephemeral-storage",                                           \
          "Enable Ephemeral Storage",                                          \
          "Use ephemeral storage for third-party frames",                      \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kBraveEphemeralStorage),           \
      },                                                                       \
      {                                                                        \
          "brave-ephemeral-storage-keep-alive",                                \
          "Ephemeral Storage Keep Alive",                                      \
          "Keep ephemeral storage partitions alive for a specified time "      \
          "after all tabs for that origin are closed",                         \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kBraveEphemeralStorageKeepAlive),  \
      },                                                                       \
      {                                                                        \
          "brave-first-party-ephemeral-storage",                               \
          "Enable First Party Ephemeral Storage",                              \
          "Enable support for First Party Ephemeral Storage using "            \
          "SESSION_ONLY cookie setting",                                       \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kBraveFirstPartyEphemeralStorage), \
      },                                                                       \
      {                                                                        \
          "brave-forget-first-party-storage",                                  \
          "Enable First Party Storage Cleanup support",                        \
          "Add cookie blocking mode which allows Brave to cleanup first "      \
          "party storage (Cookies, DOM Storage) on website close",             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kBraveForgetFirstPartyStorage),    \
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
          "brave-web-bluetooth-api",                                           \
          "Web Bluetooth API",                                                 \
          "Enables the Web Bluetooth API, giving websites access to "          \
          "Bluetooth devices",                                                 \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kBraveWebBluetoothAPI),          \
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
          "brave-block-screen-fingerprinting",                                 \
          "Block screen fingerprinting",                                       \
          "Prevents JavaScript and CSS from learning the user's screen "       \
          "dimensions or window position.",                                    \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(                                                  \
              blink::features::kBraveBlockScreenFingerprinting),               \
      },                                                                       \
      {                                                                        \
          "brave-tor-windows-https-only",                                      \
          "Use HTTPS-Only Mode in Private Windows with Tor",                   \
          "Prevents Private Windows with Tor from making any insecure HTTP "   \
          "connections without warning the user first.",                       \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kBraveTorWindowsHttpsOnly),        \
      },                                                                       \
      {                                                                        \
          "brave-round-time-stamps",                                           \
          "Round time stamps",                                                 \
          "Prevents JavaScript from getting access to high-resolution clocks " \
          "by rounding all DOMHighResTimeStamps to the nearest millisecond.",  \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kBraveRoundTimeStamps),          \
      },                                                                       \
      {                                                                        \
          "translate",                                                         \
          "Enable Chromium Translate feature",                                 \
          "Should be used with brave-translate-go, see the description here.", \
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
          "brave-copy-clean-link-by-default",                                  \
          "Override default copy hotkey with copy clean link",                 \
          "Sanitize url before copying, replaces default ctrl+c hotkey for "   \
          "url ",                                                              \
          kOsWin | kOsLinux | kOsMac,                                          \
          FEATURE_VALUE_TYPE(features::kBraveCopyCleanLinkByDefault),          \
      },                                                                       \
      {                                                                        \
          "brave-global-privacy-control-enabled",                              \
          "Enable Global Privacy Control",                                     \
          "Enable the Sec-GPC request header and the "                         \
          "navigator.globalPrivacyControl JS API",                             \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(blink::features::kBraveGlobalPrivacyControl),     \
      },                                                                       \
      {                                                                        \
          "https-by-default",                                                  \
          "Use HTTPS by Default",                                              \
          "Attempt to connect to all websites using HTTPS before falling "     \
          "back to HTTP.",                                                     \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kBraveHttpsByDefault),             \
      },                                                                       \
      {                                                                        \
          "fallback-dns-over-https",                                           \
          "Use a fallback DoH provider",                                       \
          "In Automatic DoH mode, use a fallback DoH provider if the current " \
          "provider doesn't offer Secure DNS.",                                \
          kOsAll,                                                              \
          FEATURE_VALUE_TYPE(net::features::kBraveFallbackDoHProvider),        \
      },                                                                       \
      {                                                                        \
          "brave-override-download-danger-level",                              \
          "Override download danger level",                                    \
          "Disables download warnings for files which are considered "         \
          "dangerous when Safe Browsing is disabled. Use at your own risks. "  \
          "Not recommended.",                                                  \
          kOsWin | kOsLinux | kOsMac,                                          \
          FEATURE_VALUE_TYPE(features::kBraveOverrideDownloadDangerLevel),     \
      },                                                                       \
      {                                                                        \
          "brave-web-view-rounded-corners",                                    \
          "Use rounded corners on main content areas",                         \
          "Renders the main content area and sidebar panel with rounded "      \
          "corners, padding, and a drop shadow",                               \
          kOsWin | kOsLinux | kOsMac,                                          \
          FEATURE_VALUE_TYPE(features::kBraveWebViewRoundedCorners),           \
      })                                                                       \
  BRAVE_MODULE_FILENAME_PATCH                                                  \
  BRAVE_COMMANDS_FEATURE_ENTRIES                                               \
  BRAVE_BACKGROUND_VIDEO_PLAYBACK_ANDROID                                      \
  BRAVE_SAFE_BROWSING_ANDROID                                                  \
  BRAVE_CHANGE_ACTIVE_TAB_ON_SCROLL_EVENT_FEATURE_ENTRIES                      \
  BRAVE_TABS_FEATURE_ENTRIES                                                   \
  BRAVE_OMNIBOX_FEATURES                                                       \
  BRAVE_MIDDLE_CLICK_AUTOSCROLL_FEATURE_ENTRY                                  \
  BRAVE_WORKAROUND_NEW_WINDOW_FLASH                                            \
  BRAVE_UPDATER_FEATURE_ENTRIES                                                \
  LAST_BRAVE_FEATURE_ENTRIES_ITEM  // Keep it as the last item.
namespace flags_ui {
namespace {

// Unused function to reference Brave feature entries for clang checks.
[[maybe_unused]] void UseBraveAboutFlags() {
  // These vars are declared in anonymous namespace in
  // //chrome/browser/about_flags.cc. We declare them here manually to
  // instantiate BRAVE_ABOUT_FLAGS_FEATURE_ENTRIES without errors.
  constexpr int kOsAll = 0;
  constexpr int kOsDesktop = 0;

  static_assert(
      std::initializer_list<FeatureEntry>{BRAVE_ABOUT_FLAGS_FEATURE_ENTRIES}
          .size());
}

}  // namespace
}  // namespace flags_ui
