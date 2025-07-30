/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_STATUS_LABEL_H_
#define LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_STATUS_LABEL_H_

#include "base/memory/raw_ptr.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/controls/label.h"

namespace luxxle_vpn {
class LuxxleVpnService;
}  // namespace luxxle_vpn

class Browser;

class LuxxleVPNStatusLabel : public views::Label,
                            public luxxle_vpn::LuxxleVPNServiceObserver {
  METADATA_HEADER(LuxxleVPNStatusLabel, views::Label)
 public:
  explicit LuxxleVPNStatusLabel(Browser* browser);
  ~LuxxleVPNStatusLabel() override;

  LuxxleVPNStatusLabel(const LuxxleVPNStatusLabel&) = delete;
  LuxxleVPNStatusLabel& operator=(const LuxxleVPNStatusLabel&) = delete;

 private:
  // luxxle_vpn::LuxxleVPNServiceObserver overrides:
  void OnConnectionStateChanged(
      luxxle_vpn::mojom::ConnectionState state) override;

  void UpdateState();

  int longest_state_string_id_ = -1;
  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<luxxle_vpn::LuxxleVpnService> service_ = nullptr;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_STATUS_LABEL_H_
