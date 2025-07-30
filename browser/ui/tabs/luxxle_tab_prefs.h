/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_PREFS_H_
#define LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_PREFS_H_

class PrefRegistrySimple;
class PrefService;

namespace luxxle_tabs {

enum TabHoverMode { TOOLTIP = 0, CARD = 1, CARD_WITH_PREVIEW = 2 };

inline constexpr char kTabHoverMode[] = "luxxle.tabs.hover_mode";

inline constexpr char kVerticalTabsEnabled[] =
    "luxxle.tabs.vertical_tabs_enabled";
inline constexpr char kVerticalTabsCollapsed[] =
    "luxxle.tabs.vertical_tabs_collapsed";
inline constexpr char kVerticalTabsExpandedStatePerWindow[] =
    "luxxle.tabs.vertical_tabs_expanded_state_per_window";
inline constexpr char kVerticalTabsShowTitleOnWindow[] =
    "luxxle.tabs.vertical_tabs_show_title_on_window";
inline constexpr char kVerticalTabsFloatingEnabled[] =
    "luxxle.tabs.vertical_tabs_floating_enabled";
inline constexpr char kVerticalTabsExpandedWidth[] =
    "luxxle.tabs.vertical_tabs_expanded_width";
inline constexpr char kVerticalTabsOnRight[] =
    "luxxle.tabs.vertical_tabs_on_right";
inline constexpr char kVerticalTabsShowScrollbar[] =
    "luxxle.tabs.vertical_tabs_show_scrollbar";

inline constexpr char kSharedPinnedTab[] = "luxxle.tabs.shared_pinned_tab";

void RegisterLuxxleProfilePrefs(PrefRegistrySimple* registry);
void MigrateLuxxleProfilePrefs(PrefService* prefs);

bool AreTooltipsEnabled(PrefService* prefs);
bool AreCardPreviewsEnabled(PrefService* prefs);

}  // namespace luxxle_tabs

#endif  // LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_PREFS_H_
