/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_TABS_LUXXLE_BROWSER_TAB_STRIP_CONTROLLER_H_
#define LUXXLE_BROWSER_UI_VIEWS_TABS_LUXXLE_BROWSER_TAB_STRIP_CONTROLLER_H_

#include <memory>
#include <optional>

#include "chrome/browser/ui/views/tabs/browser_tab_strip_controller.h"

class LuxxleTabContextMenuContents;

class LuxxleBrowserTabStripController : public BrowserTabStripController {
 public:
  LuxxleBrowserTabStripController(TabStripModel* model,
                                 BrowserView* browser_view,
                                 std::unique_ptr<TabMenuModelFactory>
                                     menu_model_factory_override = nullptr);
  LuxxleBrowserTabStripController(const LuxxleBrowserTabStripController&) =
      delete;
  LuxxleBrowserTabStripController& operator=(
      const LuxxleBrowserTabStripController&) = delete;
  ~LuxxleBrowserTabStripController() override;

  const std::optional<int> GetModelIndexOf(Tab* tab);

  // BrowserTabStripController overrides:
  void ShowContextMenuForTab(Tab* tab,
                             const gfx::Point& p,
                             ui::mojom::MenuSourceType source_type) override;
  int GetIndexOfLastFocusedTabInSplit(int model_index) override;

 private:
  // If non-NULL it means we're showing a menu for the tab.
  std::unique_ptr<LuxxleTabContextMenuContents> context_menu_contents_;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_TABS_LUXXLE_BROWSER_TAB_STRIP_CONTROLLER_H_
