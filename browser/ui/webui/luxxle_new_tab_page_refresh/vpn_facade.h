// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_VPN_FACADE_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_VPN_FACADE_H_

#include <optional>
#include <string>

#include "base/memory/raw_ptr.h"
#include "base/memory/raw_ref.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"

namespace tabs {
class TabInterface;
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
namespace luxxle_vpn {
class LuxxleVpnService;
}
#endif

namespace luxxle_new_tab_page_refresh {

#if BUILDFLAG(ENABLE_LUXXLE_VPN)

// Provides a simplified interface for accessing the Luxxle VPN service API from
// the new tab page. This adapter is primarily used to avoid preprocessor
// branching in `NewTabPageHandler`.
class VPNFacade {
 public:
  VPNFacade(tabs::TabInterface& tab, luxxle_vpn::LuxxleVpnService* vpn_service);
  ~VPNFacade();

  VPNFacade(const VPNFacade&) = delete;
  VPNFacade& operator=(const VPNFacade&) = delete;

  void ReloadPurchasedState();
  void OpenPanel();
  void OpenAccountPage(luxxle_vpn::mojom::ManageURLType url_type);
  void RecordWidgetUsage();
  std::optional<std::string> GetWidgetPrefName();

 private:
  raw_ref<tabs::TabInterface> tab_;
  raw_ptr<luxxle_vpn::LuxxleVpnService> vpn_service_;
};

#else

// Provides a no-op implementation for when the Luxxle VPN API does not exist.
class VPNFacade {
 public:
  VPNFacade() = default;
  ~VPNFacade() = default;

  VPNFacade(const VPNFacade&) = delete;
  VPNFacade& operator=(const VPNFacade&) = delete;

  void ReloadPurchasedState() {}
  void OpenPanel() {}
  void OpenAccountPage(luxxle_vpn::mojom::ManageURLType url_type) {}
  void RecordWidgetUsage() {}
  std::optional<std::string> GetWidgetPrefName() { return std::nullopt; }
};

#endif

}  // namespace luxxle_new_tab_page_refresh

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_VPN_FACADE_H_
