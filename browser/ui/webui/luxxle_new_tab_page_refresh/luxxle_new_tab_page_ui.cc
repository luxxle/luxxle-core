// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/luxxle_new_tab_page_ui.h"

#include <utility>

// REMOVED: #include "luxxle/browser/luxxle_ads/.*"
#include "luxxle/browser/luxxle_browser_process.h"
#include "luxxle/browser/luxxle_news/luxxle_news_controller_factory.h"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/browser/misc_metrics/process_misc_metrics.h"
#include "luxxle/browser/ntp_background/luxxle_ntp_custom_background_service_factory.h"
#include "luxxle/browser/ntp_background/custom_background_file_manager.h"
#include "luxxle/browser/ntp_background/ntp_p3a_helper_impl.h"
#include "luxxle/browser/ntp_background/view_counter_service_factory.h"
#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/background_facade.h"
#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/custom_image_chooser.h"
#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/new_tab_page_handler.h"
#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/new_tab_page_initializer.h"
#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/top_sites_facade.h"
#include "luxxle/browser/ui/webui/luxxle_new_tab_page_refresh/vpn_facade.h"
#include "luxxle/browser/ui/webui/luxxle_rewards/rewards_page_handler.h"
#include "luxxle/components/luxxle_news/browser/luxxle_news_controller.h"
#include "luxxle/components/ntp_background_images/browser/ntp_sponsored_rich_media_ad_event_handler.h"
#include "luxxle/components/ntp_background_images/browser/view_counter_service.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/ntp_tiles/chrome_most_visited_sites_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/ui/webui/searchbox/realbox_handler.h"
#include "components/tabs/public/tab_interface.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

namespace {

using luxxle_new_tab_page_refresh::BackgroundFacade;
using luxxle_new_tab_page_refresh::CustomImageChooser;
using luxxle_new_tab_page_refresh::NewTabPageHandler;
using luxxle_new_tab_page_refresh::NewTabPageInitializer;
using luxxle_new_tab_page_refresh::TopSitesFacade;
using luxxle_new_tab_page_refresh::VPNFacade;

}  // namespace

LuxxleNewTabPageUI::LuxxleNewTabPageUI(content::WebUI* web_ui)
    : ui::MojoWebUIController(web_ui, /* enable_chrome_send = */ true) {
  NewTabPageInitializer(*web_ui).Initialize();
}

LuxxleNewTabPageUI::~LuxxleNewTabPageUI() = default;

void LuxxleNewTabPageUI::BindInterface(
    mojo::PendingReceiver<luxxle_new_tab_page_refresh::mojom::NewTabPageHandler>
        receiver) {
  auto* web_contents = web_ui()->GetWebContents();
  auto* profile = Profile::FromWebUI(web_ui());
  auto* prefs = profile->GetPrefs();
  auto* tab = tabs::TabInterface::GetFromContents(web_contents);
  auto image_chooser =
      std::make_unique<CustomImageChooser>(*web_contents, *profile);
  auto background_facade = std::make_unique<BackgroundFacade>(
      std::make_unique<CustomBackgroundFileManager>(profile), *prefs,
      g_luxxle_browser_process->ntp_background_images_service(),
      ntp_background_images::ViewCounterServiceFactory::GetForProfile(profile));
  auto top_sites_facade = std::make_unique<TopSitesFacade>(
      ChromeMostVisitedSitesFactory::NewForProfile(profile), *prefs);

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  auto vpn_facade = std::make_unique<VPNFacade>(
      *tab, luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(profile));
#else
  auto vpn_facade = std::make_unique<VPNFacade>();
#endif

  page_handler_ = std::make_unique<NewTabPageHandler>(
      std::move(receiver), std::move(image_chooser),
      std::move(background_facade), std::move(top_sites_facade),
      std::move(vpn_facade), *tab, *prefs,
      *TemplateURLServiceFactory::GetForProfile(profile),
      *g_luxxle_browser_process->process_misc_metrics()->new_tab_metrics());
}

void LuxxleNewTabPageUI::BindInterface(
    mojo::PendingReceiver<
        ntp_background_images::mojom::SponsoredRichMediaAdEventHandler>
        receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  ntp_background_images::NTPP3AHelper* ntp_p3a_helper = nullptr;
  if (ntp_background_images::ViewCounterService* view_counter_service =
          ntp_background_images::ViewCounterServiceFactory::GetForProfile(
              profile)) {
    ntp_p3a_helper = view_counter_service->GetP3AHelper();
  }
  rich_media_ad_event_handler_ = std::make_unique<
      ntp_background_images::NTPSponsoredRichMediaAdEventHandler>(
      luxxle_ads::AdsServiceFactory::GetForProfile(profile), ntp_p3a_helper);
  rich_media_ad_event_handler_->Bind(std::move(receiver));
}

void LuxxleNewTabPageUI::BindInterface(
    mojo::PendingReceiver<searchbox::mojom::PageHandler> receiver) {
  realbox_handler_ = std::make_unique<RealboxHandler>(
      std::move(receiver), Profile::FromWebUI(web_ui()),
      web_ui()->GetWebContents(), /*metrics_reporter=*/nullptr,
      /*omnibox_controller=*/nullptr);
}

void LuxxleNewTabPageUI::BindInterface(
    mojo::PendingReceiver<luxxle_rewards::mojom::RewardsPageHandler> receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  rewards_page_handler_ = std::make_unique<luxxle_rewards::RewardsPageHandler>(
      std::move(receiver), nullptr,
      luxxle_rewards::RewardsServiceFactory::GetForProfile(profile),
      luxxle_ads::AdsServiceFactory::GetForProfile(profile), nullptr,
      profile->GetPrefs());
}

void LuxxleNewTabPageUI::BindInterface(
    mojo::PendingReceiver<luxxle_news::mojom::LuxxleNewsController> receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  auto* luxxle_news_controller =
      luxxle_news::LuxxleNewsControllerFactory::GetForBrowserContext(profile);
  if (luxxle_news_controller) {
    luxxle_news_controller->Bind(std::move(receiver));
  }
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
void LuxxleNewTabPageUI::BindInterface(
    mojo::PendingReceiver<luxxle_vpn::mojom::ServiceHandler> receiver) {
  auto* vpn_service = luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(
      Profile::FromWebUI(web_ui()));
  if (vpn_service) {
    vpn_service->BindInterface(std::move(receiver));
  }
}
#endif

WEB_UI_CONTROLLER_TYPE_IMPL(LuxxleNewTabPageUI)
