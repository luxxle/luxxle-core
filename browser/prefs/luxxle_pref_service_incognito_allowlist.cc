/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/prefs/luxxle_pref_service_incognito_allowlist.h"

#include <array>

#include "base/containers/span.h"
#include "base/strings/cstring_view.h"
#include "luxxle/browser/ui/bookmark/luxxle_bookmark_prefs.h"
#include "luxxle/components/ai_chat/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/constants/pref_names.h"
#include "build/build_config.h"
#include "chrome/common/pref_names.h"

#if !BUILDFLAG(IS_ANDROID)
#include "luxxle/browser/ui/tabs/luxxle_tab_prefs.h"
#endif

#if defined(TOOLKIT_VIEWS)
#include "luxxle/components/sidebar/browser/pref_names.h"
#endif

namespace luxxle {

base::span<const base::cstring_view> GetLuxxlePersistentPrefNames() {
  static constexpr auto kAllowlist = std::to_array<base::cstring_view>({
      kLuxxleAutofillPrivateWindows,
#if !BUILDFLAG(IS_ANDROID)
      kShowWalletIconOnToolbar,
      prefs::kSidePanelHorizontalAlignment,
      kTabMuteIndicatorNotClickable,
      luxxle_tabs::kVerticalTabsExpandedWidth,
      luxxle_tabs::kVerticalTabsEnabled,
      luxxle_tabs::kVerticalTabsCollapsed,
      luxxle_tabs::kVerticalTabsFloatingEnabled,
      luxxle_tabs::kVerticalTabsShowTitleOnWindow,
      luxxle_tabs::kVerticalTabsOnRight,
      luxxle_tabs::kVerticalTabsShowScrollbar,
      luxxle_tabs::kSharedPinnedTab,
#endif
#if defined(TOOLKIT_VIEWS)
      sidebar::kSidePanelWidth,
#endif
      ai_chat::prefs::kLastAcceptedDisclaimer,
      ai_chat::prefs::kLuxxleChatAutocompleteProviderEnabled,
      luxxle::bookmarks::prefs::kShowAllBookmarksButton,
  });

  return kAllowlist;
}

}  // namespace luxxle
