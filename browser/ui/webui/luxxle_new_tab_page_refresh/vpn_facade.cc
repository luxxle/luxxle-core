// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/vpn_facade.h"

#include "luxxle/components/constants/pref_names.h"
#include "chrome/browser/ui/browser_window/public/browser_window_features.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "components/tabs/public/tab_interface.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
#include "luxxle/browser/ui/luxxle_vpn/luxxle_vpn_controller.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

namespace luxxle_new_tab_page_refresh {

#if BUILDFLAG(ENABLE_LUXXLE_VPN)

VPNFacade::VPNFacade(tabs::TabInterface& tab,
                     luxxle_vpn::LuxxleVpnService* vpn_service)
    : tab_(tab), vpn_service_(vpn_service) {}

VPNFacade::~VPNFacade() = default;

void VPNFacade::ReloadPurchasedState() {
  if (vpn_service_) {
    vpn_service_->ReloadPurchasedState();
  }
}

void VPNFacade::OpenPanel() {
  tab_->GetBrowserWindowInterface()
      ->GetFeatures()
      .luxxle_vpn_controller()
      ->ShowLuxxleVPNBubble(/* show_select */ true);
}

void VPNFacade::OpenAccountPage(luxxle_vpn::mojom::ManageURLType url_type) {
  tab_->GetBrowserWindowInterface()
      ->GetFeatures()
      .luxxle_vpn_controller()
      ->OpenVPNAccountPage(url_type);
}

void VPNFacade::RecordWidgetUsage() {
  if (vpn_service_) {
    vpn_service_->luxxle_vpn_metrics()->RecordWidgetUsage(true);
  }
}

std::optional<std::string> VPNFacade::GetWidgetPrefName() {
  return kNewTabPageShowLuxxleVPN;
}

#endif

}  // namespace luxxle_new_tab_page_refresh
