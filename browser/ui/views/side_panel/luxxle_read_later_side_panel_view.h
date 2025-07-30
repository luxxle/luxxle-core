/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_READ_LATER_SIDE_PANEL_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_READ_LATER_SIDE_PANEL_VIEW_H_

#include "base/functional/callback_forward.h"
#include "luxxle/browser/ui/views/side_panel/luxxle_side_panel_view_base.h"
#include "ui/base/metadata/metadata_header_macros.h"

class Browser;
class SidePanelEntryScope;

// Gives reading list specific header view with web view.
class LuxxleReadLaterSidePanelView : public LuxxleSidePanelViewBase {
  METADATA_HEADER(LuxxleReadLaterSidePanelView, LuxxleSidePanelViewBase)

 public:
  LuxxleReadLaterSidePanelView(Browser* browser,
                              SidePanelEntryScope& scope,
                              base::RepeatingClosure close_cb);
  ~LuxxleReadLaterSidePanelView() override;
  LuxxleReadLaterSidePanelView(const LuxxleReadLaterSidePanelView&) = delete;
  LuxxleReadLaterSidePanelView& operator=(const LuxxleReadLaterSidePanelView&) =
      delete;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_READ_LATER_SIDE_PANEL_VIEW_H_
