/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_VPN_MENU_MODEL_H_
#define LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_VPN_MENU_MODEL_H_

#include <optional>

#include "base/memory/raw_ptr.h"
#include "ui/menus/simple_menu_model.h"

class Browser;
class PrefService;

class LuxxleVPNMenuModel : public ui::SimpleMenuModel,
                          public ui::SimpleMenuModel::Delegate {
 public:
  LuxxleVPNMenuModel(Browser* browser, PrefService* profile_prefs);
  ~LuxxleVPNMenuModel() override;

  LuxxleVPNMenuModel(const LuxxleVPNMenuModel&) = delete;
  LuxxleVPNMenuModel& operator=(const LuxxleVPNMenuModel&) = delete;
#if BUILDFLAG(IS_WIN)
  void SetTrayIconEnabledForTesting(bool value) {
    tray_icon_enabled_for_testing_ = value;
  }
#endif  // BUILDFLAG(IS_WIN)
 private:
  FRIEND_TEST_ALL_PREFIXES(LuxxleVPNMenuModelUnitTest, TrayIconEnabled);
  FRIEND_TEST_ALL_PREFIXES(LuxxleVPNMenuModelUnitTest, TrayIconDisabled);
  FRIEND_TEST_ALL_PREFIXES(LuxxleVPNMenuModelUnitTest, ToolbarVPNButton);

  // ui::SimpleMenuModel::Delegate override:
  void ExecuteCommand(int command_id, int event_flags) override;

  void Build();
  bool IsLuxxleVPNButtonVisible() const;
#if BUILDFLAG(IS_WIN)
  bool IsTrayIconEnabled() const;
#endif  // BUILDFLAG(IS_WIN)
  std::optional<bool> tray_icon_enabled_for_testing_;
  raw_ptr<PrefService> profile_prefs_ = nullptr;
  raw_ptr<Browser> browser_ = nullptr;
};

#endif  // LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_VPN_MENU_MODEL_H_
