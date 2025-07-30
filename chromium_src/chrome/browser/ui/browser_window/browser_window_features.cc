/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/browser_window/public/browser_window_features.h"

#include "luxxle/browser/ui/views/side_panel/bookmarks/luxxle_bookmarks_side_panel_coordinator.h"
#include "luxxle/browser/ui/views/side_panel/luxxle_side_panel_coordinator.h"
#include "chrome/browser/ui/views/side_panel/bookmarks/bookmarks_side_panel_coordinator.h"

#define BrowserWindowFeatures BrowserWindowFeatures_ChromiumImpl
#define SidePanelCoordinator LuxxleSidePanelCoordinator
#define BookmarksSidePanelCoordinator LuxxleBookmarksSidePanelCoordinator

#include "src/chrome/browser/ui/browser_window/browser_window_features.cc"

#undef BookmarksSidePanelCoordinator
#undef SidePanelCoordinator
#undef BrowserWindowFeatures
