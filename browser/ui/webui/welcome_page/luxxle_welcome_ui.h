/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_WELCOME_PAGE_LUXXLE_WELCOME_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_WELCOME_PAGE_LUXXLE_WELCOME_UI_H_

#include <string>

#include "content/public/browser/web_ui_controller.h"

class LuxxleWelcomeUI : public content::WebUIController {
 public:
  LuxxleWelcomeUI(content::WebUI* web_ui, const std::string& host);
  ~LuxxleWelcomeUI() override;
  LuxxleWelcomeUI(const LuxxleWelcomeUI&) = delete;
  LuxxleWelcomeUI& operator=(const LuxxleWelcomeUI&) = delete;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_WELCOME_PAGE_LUXXLE_WELCOME_UI_H_
