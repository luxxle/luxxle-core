// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/luxxle_news_internals/luxxle_news_internals_ui.h"

#include <string>
#include <utility>

#include "luxxle/browser/ui/webui/luxxle_webui_source.h"
#include "luxxle/components/luxxle_news/browser/luxxle_news_controller.h"
#include "luxxle/components/luxxle_news/browser/resources/grit/luxxle_news_internals_generated_map.h"
#include "luxxle/components/luxxle_news/common/luxxle_news.mojom.h"
#include "components/grit/luxxle_components_resources.h"

LuxxleNewsInternalsUI::LuxxleNewsInternalsUI(
    content::WebUI* web_ui,
    const std::string& host,
    luxxle_news::LuxxleNewsController* controller)
    : content::WebUIController(web_ui), controller_(controller) {
  auto* source =
      CreateAndAddWebUIDataSource(web_ui, host, kLuxxleNewsInternalsGenerated,
                                  /* REMOVED: IDR_LUXXLE_NEWS_INTERNALS_HTML */ 0);
  DCHECK(source);
}

LuxxleNewsInternalsUI::~LuxxleNewsInternalsUI() = default;
WEB_UI_CONTROLLER_TYPE_IMPL(LuxxleNewsInternalsUI)

void LuxxleNewsInternalsUI::BindInterface(
    mojo::PendingReceiver<luxxle_news::mojom::LuxxleNewsController> receiver) {
  if (!controller_) {
    return;
  }

  controller_->Bind(std::move(receiver));
}

void LuxxleNewsInternalsUI::BindInterface(
    mojo::PendingReceiver<luxxle_news::mojom::LuxxleNewsInternals> receiver) {
  if (!controller_) {
    return;
  }

  controller_->Bind(std::move(receiver));
}
