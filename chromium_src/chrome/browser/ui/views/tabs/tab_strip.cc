/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/views/tabs/tab_strip.h"

#include <cmath>

#include "luxxle/browser/ui/views/tabs/luxxle_compound_tab_container.h"
#include "luxxle/browser/ui/views/tabs/luxxle_tab.h"
#include "luxxle/browser/ui/views/tabs/luxxle_tab_group_header.h"
#include "luxxle/browser/ui/views/tabs/luxxle_tab_hover_card_controller.h"
#include "luxxle/browser/ui/views/tabs/vertical_tab_utils.h"
#include "chrome/browser/ui/ui_features.h"
#include "chrome/browser/ui/views/tabs/browser_tab_strip_controller.h"
#include "chrome/browser/ui/views/tabs/compound_tab_container.h"
#include "chrome/browser/ui/views/tabs/tab_container.h"
#include "chrome/browser/ui/views/tabs/tab_container_impl.h"

#if BUILDFLAG(IS_WIN)
#include "ui/gfx/win/hwnd_util.h"
#endif

// Overrides TabContainer::TabInsertionParams construction in
// TabStrip::AddTabsAt
#define param(TAB, MODEL_INDEX, PINNED) \
  param(std::make_unique<LuxxleTab>(this), MODEL_INDEX, PINNED)

#define CompoundTabContainer LuxxleCompoundTabContainer
#define TabContainerImpl LuxxleTabContainer
#define TabHoverCardController LuxxleTabHoverCardController

// Macros for TabDragContextImpl. TabDragContextImpl is in anonymous namespace
// and it's used in the tab_strip.cc file at the same time. Because of this,
// the class is really hard to be extended with inheritance, so using patch file
// seems to be the most efficient way for now. If we could split this into
// another file or child class, that'd be great.
#define LUXXLE_TAB_DRAG_CONTEXT_IMPL_CALCULATE_INSERTION_INDEX                \
  if (tabs::utils::ShouldShowVerticalTabs(tab_strip_->GetBrowser())) {       \
    tabs::UpdateInsertionIndexForVerticalTabs(                               \
        dragged_bounds, first_dragged_tab_index, num_dragged_tabs,           \
        GetTabAt(first_dragged_tab_index)->group().has_value(),              \
        candidate_index, tab_strip_->controller_.get(),                      \
        &tab_strip_->tab_container_.get(), min_distance, min_distance_index, \
        tab_strip_);                                                         \
    continue;                                                                \
  }

#define LUXXLE_TAB_DRAG_CONTEXT_IMPL_CALCULATE_BOUNDS_FOR_DRAGGED_VIEWS      \
  if (tabs::utils::ShouldShowVerticalTabs(tab_strip_->GetBrowser())) {      \
    return tabs::CalculateBoundsForVerticalDraggedViews(views, tab_strip_); \
  }

#include "src/chrome/browser/ui/views/tabs/tab_strip.cc"

#undef LUXXLE_TAB_DRAG_CONTEXT_IMPL_CALCULATE_BOUNDS_FOR_DRAGGED_VIEWS
#undef LUXXLE_TAB_DRAG_CONTEXT_IMPL_CALCULATE_INSERTION_INDEX
#undef TabHoverCardController
#undef CompoundTabContainer
#undef TabContainerImpl
#undef param

bool TabStrip::IsTabTiled(const Tab* tab) const {
  return false;
}

bool TabStrip::IsFirstTabInTile(const Tab* tab) const {
  return false;
}
