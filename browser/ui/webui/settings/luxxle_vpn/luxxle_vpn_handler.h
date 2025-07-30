/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_VPN_LUXXLE_VPN_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_VPN_LUXXLE_VPN_HANDLER_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"
#include "components/prefs/pref_change_registrar.h"

class LuxxleVpnHandler : public settings::SettingsPageUIHandler,
                        public luxxle_vpn::LuxxleVPNServiceObserver {
 public:
  explicit LuxxleVpnHandler(Profile* profile);
  ~LuxxleVpnHandler() override;

 private:
  // WebUIMessageHandler implementation.
  void RegisterMessages() override;

  void HandleIsWireguardServiceInstalled(const base::Value::List& args);
  void OnWireguardServiceInstalled(const std::string& callback_id,
                                   bool success);
  void HandleIsLuxxleVpnConnected(const base::Value::List& args);

  // luxxle_vpn::LuxxleVPNServiceObserver
  void OnConnectionStateChanged(
      luxxle_vpn::mojom::ConnectionState state) override;
  void OnProtocolChanged();

  // SettingsPageUIHandler implementation.
  void OnJavascriptAllowed() override;
  void OnJavascriptDisallowed() override;

  PrefChangeRegistrar pref_change_registrar_;
  const raw_ptr<Profile, DanglingUntriaged> profile_;
  base::WeakPtrFactory<LuxxleVpnHandler> weak_factory_{this};
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_VPN_LUXXLE_VPN_HANDLER_H_
