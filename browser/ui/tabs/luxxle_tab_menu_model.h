/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_MENU_MODEL_H_
#define LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_MENU_MODEL_H_

#include <vector>

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/tabs/tab_menu_model.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"

namespace content {
class WebContents;
}  // namespace content

namespace sessions {
class TabRestoreService;
}  // namespace sessions

class Browser;

class LuxxleTabMenuModel : public TabMenuModel {
 public:
  enum LuxxleTabContextMenuCommand {
    CommandStart = TabStripModel::CommandLast,
    CommandRestoreTab,
    CommandBookmarkAllTabs,
    CommandShowVerticalTabs,
    CommandToggleTabMuted,
    CommandBringAllTabsToThisWindow,
    CommandCloseDuplicateTabs,
    CommandNewSplitView,
    CommandTileTabs,
    CommandBreakTile,
    CommandSwapTabsInTile,
    CommandLast,
  };

  LuxxleTabMenuModel(ui::SimpleMenuModel::Delegate* delegate,
                    TabMenuModelDelegate* tab_menu_model_delegate,
                    TabStripModel* tab_strip_model,
                    int index,
                    bool is_vertical_tab);
  LuxxleTabMenuModel(const LuxxleTabMenuModel&) = delete;
  LuxxleTabMenuModel& operator=(const LuxxleTabMenuModel&) = delete;
  ~LuxxleTabMenuModel() override;

  bool all_muted() const { return all_muted_; }

  // TabMenuModel:
  std::u16string GetLabelAt(size_t index) const override;

 private:
  void Build(Browser* browser,
             TabStripModel* tab_strip_model,
             const std::vector<int>& indices);
  void BuildItemsForSplitView(Browser* browser,
                              TabStripModel* tab_strip_model,
                              const std::vector<int>& indices);
  int GetRestoreTabCommandStringId() const;

  raw_ptr<content::WebContents> web_contents_ = nullptr;
  raw_ptr<sessions::TabRestoreService> restore_service_ = nullptr;
  bool all_muted_;

  bool is_vertical_tab_ = false;
};

#endif  // LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_MENU_MODEL_H_
