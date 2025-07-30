/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_APP_MENU_H_
#define LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_APP_MENU_H_

#include "base/memory/raw_ptr.h"
#include "luxxle/components/misc_metrics/menu_metrics.h"
#include "chrome/browser/ui/views/toolbar/app_menu.h"

class LuxxleAppMenu : public AppMenu {
 public:
  LuxxleAppMenu(Browser* browser, ui::MenuModel* model, int run_types);
  ~LuxxleAppMenu() override;

  LuxxleAppMenu(const LuxxleAppMenu&) = delete;
  LuxxleAppMenu& operator=(const LuxxleAppMenu&) = delete;

  Browser* browser() { return browser_; }
  const Browser* browser() const { return browser_; }

  // AppMenu overrides:
  void RunMenu(views::MenuButtonController* host) override;
  void ExecuteCommand(int command_id, int mouse_event_flags) override;
  void OnMenuClosed(views::MenuItemView* menu) override;

 private:
  void RecordMenuUsage(int command_id);

  // Update item's view as luxxle style.
  void UpdateMenuItemView();

  base::raw_ptr<misc_metrics::MenuMetrics> menu_metrics_;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_APP_MENU_H_
