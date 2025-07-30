/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/side_panel/bookmarks/luxxle_bookmarks_side_panel_coordinator.h"

#include "base/functional/bind.h"
#include "luxxle/browser/ui/views/side_panel/luxxle_bookmarks_side_panel_view.h"
#include "chrome/browser/ui/views/side_panel/side_panel_entry.h"
#include "chrome/browser/ui/views/side_panel/side_panel_registry.h"

LuxxleBookmarksSidePanelCoordinator::~LuxxleBookmarksSidePanelCoordinator() =
    default;

void LuxxleBookmarksSidePanelCoordinator::CreateAndRegisterEntry(
    SidePanelRegistry* global_registry) {
  global_registry->Register(std::make_unique<SidePanelEntry>(
      SidePanelEntry::Id::kBookmarks,
      base::BindRepeating(
          &LuxxleBookmarksSidePanelCoordinator::CreateBookmarksPanelView,
          base::Unretained(this))));
}

std::unique_ptr<views::View>
LuxxleBookmarksSidePanelCoordinator::CreateBookmarksPanelView(
    SidePanelEntryScope& scope) {
  return std::make_unique<LuxxleBookmarksSidePanelView>(scope);
}
