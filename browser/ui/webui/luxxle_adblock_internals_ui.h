// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_ADBLOCK_INTERNALS_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_ADBLOCK_INTERNALS_UI_H_

#include "luxxle/components/constants/webui_url_constants.h"
#include "content/public/browser/web_ui_controller.h"
#include "content/public/browser/webui_config.h"
#include "content/public/common/url_constants.h"

class LuxxleAdblockInternalsUI;

class LuxxleAdblockInternalsUIConfig
    : public content::DefaultWebUIConfig<LuxxleAdblockInternalsUI> {
 public:
  LuxxleAdblockInternalsUIConfig()
      : DefaultWebUIConfig(content::kChromeUIScheme, kAdblockInternalsHost) {}
};

// The WebUI for luxxle://adblock-internals
class LuxxleAdblockInternalsUI : public content::WebUIController {
 public:
  explicit LuxxleAdblockInternalsUI(content::WebUI* web_ui);

  LuxxleAdblockInternalsUI(const LuxxleAdblockInternalsUI&) = delete;
  LuxxleAdblockInternalsUI& operator=(const LuxxleAdblockInternalsUI&) = delete;

  ~LuxxleAdblockInternalsUI() override;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_ADBLOCK_INTERNALS_UI_H_
