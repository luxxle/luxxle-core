/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_TABS_LUXXLE_TAB_CONTEXT_MENU_CONTENTS_H_
#define LUXXLE_BROWSER_UI_VIEWS_TABS_LUXXLE_TAB_CONTEXT_MENU_CONTENTS_H_

#include <memory>
#include <vector>

#include "base/gtest_prod_util.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "ui/base/mojom/menu_source_type.mojom.h"
#include "ui/base/ui_base_types.h"
#include "ui/menus/simple_menu_model.h"

class LuxxleBrowserTabStripController;
class LuxxleTabMenuModel;
class Browser;
class Tab;

namespace gfx {
class Point;
}  // namespace gfx

namespace sessions {
class TabRestoreService;
}  // namespace sessions

namespace views {
class MenuRunner;
}  // namespace views

class LuxxleTabContextMenuContents : public ui::SimpleMenuModel::Delegate {
 public:
  LuxxleTabContextMenuContents(Tab* tab,
                              LuxxleBrowserTabStripController* controller,
                              int index);
  LuxxleTabContextMenuContents(const LuxxleTabContextMenuContents&) = delete;
  LuxxleTabContextMenuContents& operator=(const LuxxleTabContextMenuContents&) =
      delete;
  ~LuxxleTabContextMenuContents() override;

  void Cancel();

  void RunMenuAt(const gfx::Point& point,
                 ui::mojom::MenuSourceType source_type);

  // ui::SimpleMenuModel::Delegate overrides:
  bool IsCommandIdChecked(int command_id) const override;
  bool IsCommandIdEnabled(int command_id) const override;
  bool IsCommandIdVisible(int command_id) const override;
  bool GetAcceleratorForCommandId(int command_id,
                                  ui::Accelerator* accelerator) const override;
  void ExecuteCommand(int command_id, int event_flags) override;

 private:
  FRIEND_TEST_ALL_PREFIXES(VerticalTabStripStringBrowserTest,
                           ContextMenuString);

  bool IsLuxxleCommandIdEnabled(int command_id) const;
  void ExecuteLuxxleCommand(int command_id);
  void BringAllTabsToThisWindow();

  bool IsLuxxleCommandId(int command_id) const;
  bool IsValidContextMenu() const;
  void OnMenuClosed();

  void NewSplitView();
  void TileSelectedTabs();
  void BreakSelectedTile();
  void SwapTabsInTile();

  std::vector<int> GetTabIndicesForSplitViewCommand() const;

  std::unique_ptr<LuxxleTabMenuModel> model_;
  std::unique_ptr<views::MenuRunner> menu_runner_;

  raw_ptr<Tab> tab_ = nullptr;
  int tab_index_ = -1;

  // true when menu is closed.
  // If it's set to true, this instance will not be used anymore because
  // new instance is created when showing context menu.
  bool menu_closed_ = false;
  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<sessions::TabRestoreService> restore_service_ = nullptr;
  raw_ptr<LuxxleBrowserTabStripController> controller_ = nullptr;

  base::WeakPtrFactory<LuxxleTabContextMenuContents> weak_ptr_{this};
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_TABS_LUXXLE_TAB_CONTEXT_MENU_CONTENTS_H_
