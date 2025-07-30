/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_ADBLOCK_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_ADBLOCK_UI_H_

#include "luxxle/components/constants/webui_url_constants.h"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/webui_config.h"
#include "content/public/common/url_constants.h"

class LuxxleAdblockUI;

class LuxxleAdblockUIConfig
    : public content::DefaultWebUIConfig<LuxxleAdblockUI> {
 public:
  LuxxleAdblockUIConfig()
      : DefaultWebUIConfig(content::kChromeUIScheme, kAdblockHost) {}
};

class LuxxleAdblockUI : public content::WebUIController {
 public:
  explicit LuxxleAdblockUI(content::WebUI* web_ui);
  ~LuxxleAdblockUI() override;
  LuxxleAdblockUI(const LuxxleAdblockUI&) = delete;
  LuxxleAdblockUI& operator=(const LuxxleAdblockUI&) = delete;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_ADBLOCK_UI_H_
