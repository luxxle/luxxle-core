/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_REWARDS_INTERNALS_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_REWARDS_INTERNALS_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class LuxxleRewardsInternalsUI : public content::WebUIController {
 public:
  LuxxleRewardsInternalsUI(content::WebUI* web_ui, const std::string& host);
  ~LuxxleRewardsInternalsUI() override;
  LuxxleRewardsInternalsUI(const LuxxleRewardsInternalsUI&) = delete;
  LuxxleRewardsInternalsUI& operator=(const LuxxleRewardsInternalsUI&) = delete;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_REWARDS_INTERNALS_UI_H_
