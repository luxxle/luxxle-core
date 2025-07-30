/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_VPN_LUXXLE_VPN_LOCALIZED_STRING_PROVIDER_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_VPN_LUXXLE_VPN_LOCALIZED_STRING_PROVIDER_H_

namespace content {
class WebUIDataSource;
}
namespace luxxle_vpn {

void AddLocalizedStrings(content::WebUIDataSource* html_source);

}  // namespace luxxle_vpn

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_VPN_LUXXLE_VPN_LOCALIZED_STRING_PROVIDER_H_
