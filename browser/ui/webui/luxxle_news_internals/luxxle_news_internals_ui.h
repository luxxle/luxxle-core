// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEWS_INTERNALS_LUXXLE_NEWS_INTERNALS_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEWS_INTERNALS_LUXXLE_NEWS_INTERNALS_UI_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "luxxle/components/luxxle_news/common/luxxle_news.mojom-forward.h"
#include "content/public/browser/web_ui_controller.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

namespace luxxle_news {
class LuxxleNewsController;
}

class LuxxleNewsInternalsUI : public content::WebUIController {
 public:
  explicit LuxxleNewsInternalsUI(content::WebUI* web_ui,
                                const std::string& host,
                                luxxle_news::LuxxleNewsController* controller);
  LuxxleNewsInternalsUI(const LuxxleNewsInternalsUI&) = delete;
  LuxxleNewsInternalsUI& operator=(const LuxxleNewsInternalsUI&) = delete;
  ~LuxxleNewsInternalsUI() override;

  void BindInterface(
      mojo::PendingReceiver<luxxle_news::mojom::LuxxleNewsController> receiver);
  void BindInterface(
      mojo::PendingReceiver<luxxle_news::mojom::LuxxleNewsInternals> receiver);

 private:
  raw_ptr<luxxle_news::LuxxleNewsController> controller_ = nullptr;
  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEWS_INTERNALS_LUXXLE_NEWS_INTERNALS_UI_H_
