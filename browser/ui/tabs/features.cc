/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/tabs/features.h"

#include "chrome/browser/ui/ui_features.h"

namespace tabs::features {

#if BUILDFLAG(IS_LINUX)
BASE_FEATURE(kLuxxleChangeActiveTabOnScrollEvent,
             "LuxxleChangeActiveTabOnScrollEvent",
             base::FEATURE_ENABLED_BY_DEFAULT);
#endif  // BUILDFLAG(IS_LINUX)

BASE_FEATURE(kLuxxleSharedPinnedTabs,
             "LuxxleSharedPinnedTabs",
             base::FEATURE_DISABLED_BY_DEFAULT);

BASE_FEATURE(kLuxxleHorizontalTabsUpdate,
             "LuxxleHorizontalTabsUpdate",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kLuxxleCompactHorizontalTabs,
             "LuxxleCompactHorizontalTabs",
             base::FEATURE_DISABLED_BY_DEFAULT);

BASE_FEATURE(kLuxxleVerticalTabScrollBar,
             "LuxxleVerticalTabScrollBar",
             base::FEATURE_DISABLED_BY_DEFAULT);

BASE_FEATURE(kLuxxleSplitView,
             "LuxxleSplitView",
             base::FEATURE_ENABLED_BY_DEFAULT);

bool HorizontalTabsUpdateEnabled() {
  return base::FeatureList::IsEnabled(kLuxxleHorizontalTabsUpdate);
}

bool IsLuxxleSplitViewEnabled() {
  if (!base::FeatureList::IsEnabled(tabs::features::kLuxxleSplitView)) {
    return false;
  }

  // Luxxle can't use both features together.
  // We'll migrate our SplitView feature onto upstream's SideBySide
  // feature.
  return !base::FeatureList::IsEnabled(::features::kSideBySide);
}

}  // namespace tabs::features
