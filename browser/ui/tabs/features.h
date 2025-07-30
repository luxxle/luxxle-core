/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_TABS_FEATURES_H_
#define LUXXLE_BROWSER_UI_TABS_FEATURES_H_

#include "base/feature_list.h"
namespace tabs::features {

#if BUILDFLAG(IS_LINUX)
// This flag controls the behavior of browser_default::kScrollEventChangesTab,
// which is true only when it's Linux.
BASE_DECLARE_FEATURE(kLuxxleChangeActiveTabOnScrollEvent);
#endif  // BUILDFLAG(IS_LINUX)

BASE_DECLARE_FEATURE(kLuxxleSharedPinnedTabs);

BASE_DECLARE_FEATURE(kLuxxleHorizontalTabsUpdate);

BASE_DECLARE_FEATURE(kLuxxleCompactHorizontalTabs);

BASE_DECLARE_FEATURE(kLuxxleVerticalTabScrollBar);

BASE_DECLARE_FEATURE(kLuxxleSplitView);

bool HorizontalTabsUpdateEnabled();
bool IsLuxxleSplitViewEnabled();

}  // namespace tabs::features

#endif  // LUXXLE_BROWSER_UI_TABS_FEATURES_H_
