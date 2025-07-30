// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

// based on //chrome/browser/ui/webui/whats_new/whats_new_ui.h

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_PAGE_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_PAGE_UI_H_

#include <memory>

#include "luxxle/browser/ui/webui/luxxle_browser_command/luxxle_browser_command_handler.h"
#include "luxxle/browser/ui/webui/luxxle_education/luxxle_education.mojom.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "luxxle/ui/webui/resources/js/luxxle_browser_command/luxxle_browser_command.mojom.h"
#include "content/public/browser/webui_config.h"
#include "content/public/common/url_constants.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_web_ui_controller.h"
#include "url/gurl.h"

namespace content {
class WebUI;
}

class LuxxleEducationHandler;
class LuxxleEducationPageUI;

class LuxxleEducationPageUIConfig
    : public content::DefaultWebUIConfig<LuxxleEducationPageUI> {
 public:
  LuxxleEducationPageUIConfig()
      : DefaultWebUIConfig(content::kChromeUIScheme, kLuxxleGettingStartedHost) {
  }
};

// The Web UI controller for the Luxxle product education page, which displays
// production education website content in an iframe.
class LuxxleEducationPageUI
    : public ui::MojoWebUIController,
      public luxxle_browser_command::mojom::LuxxleBrowserCommandHandlerFactory,
      public luxxle_education::mojom::PageHandlerFactory {
 public:
  LuxxleEducationPageUI(content::WebUI* web_ui, const GURL& url);
  ~LuxxleEducationPageUI() override;

  // Instantiates the implementor of the
  // luxxle_education::mojom::PageHandlerFactory mojo interface.
  void BindInterface(
      mojo::PendingReceiver<luxxle_education::mojom::PageHandlerFactory>
          receiver);

  // Instantiates the implementor of the
  // browser_command::mojom::LuxxleBrowserCommandHandler mojo interface.
  void BindInterface(
      mojo::PendingReceiver<
          luxxle_browser_command::mojom::LuxxleBrowserCommandHandlerFactory>
          pending_receiver);

  LuxxleEducationPageUI(const LuxxleEducationPageUI&) = delete;
  LuxxleEducationPageUI& operator=(const LuxxleEducationPageUI&) = delete;

 private:
  // luxxle_education::mojom::PageHandlerFactory:
  void CreatePageHandler(
      mojo::PendingRemote<luxxle_education::mojom::Page> page,
      mojo::PendingReceiver<luxxle_education::mojom::PageHandler> receiver)
      override;

  std::unique_ptr<LuxxleEducationHandler> page_handler_;
  mojo::Receiver<luxxle_education::mojom::PageHandlerFactory>
      page_factory_receiver_{this};

  // luxxle_browser_command::mojom::LuxxleBrowserCommandHandlerFactory:
  void CreateBrowserCommandHandler(
      mojo::PendingReceiver<
          luxxle_browser_command::mojom::LuxxleBrowserCommandHandler> handler)
      override;

  std::unique_ptr<LuxxleBrowserCommandHandler> command_handler_;
  mojo::Receiver<
      luxxle_browser_command::mojom::LuxxleBrowserCommandHandlerFactory>
      browser_command_factory_receiver_;
  raw_ptr<Profile> profile_;

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_PAGE_UI_H_
