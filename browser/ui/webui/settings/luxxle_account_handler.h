/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_ACCOUNT_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_ACCOUNT_HANDLER_H_

#include <string>

#include "luxxle/components/luxxle_account/core/mojom/luxxle_account.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"

namespace luxxle_account {
class LuxxleAccountHandler : public mojom::LuxxleAccountHandler {
 public:
  explicit LuxxleAccountHandler(
      mojo::PendingReceiver<mojom::LuxxleAccountHandler> handler);

  LuxxleAccountHandler(const LuxxleAccountHandler&) = delete;
  LuxxleAccountHandler& operator=(const LuxxleAccountHandler&) = delete;

  ~LuxxleAccountHandler() override;

  void GetPasswordStrength(
      const std::string& password,
      mojom::LuxxleAccountHandler::GetPasswordStrengthCallback callback)
      override;

 private:
  mojo::Receiver<mojom::LuxxleAccountHandler> handler_;
};
}  // namespace luxxle_account

#endif  // LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_ACCOUNT_HANDLER_H_
