// Copyright (c) 2019 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_LUXXLE_NEW_TAB_UI_H_
#define LUXXLE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_LUXXLE_NEW_TAB_UI_H_

#include <memory>
#include <string>

#include "base/memory/raw_ptr.h"
#include "luxxle/components/luxxle_new_tab_ui/luxxle_new_tab_page.mojom.h"
#include "luxxle/components/luxxle_news/common/luxxle_news.mojom.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/browser/ui/webui/searchbox/realbox_handler.h"
#include "components/regional_capabilities/regional_capabilities_service.h"
#include "content/public/browser/web_ui_controller.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_web_ui_controller.h"
#include "ui/webui/resources/cr_components/searchbox/searchbox.mojom.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"  // nogncheck
#endif  // BUILDFLAG(ENABLE_LUXXLE_VPN)

namespace luxxle_ads {
class AdsService;
}  // namespace luxxle_ads

namespace ntp_background_images {
class NTPSponsoredRichMediaAdEventHandler;
class ViewCounterService;
}  // namespace ntp_background_images

class LuxxleNewTabPageHandler;

class LuxxleNewTabUI : public ui::MojoWebUIController,
                      public luxxle_new_tab_page::mojom::PageHandlerFactory {
 public:
  LuxxleNewTabUI(content::WebUI* web_ui,
                const std::string& name,
                luxxle_ads::AdsService* ads_service,
                ntp_background_images::ViewCounterService* view_counter_service,
                regional_capabilities::RegionalCapabilitiesService*
                    regional_capabilities);
  ~LuxxleNewTabUI() override;
  LuxxleNewTabUI(const LuxxleNewTabUI&) = delete;
  LuxxleNewTabUI& operator=(const LuxxleNewTabUI&) = delete;

  // Instantiates the implementor of the mojo
  // interface passing the pending receiver that will be internally bound.
  void BindInterface(
      mojo::PendingReceiver<luxxle_news::mojom::LuxxleNewsController> receiver);

  void BindInterface(
      mojo::PendingReceiver<luxxle_new_tab_page::mojom::PageHandlerFactory>
          pending_receiver);

  void BindInterface(mojo::PendingReceiver<searchbox::mojom::PageHandler>
                         pending_page_handler);

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  void BindInterface(mojo::PendingReceiver<luxxle_vpn::mojom::ServiceHandler>
                         pending_vpn_service_handler);
#endif

 private:
  // new_tab_page::mojom::PageHandlerFactory:
  void CreatePageHandler(
      mojo::PendingRemote<luxxle_new_tab_page::mojom::Page> pending_page,
      mojo::PendingReceiver<luxxle_new_tab_page::mojom::PageHandler>
          pending_page_handler,
      mojo::PendingReceiver<luxxle_new_tab_page::mojom::NewTabMetrics>
          pending_new_tab_metrics,
      mojo::PendingReceiver<
          ntp_background_images::mojom::SponsoredRichMediaAdEventHandler>
          pending_rich_media_ad_event_handler) override;

  std::unique_ptr<LuxxleNewTabPageHandler> page_handler_;
  std::unique_ptr<RealboxHandler> realbox_handler_;
  mojo::Receiver<luxxle_new_tab_page::mojom::PageHandlerFactory>
      page_factory_receiver_;
  std::unique_ptr<ntp_background_images::NTPSponsoredRichMediaAdEventHandler>
      rich_media_ad_event_handler_;
  raw_ptr<regional_capabilities::RegionalCapabilitiesService>
      regional_capabilities_ = nullptr;

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // LUXXLE_BROWSER_UI_WEBUI_NEW_TAB_PAGE_LUXXLE_NEW_TAB_UI_H_
