/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_BOOKMARKS_SIDE_PANEL_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_BOOKMARKS_SIDE_PANEL_VIEW_H_

#include "luxxle/browser/ui/views/side_panel/luxxle_side_panel_view_base.h"

class SidePanelEntryScope;

// Gives bookmarks panel specific header view with web view.
class LuxxleBookmarksSidePanelView : public LuxxleSidePanelViewBase {
 public:
  explicit LuxxleBookmarksSidePanelView(SidePanelEntryScope& scope);
  ~LuxxleBookmarksSidePanelView() override;
  LuxxleBookmarksSidePanelView(const LuxxleBookmarksSidePanelView&) = delete;
  LuxxleBookmarksSidePanelView& operator=(const LuxxleBookmarksSidePanelView&) =
      delete;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_BOOKMARKS_SIDE_PANEL_VIEW_H_
