// Copyright (c) 2021 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_SHIELDS_SHIELDS_PANEL_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_SHIELDS_SHIELDS_PANEL_HANDLER_H_

#include "luxxle/components/luxxle_shields/core/common/luxxle_shields_panel.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

class TopChromeWebUIController;

namespace content {
class WebUI;
}  // namespace content

class Profile;
class LuxxleBrowserWindow;

class ShieldsPanelHandler : public luxxle_shields::mojom::PanelHandler {
 public:
  ShieldsPanelHandler(
      mojo::PendingReceiver<luxxle_shields::mojom::PanelHandler> receiver,
      TopChromeWebUIController* webui_controller,
      LuxxleBrowserWindow* luxxle_browser_window,
      Profile* profile);

  ShieldsPanelHandler(const ShieldsPanelHandler&) = delete;
  ShieldsPanelHandler& operator=(const ShieldsPanelHandler&) = delete;
  ~ShieldsPanelHandler() override;

  // luxxle_shields::mojom::PanelHandler:
  void ShowUI() override;
  void CloseUI() override;
  void GetPosition(GetPositionCallback callback) override;
  void SetAdvancedViewEnabled(bool is_enabled) override;
  void GetAdvancedViewEnabled(GetAdvancedViewEnabledCallback callback) override;

 private:
  mojo::Receiver<luxxle_shields::mojom::PanelHandler> receiver_;
  raw_ptr<TopChromeWebUIController> const webui_controller_;
  raw_ptr<LuxxleBrowserWindow> luxxle_browser_window_ = nullptr;
  raw_ptr<Profile> profile_ = nullptr;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_SHIELDS_SHIELDS_PANEL_HANDLER_H_
