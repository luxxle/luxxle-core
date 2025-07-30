/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_account_handler.h"

#include <utility>

#include "components/password_manager/core/browser/ui/weak_check_utility.h"

namespace luxxle_account {
LuxxleAccountHandler::LuxxleAccountHandler(
    mojo::PendingReceiver<mojom::LuxxleAccountHandler> handler)
    : handler_(this, std::move(handler)) {}

LuxxleAccountHandler::~LuxxleAccountHandler() = default;

void LuxxleAccountHandler::GetPasswordStrength(
    const std::string& password,
    mojom::LuxxleAccountHandler::GetPasswordStrengthCallback callback) {
  std::move(callback).Run(password_manager::GetPasswordStrength(password));
}
}  // namespace luxxle_account
