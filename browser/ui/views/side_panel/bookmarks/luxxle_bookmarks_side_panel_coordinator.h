/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_BOOKMARKS_LUXXLE_BOOKMARKS_SIDE_PANEL_COORDINATOR_H_
#define LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_BOOKMARKS_LUXXLE_BOOKMARKS_SIDE_PANEL_COORDINATOR_H_

#include <memory>

#include "chrome/browser/ui/views/side_panel/bookmarks/bookmarks_side_panel_coordinator.h"

class SidePanelEntryScope;
class SidePanelRegistry;

namespace views {
class View;
}  // namespace views

// Introduced to give custom contents view(LuxxleBookmarksSidePanelView) for
// bookmarks panel entry. That contents view includes bookmarks panel specific
// header view and web view.
// BookmarksSidePanelCoordinator::CreateBookmarksWebView() is reused from
// LuxxleBookmarksSidePanelView. That's why LuxxleBookmarksSidePanelView is set
// as BookmarksSidePanelCoordinator's friend.
class LuxxleBookmarksSidePanelCoordinator
    : public BookmarksSidePanelCoordinator {
 public:
  using BookmarksSidePanelCoordinator::BookmarksSidePanelCoordinator;
  ~LuxxleBookmarksSidePanelCoordinator() override;

  void CreateAndRegisterEntry(SidePanelRegistry* global_registry) override;

 private:
  std::unique_ptr<views::View> CreateBookmarksPanelView(
      SidePanelEntryScope& scope);
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_BOOKMARKS_LUXXLE_BOOKMARKS_SIDE_PANEL_COORDINATOR_H_
