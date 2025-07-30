/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_TOGGLE_BUTTON_H_
#define LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_TOGGLE_BUTTON_H_

#include "base/memory/raw_ptr.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/controls/button/toggle_button.h"

namespace luxxle_vpn {
class LuxxleVpnService;
}  // namespace luxxle_vpn

class Browser;

class LuxxleVPNToggleButton : public views::ToggleButton,
                             public luxxle_vpn::LuxxleVPNServiceObserver {
  METADATA_HEADER(LuxxleVPNToggleButton, views::ToggleButton)
 public:
  explicit LuxxleVPNToggleButton(Browser* browser);
  ~LuxxleVPNToggleButton() override;

  LuxxleVPNToggleButton(const LuxxleVPNToggleButton&) = delete;
  LuxxleVPNToggleButton& operator=(const LuxxleVPNToggleButton&) = delete;

 private:
  // LuxxleVPNServiceObserver overrides:
  void OnConnectionStateChanged(
      luxxle_vpn::mojom::ConnectionState state) override;

  void OnButtonPressed(const ui::Event& event);
  void UpdateState();

  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<luxxle_vpn::LuxxleVpnService> service_ = nullptr;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_TOGGLE_BUTTON_H_
