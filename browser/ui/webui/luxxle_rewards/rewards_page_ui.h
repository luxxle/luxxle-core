/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_REWARDS_REWARDS_PAGE_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_REWARDS_REWARDS_PAGE_UI_H_

#include <memory>
#include <string>

// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "content/public/browser/web_ui_controller.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

namespace luxxle_rewards {

// The WebUI controller for the Rewards page when viewed in a tab.
class RewardsPageUI : public content::WebUIController {
 public:
  explicit RewardsPageUI(content::WebUI* web_ui, const std::string& host);
  ~RewardsPageUI() override;

  void BindInterface(mojo::PendingReceiver<mojom::RewardsPageHandler> receiver);

 private:
  std::unique_ptr<mojom::RewardsPageHandler> handler_;

  WEB_UI_CONTROLLER_TYPE_DECL();
};

}  // namespace luxxle_rewards

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_REWARDS_REWARDS_PAGE_UI_H_
