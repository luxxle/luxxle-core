/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_PERMISSIONS_LUXXLE_WALLET_PERMISSION_PROMPT_ANDROID_H_
#define LUXXLE_BROWSER_PERMISSIONS_LUXXLE_WALLET_PERMISSION_PROMPT_ANDROID_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "luxxle/browser/permissions/luxxle_dapp_permission_prompt_dialog_controller_android.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "components/permissions/permissions_client.h"

namespace content {
class WebContents;
}

namespace permissions {
class PermissionPromptAndroid;
}

class LuxxleWalletPermissionPrompt
    : public LuxxleDappPermissionPromptDialogController::Delegate,
      public permissions::PermissionsClient::PermissionMessageDelegate {
 public:
  class Delegate {
   public:
    Delegate();
    Delegate(const base::WeakPtr<permissions::PermissionPromptAndroid>&
                 permission_prompt);
    virtual ~Delegate();
    virtual void Closing();

   private:
    base::WeakPtr<permissions::PermissionPromptAndroid> permission_prompt_;
  };

  LuxxleWalletPermissionPrompt(content::WebContents* web_contents,
                              std::unique_ptr<Delegate> delegate,
                              luxxle_wallet::mojom::CoinType coin_type);
  ~LuxxleWalletPermissionPrompt() override;

 protected:
  // LuxxleDappPermissionPromptDialogController::Delegate:
  void OnDialogDismissed() override;
  void ConnectToSite(const std::vector<std::string>& accounts,
                     int permission_lifetime_option) override;
  void CancelConnectToSite() override;

 private:
  std::unique_ptr<LuxxleDappPermissionPromptDialogController> dialog_controller_;
  raw_ptr<content::WebContents> web_contents_ = nullptr;
  std::unique_ptr<Delegate> delegate_;

  bool has_interacted_with_dialog_ = false;
};

#endif  // LUXXLE_BROWSER_PERMISSIONS_LUXXLE_WALLET_PERMISSION_PROMPT_ANDROID_H_
