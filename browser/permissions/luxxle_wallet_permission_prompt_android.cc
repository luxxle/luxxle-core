/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/permissions/luxxle_wallet_permission_prompt_android.h"

#include <utility>

#include "luxxle/browser/permissions/luxxle_dapp_permission_prompt_dialog_controller_android.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/permissions/contexts/luxxle_wallet_permission_context.h"
#include "components/permissions/android/permission_prompt/permission_prompt_android.h"
#include "content/public/browser/web_contents.h"

LuxxleWalletPermissionPrompt::LuxxleWalletPermissionPrompt(
    content::WebContents* web_contents,
    std::unique_ptr<Delegate> delegate,
    luxxle_wallet::mojom::CoinType coin_type)
    : web_contents_(web_contents), delegate_(std::move(delegate)) {
  dialog_controller_ =
      std::make_unique<LuxxleDappPermissionPromptDialogController>(
          this, web_contents_, coin_type);
  dialog_controller_->ShowDialog();
}

LuxxleWalletPermissionPrompt::~LuxxleWalletPermissionPrompt() {}

void LuxxleWalletPermissionPrompt::ConnectToSite(
    const std::vector<std::string>& accounts,
    int permission_lifetime_option) {
  has_interacted_with_dialog_ = true;
  dialog_controller_.reset();
  // TODO(SergeyZhukovsky): Use the real option that the user chooses, using
  // `kForever` here is for landing new API changes separately.
  permissions::LuxxleWalletPermissionContext::AcceptOrCancel(
      accounts, luxxle_wallet::mojom::PermissionLifetimeOption::kForever,
      web_contents_);
}

void LuxxleWalletPermissionPrompt::CancelConnectToSite() {
  has_interacted_with_dialog_ = true;
  dialog_controller_.reset();
  permissions::LuxxleWalletPermissionContext::Cancel(web_contents_);
}

void LuxxleWalletPermissionPrompt::OnDialogDismissed() {
  if (!dialog_controller_) {
    // Dismissed by clicking on dialog buttons.
    return;
  }
  dialog_controller_.reset();
  // If |has_interacted_with_dialog_| is true, |ConnectToSite| or
  // |CancelConnectToSite| should be recorded instead.
  if (!has_interacted_with_dialog_) {
    permissions::LuxxleWalletPermissionContext::Cancel(web_contents_);
  }
}

void LuxxleWalletPermissionPrompt::Delegate::Closing() {
  if (!permission_prompt_) {
    return;
  }
  permission_prompt_->Closing();
}

LuxxleWalletPermissionPrompt::Delegate::~Delegate() {
  Closing();
}

LuxxleWalletPermissionPrompt::Delegate::Delegate() {}

LuxxleWalletPermissionPrompt::Delegate::Delegate(
    const base::WeakPtr<permissions::PermissionPromptAndroid>&
        permission_prompt)
    : permission_prompt_(permission_prompt) {}
