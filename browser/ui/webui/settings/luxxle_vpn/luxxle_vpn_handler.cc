/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_vpn/luxxle_vpn_handler.h"

#include <memory>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/path_service.h"
#include "base/process/launch.h"
#include "base/task/thread_pool.h"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/browser/browser_process.h"
#include "components/prefs/pref_service.h"

LuxxleVpnHandler::LuxxleVpnHandler(Profile* profile) : profile_(profile) {
  auto* service = luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(profile);
  CHECK(service);
  Observe(service);

  pref_change_registrar_.Init(g_browser_process->local_state());
  pref_change_registrar_.Add(
      luxxle_vpn::prefs::kLuxxleVPNWireguardEnabled,
      base::BindRepeating(&LuxxleVpnHandler::OnProtocolChanged,
                          base::Unretained(this)));
}

LuxxleVpnHandler::~LuxxleVpnHandler() = default;

void LuxxleVpnHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "isWireguardServiceInstalled",
      base::BindRepeating(&LuxxleVpnHandler::HandleIsWireguardServiceInstalled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isLuxxleVpnConnected",
      base::BindRepeating(&LuxxleVpnHandler::HandleIsLuxxleVpnConnected,
                          base::Unretained(this)));
}

void LuxxleVpnHandler::OnProtocolChanged() {
  auto enabled =
      luxxle_vpn::IsLuxxleVPNWireguardEnabled(g_browser_process->local_state());
  luxxle_vpn::SetWireguardActive(enabled);
}

void LuxxleVpnHandler::OnWireguardServiceInstalled(
    const std::string& callback_id,
    bool success) {
  AllowJavascript();
  ResolveJavascriptCallback(callback_id, base::Value(success));
}

void LuxxleVpnHandler::HandleIsWireguardServiceInstalled(
    const base::Value::List& args) {
  AllowJavascript();

  ResolveJavascriptCallback(
      args[0],
      base::Value(luxxle_vpn::wireguard::IsWireguardServiceInstalled()));
}

void LuxxleVpnHandler::HandleIsLuxxleVpnConnected(const base::Value::List& args) {
  AllowJavascript();

  auto* service = luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(profile_);
  ResolveJavascriptCallback(args[0],
                            base::Value(service && service->IsConnected()));
}

void LuxxleVpnHandler::OnConnectionStateChanged(
    luxxle_vpn::mojom::ConnectionState state) {
  AllowJavascript();
  FireWebUIListener(
      "luxxle-vpn-state-change",
      base::Value(state == luxxle_vpn::mojom::ConnectionState::CONNECTED));
}

void LuxxleVpnHandler::OnJavascriptAllowed() {}

void LuxxleVpnHandler::OnJavascriptDisallowed() {}
