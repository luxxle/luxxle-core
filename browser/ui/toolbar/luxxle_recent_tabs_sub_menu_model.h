/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_RECENT_TABS_SUB_MENU_MODEL_H_
#define LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_RECENT_TABS_SUB_MENU_MODEL_H_

#include "chrome/browser/ui/tabs/recent_tabs_sub_menu_model.h"

class Browser;

namespace ui {
class AcceleratorProvider;
}

class LuxxleRecentTabsSubMenuModel : public RecentTabsSubMenuModel {
 public:
  LuxxleRecentTabsSubMenuModel(ui::AcceleratorProvider* accelerator_provider,
                         Browser* browser);

  LuxxleRecentTabsSubMenuModel(const LuxxleRecentTabsSubMenuModel&) = delete;
  LuxxleRecentTabsSubMenuModel& operator=(const LuxxleRecentTabsSubMenuModel&) =
      delete;

  ~LuxxleRecentTabsSubMenuModel() override;

  void ExecuteCommand(int command_id, int event_flags) override;
};

#endif  // LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_RECENT_TABS_SUB_MENU_MODEL_H_
