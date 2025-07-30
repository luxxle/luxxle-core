/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_settings_secure_dns_handler.h"

#include "base/feature_list.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/browser/browser_process.h"
#include "chrome/common/pref_names.h"
#include "components/prefs/pref_service.h"

namespace settings {

LuxxleSecureDnsHandler::LuxxleSecureDnsHandler() = default;
LuxxleSecureDnsHandler::~LuxxleSecureDnsHandler() = default;

void LuxxleSecureDnsHandler::OnJavascriptAllowed() {
  SecureDnsHandler::OnJavascriptAllowed();
  pref_registrar_.Init(g_browser_process->local_state());
  if (base::FeatureList::IsEnabled(
          luxxle_vpn::features::kLuxxleVPNDnsProtection)) {
    pref_registrar_.Add(
        prefs::kLuxxleVpnDnsConfig,
        base::BindRepeating(
            &LuxxleSecureDnsHandler::SendSecureDnsSettingUpdatesToJavascript,
            base::Unretained(this)));
  }
}

void LuxxleSecureDnsHandler::OnJavascriptDisallowed() {
  SecureDnsHandler::OnJavascriptDisallowed();
  pref_registrar_.RemoveAll();
}

}  // namespace settings
