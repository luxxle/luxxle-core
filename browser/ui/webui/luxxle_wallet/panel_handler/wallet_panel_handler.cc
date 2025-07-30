// Copyright (c) 2021 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/luxxle_wallet/panel_handler/wallet_panel_handler.h"

#include <utility>

#include "base/functional/callback.h"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/permissions/contexts/luxxle_wallet_permission_context.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"

// It's safe to bind the active webcontents when panel is created because
// the panel will not be shared across tabs.
WalletPanelHandler::WalletPanelHandler(
    mojo::PendingReceiver<luxxle_wallet::mojom::PanelHandler> receiver,
    TopChromeWebUIController* webui_controller,
    content::WebContents* active_web_contents,
    PanelCloseOnDeactivationCallback close_on_deactivation)
    : receiver_(this, std::move(receiver)),
      webui_controller_(webui_controller),
      active_web_contents_(active_web_contents),
      close_on_deactivation_(std::move(close_on_deactivation)) {
  DCHECK(active_web_contents_);
}

WalletPanelHandler::~WalletPanelHandler() = default;

void WalletPanelHandler::ShowUI() {
  auto embedder = webui_controller_->embedder();
  if (embedder) {
    embedder->ShowUI();
  }
}

void WalletPanelHandler::CloseUI() {
  auto embedder = webui_controller_->embedder();
  if (embedder) {
    embedder->CloseUI();
  }
}

void WalletPanelHandler::ConnectToSite(
    const std::vector<std::string>& accounts,
    luxxle_wallet::mojom::PermissionLifetimeOption option) {
  permissions::LuxxleWalletPermissionContext::AcceptOrCancel(
      accounts, option, active_web_contents_);
}

void WalletPanelHandler::CancelConnectToSite() {
  permissions::LuxxleWalletPermissionContext::Cancel(active_web_contents_);
}

void WalletPanelHandler::SetCloseOnDeactivate(bool close) {
  if (close_on_deactivation_) {
    close_on_deactivation_.Run(close);
  }
}

void WalletPanelHandler::Focus() {
  webui_controller_->web_ui()->GetWebContents()->Focus();
}

void WalletPanelHandler::IsSolanaAccountConnected(
    const std::string& account,
    IsSolanaAccountConnectedCallback callback) {
  content::RenderFrameHost* rfh = nullptr;
  if (!(rfh = active_web_contents_->GetFocusedFrame())) {
    std::move(callback).Run(false);
    return;
  }

  auto* tab_helper =
      luxxle_wallet::LuxxleWalletTabHelper::FromWebContents(active_web_contents_);
  if (!tab_helper) {
    std::move(callback).Run(false);
    return;
  }

  std::move(callback).Run(
      tab_helper->IsSolanaAccountConnected(rfh->GetGlobalId(), account));
}

void WalletPanelHandler::RequestPermission(
    luxxle_wallet::mojom::AccountIdPtr account_id,
    RequestPermissionCallback callback) {
  content::RenderFrameHost* rfh = nullptr;
  if (!(rfh = active_web_contents_->GetFocusedFrame())) {
    std::move(callback).Run(false);
    return;
  }

  auto request_type =
      luxxle_wallet::CoinTypeToPermissionRequestType(account_id->coin);
  auto permission = luxxle_wallet::CoinTypeToPermissionType(account_id->coin);
  if (!request_type || !permission) {
    std::move(callback).Run(false);
    return;
  }

  if (permissions::LuxxleWalletPermissionContext::HasRequestsInProgress(
          rfh, *request_type)) {
    std::move(callback).Run(false);
    return;
  }

  permissions::LuxxleWalletPermissionContext::RequestPermissions(
      *permission, rfh, {account_id->address},
      base::BindOnce(
          [](RequestPermissionCallback cb,
             const std::vector<blink::mojom::PermissionStatus>& responses) {
            if (responses.empty() || responses.size() != 1u) {
              std::move(cb).Run(false);
            } else {
              std::move(cb).Run(true);
            }
          },
          std::move(callback)));
}
