/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_SETTINGS_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_SETTINGS_UI_H_

#include <memory>

#include "luxxle/components/ai_chat/core/common/mojom/settings_helper.mojom.h"
#include "luxxle/components/luxxle_account/core/mojom/luxxle_account.mojom.h"
#include "luxxle/components/commands/common/commands.mojom.h"
#include "chrome/browser/ui/webui/settings/settings_ui.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

class LuxxleSettingsUI;

namespace luxxle_account {
class LuxxleAccountHandler;
}

namespace content {
class WebUIDataSource;
}

class Profile;

class LuxxleSettingsUIConfig
    : public content::DefaultWebUIConfig<LuxxleSettingsUI> {
 public:
  LuxxleSettingsUIConfig()
      : DefaultWebUIConfig(content::kChromeUIScheme,
                           chrome::kChromeUISettingsHost) {}
};

class LuxxleSettingsUI : public settings::SettingsUI {
 public:
  explicit LuxxleSettingsUI(content::WebUI* web_ui);
  LuxxleSettingsUI(const LuxxleSettingsUI&) = delete;
  LuxxleSettingsUI& operator=(const LuxxleSettingsUI&) = delete;
  ~LuxxleSettingsUI() override;

  static void AddResources(content::WebUIDataSource* html_source,
                           Profile* profile);
  static bool& ShouldExposeElementsForTesting();

  void BindInterface(
      mojo::PendingReceiver<commands::mojom::CommandsService> pending_receiver);
  void BindInterface(mojo::PendingReceiver<ai_chat::mojom::AIChatSettingsHelper>
                         pending_receiver);
  void BindInterface(
      mojo::PendingReceiver<luxxle_account::mojom::LuxxleAccountHandler>
          pending_receiver);

 private:
  std::unique_ptr<luxxle_account::LuxxleAccountHandler> luxxle_account_handler_;
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_SETTINGS_UI_H_
