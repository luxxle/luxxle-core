/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/tabs/luxxle_tab_group_highlight.h"

#include "luxxle/browser/ui/tabs/luxxle_tab_layout_constants.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/ui/views/tabs/tab_group_views.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/gfx/geometry/skia_conversions.h"

LuxxleTabGroupHighlight::~LuxxleTabGroupHighlight() = default;

SkPath LuxxleTabGroupHighlight::GetPath() const {
  if (!tabs::utils::ShouldShowVerticalTabs(tab_group_views_->GetBrowser()) &&
      !tabs::features::HorizontalTabsUpdateEnabled()) {
    return TabGroupHighlight::GetPath();
  }

  return {};
}

BEGIN_METADATA(LuxxleTabGroupHighlight)
END_METADATA
