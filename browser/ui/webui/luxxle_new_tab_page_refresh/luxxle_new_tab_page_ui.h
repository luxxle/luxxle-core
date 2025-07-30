// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_LUXXLE_NEW_TAB_PAGE_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_LUXXLE_NEW_TAB_PAGE_UI_H_

#include <memory>

#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/luxxle_new_tab_page.mojom.h"
#include "luxxle/components/luxxle_news/common/luxxle_news.mojom-forward.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/ntp_background_images/browser/mojom/ntp_background_images.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "ui/webui/mojo_web_ui_controller.h"
#include "ui/webui/resources/cr_components/searchbox/searchbox.mojom.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

namespace ntp_background_images {
class NTPSponsoredRichMediaAdEventHandler;
}

namespace luxxle_rewards {
class RewardsPageHandler;
}

class RealboxHandler;

// The Web UI controller for the Luxxle new tab page.
class LuxxleNewTabPageUI : public ui::MojoWebUIController {
 public:
  explicit LuxxleNewTabPageUI(content::WebUI* web_ui);
  ~LuxxleNewTabPageUI() override;

  void BindInterface(
      mojo::PendingReceiver<
          luxxle_new_tab_page_refresh::mojom::NewTabPageHandler> receiver);

  void BindInterface(
      mojo::PendingReceiver<
          ntp_background_images::mojom::SponsoredRichMediaAdEventHandler>
          receiver);

  void BindInterface(
      mojo::PendingReceiver<searchbox::mojom::PageHandler> receiver);

  void BindInterface(
      mojo::PendingReceiver<luxxle_rewards::mojom::RewardsPageHandler> receiver);

  void BindInterface(
      mojo::PendingReceiver<luxxle_news::mojom::LuxxleNewsController> receiver);

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  void BindInterface(
      mojo::PendingReceiver<luxxle_vpn::mojom::ServiceHandler> receiver);
#endif

 private:
  std::unique_ptr<luxxle_new_tab_page_refresh::mojom::NewTabPageHandler>
      page_handler_;
  std::unique_ptr<ntp_background_images::NTPSponsoredRichMediaAdEventHandler>
      rich_media_ad_event_handler_;
  std::unique_ptr<RealboxHandler> realbox_handler_;
  std::unique_ptr<luxxle_rewards::RewardsPageHandler> rewards_page_handler_;

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_NEW_TAB_PAGE_REFRESH_LUXXLE_NEW_TAB_PAGE_UI_H_
