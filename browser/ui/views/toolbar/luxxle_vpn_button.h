/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_BUTTON_H_
#define LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_BUTTON_H_

#include <memory>
#include <optional>
#include <string>

#include "base/memory/raw_ptr.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/browser/ui/views/toolbar/toolbar_button.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/controls/button/menu_button_controller.h"

namespace luxxle_vpn {
class LuxxleVpnService;
class LuxxleVpnButtonUnitTest;
}  // namespace luxxle_vpn

namespace views {
class Border;
}  // namespace views

class Browser;

class LuxxleVPNButton : public ToolbarButton,
                       public luxxle_vpn::LuxxleVPNServiceObserver {
  METADATA_HEADER(LuxxleVPNButton, ToolbarButton)
 public:

  explicit LuxxleVPNButton(Browser* browser);
  ~LuxxleVPNButton() override;

  LuxxleVPNButton(const LuxxleVPNButton&) = delete;
  LuxxleVPNButton& operator=(const LuxxleVPNButton&) = delete;

  // LuxxleVPNServiceObserver overrides:
  void OnConnectionStateChanged(
      luxxle_vpn::mojom::ConnectionState state) override;
  void OnPurchasedStateChanged(
      luxxle_vpn::mojom::PurchasedState state,
      const std::optional<std::string>& description) override;

 private:
  friend class luxxle_vpn::LuxxleVpnButtonUnitTest;

  // ToolbarButton overrides:
  void UpdateColorsAndInsets() override;
  std::u16string GetRenderedTooltipText(const gfx::Point& p) const override;
  void OnThemeChanged() override;
  void InkDropRippleAnimationEnded(views::InkDropState state) override;

  void SetVpnConnectionStateForTesting(
      luxxle_vpn::mojom::ConnectionState state) {
    connection_state_for_testing_ = state;
  }
  luxxle_vpn::mojom::ConnectionState GetVpnConnectionState() const;
  bool IsErrorState() const { return is_error_state_; }
  bool IsConnected() const;
  bool IsConnectError() const;
  bool IsPurchased() const;
  std::unique_ptr<views::Border> GetBorder(SkColor border_color) const;
  void OnButtonPressed(const ui::Event& event);
  void UpdateButtonState();
  SkColor GetIconColor();
  SkColor GetBadgeColor();
  const gfx::VectorIcon& GetBadgeIcon();

  bool is_error_state_ = false;
  bool is_connected_ = false;
  std::optional<luxxle_vpn::mojom::ConnectionState>
      connection_state_for_testing_;
  raw_ptr<Browser, DanglingUntriaged> browser_ = nullptr;
  raw_ptr<luxxle_vpn::LuxxleVpnService, DanglingUntriaged> service_ = nullptr;
  raw_ptr<views::MenuButtonController> menu_button_controller_ = nullptr;
  base::WeakPtrFactory<LuxxleVPNButton> weak_ptr_factory_{this};
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_LUXXLE_VPN_BUTTON_H_
