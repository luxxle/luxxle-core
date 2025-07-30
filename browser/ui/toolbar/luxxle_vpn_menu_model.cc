/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/toolbar/luxxle_vpn_menu_model.h"

#include "base/feature_list.h"
#include "luxxle/app/luxxle_command_ids.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_commands.h"
#include "components/prefs/pref_service.h"

#if BUILDFLAG(IS_WIN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
#endif

LuxxleVPNMenuModel::LuxxleVPNMenuModel(Browser* browser,
                                     PrefService* profile_prefs)
    : SimpleMenuModel(this), profile_prefs_(profile_prefs), browser_(browser) {
  Build();
}

LuxxleVPNMenuModel::~LuxxleVPNMenuModel() = default;

void LuxxleVPNMenuModel::Build() {
  AddItemWithStringId(IDC_TOGGLE_LUXXLE_VPN, IDS_LUXXLE_VPN_MENU);
  AddSeparator(ui::NORMAL_SEPARATOR);
  if (!IsLuxxleVPNButtonVisible()) {
    AddItemWithStringId(IDC_TOGGLE_LUXXLE_VPN_TOOLBAR_BUTTON,
                        IDS_LUXXLE_VPN_SHOW_VPN_BUTTON_MENU_ITEM);
  }
#if BUILDFLAG(IS_WIN)
  if (!IsTrayIconEnabled()) {
    AddItemWithStringId(IDC_TOGGLE_LUXXLE_VPN_TRAY_ICON,
                        IDS_LUXXLE_VPN_SHOW_VPN_TRAY_ICON_MENU_ITEM);
  }
#endif  // BUILDFLAG(IS_WIN)
  AddItemWithStringId(IDC_SEND_LUXXLE_VPN_FEEDBACK,
                      IDS_LUXXLE_VPN_SHOW_FEEDBACK_MENU_ITEM);
  AddItemWithStringId(IDC_ABOUT_LUXXLE_VPN, IDS_LUXXLE_VPN_ABOUT_VPN_MENU_ITEM);
  AddItemWithStringId(IDC_MANAGE_LUXXLE_VPN_PLAN,
                      IDS_LUXXLE_VPN_MANAGE_MY_PLAN_MENU_ITEM);
}

void LuxxleVPNMenuModel::ExecuteCommand(int command_id, int event_flags) {
  chrome::ExecuteCommand(browser_, command_id);
}

bool LuxxleVPNMenuModel::IsLuxxleVPNButtonVisible() const {
  return profile_prefs_->GetBoolean(luxxle_vpn::prefs::kLuxxleVPNShowButton);
}

#if BUILDFLAG(IS_WIN)
bool LuxxleVPNMenuModel::IsTrayIconEnabled() const {
  if (tray_icon_enabled_for_testing_.has_value()) {
    return tray_icon_enabled_for_testing_.value();
  }

  return luxxle_vpn::IsVPNTrayIconEnabled();
}
#endif
