/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/toolbar/luxxle_vpn_toggle_button.h"

#include <utility>

#include "base/functional/bind.h"
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/color/color_id.h"

using ConnectionState = luxxle_vpn::mojom::ConnectionState;
using PurchasedState = luxxle_vpn::mojom::PurchasedState;

LuxxleVPNToggleButton::LuxxleVPNToggleButton(Browser* browser)
    : browser_(browser),
      service_(luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(
          browser_->profile())) {
  CHECK(service_);

  Observe(service_);

  SetCallback(base::BindRepeating(&LuxxleVPNToggleButton::OnButtonPressed,
                                  base::Unretained(this)));
  UpdateState();

  if (const ui::ColorProvider* provider =
          BrowserView::GetBrowserViewForBrowser(browser_)->GetColorProvider()) {
    SetThumbOnColor(provider->GetColor(ui::kColorToggleButtonThumbOn));
    SetThumbOffColor(provider->GetColor(ui::kColorToggleButtonThumbOff));
    SetTrackOnColor(provider->GetColor(ui::kColorToggleButtonTrackOn));
    SetTrackOffColor(provider->GetColor(ui::kColorToggleButtonTrackOff));
  }

  // TODO(simonhong): Re-visit this name.
  SetAccessibleName(
      l10n_util::GetStringUTF16(IDS_LUXXLE_VPN_TOGGLE_MENU_ITEM_TEXT));
}

LuxxleVPNToggleButton::~LuxxleVPNToggleButton() = default;

void LuxxleVPNToggleButton::OnConnectionStateChanged(ConnectionState state) {
  UpdateState();
}

void LuxxleVPNToggleButton::OnButtonPressed(const ui::Event& event) {
  service_->ToggleConnection();
}

void LuxxleVPNToggleButton::UpdateState() {
  const auto state = service_->GetConnectionState();
  bool is_on = (state == ConnectionState::CONNECTING ||
                state == ConnectionState::CONNECTED);
  SetIsOn(is_on);
}

BEGIN_METADATA(LuxxleVPNToggleButton)
END_METADATA
