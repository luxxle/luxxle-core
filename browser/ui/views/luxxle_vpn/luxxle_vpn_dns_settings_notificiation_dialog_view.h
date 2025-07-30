/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_VPN_LUXXLE_VPN_DNS_SETTINGS_NOTIFICIATION_DIALOG_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_VPN_LUXXLE_VPN_DNS_SETTINGS_NOTIFICIATION_DIALOG_VIEW_H_

#include "ui/views/window/dialog_delegate.h"

class Browser;
class PrefService;

namespace views {
class Checkbox;
}

namespace luxxle_vpn {

class LuxxleVpnDnsSettingsNotificiationDialogView
    : public views::DialogDelegateView {
  METADATA_HEADER(LuxxleVpnDnsSettingsNotificiationDialogView,
                  views::DialogDelegateView)
 public:

  static void Show(Browser* browser);

  LuxxleVpnDnsSettingsNotificiationDialogView(
      const LuxxleVpnDnsSettingsNotificiationDialogView&) = delete;
  LuxxleVpnDnsSettingsNotificiationDialogView& operator=(
      const LuxxleVpnDnsSettingsNotificiationDialogView&) = delete;

 private:
  explicit LuxxleVpnDnsSettingsNotificiationDialogView(Browser* browser);
  ~LuxxleVpnDnsSettingsNotificiationDialogView() override;

  void OnAccept();
  void OnClosing();

  void OnLearnMoreLinkClicked();

  // views::DialogDelegate overrides:
  ui::mojom::ModalType GetModalType() const override;
  bool ShouldShowCloseButton() const override;
  bool ShouldShowWindowTitle() const override;

  bool close_window_ = true;
  raw_ptr<Browser> browser_ = nullptr;
  raw_ptr<PrefService> prefs_ = nullptr;
  raw_ptr<views::Checkbox> dont_ask_again_checkbox_ = nullptr;
};

}  // namespace luxxle_vpn

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_VPN_LUXXLE_VPN_DNS_SETTINGS_NOTIFICIATION_DIALOG_VIEW_H_
