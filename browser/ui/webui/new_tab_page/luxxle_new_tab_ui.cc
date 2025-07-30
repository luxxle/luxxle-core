// Copyright (c) 2019 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/new_tab_page/luxxle_new_tab_ui.h"

#include <utility>

#include "base/check.h"
#include "base/check_is_test.h"
#include "base/feature_list.h"
#include "base/strings/stringprintf.h"
#include "luxxle/browser/luxxle_browser_process.h"
#include "luxxle/browser/luxxle_news/luxxle_news_controller_factory.h"
#include "luxxle/browser/misc_metrics/process_misc_metrics.h"
#include "luxxle/browser/new_tab/new_tab_shows_options.h"
#include "luxxle/browser/ntp_background/luxxle_ntp_custom_background_service_factory.h"
#include "luxxle/browser/ntp_background/ntp_p3a_helper_impl.h"
#include "luxxle/browser/ui/luxxle_ui_features.h"
#include "luxxle/browser/ui/webui/luxxle_webui_source.h"
#include "luxxle/browser/ui/webui/new_tab_page/luxxle_new_tab_message_handler.h"
#include "luxxle/browser/ui/webui/new_tab_page/luxxle_new_tab_page_handler.h"
#include "luxxle/browser/ui/webui/new_tab_page/top_sites_message_handler.h"
// REMOVED: #include "luxxle/components/luxxle_ads/.*"
#include "luxxle/components/luxxle_new_tab/resources/grit/luxxle_new_tab_generated_map.h"
#include "luxxle/components/luxxle_news/browser/luxxle_news_controller.h"
#include "luxxle/components/luxxle_news/common/features.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "luxxle/components/misc_metrics/new_tab_metrics.h"
#include "luxxle/components/ntp_background_images/browser/ntp_custom_images_source.h"
#include "luxxle/components/ntp_background_images/browser/ntp_sponsored_rich_media_ad_event_handler.h"
#include "luxxle/components/ntp_background_images/browser/view_counter_service.h"
#include "luxxle/components/ntp_background_images/common/url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/themes/theme_syncable_service.h"
#include "chrome/browser/ui/webui/sanitized_image_source.h"
#include "components/country_codes/country_codes.h"
#include "components/grit/luxxle_components_resources.h"
#include "components/prefs/pref_service.h"
#include "components/regional_capabilities/regional_capabilities_country_id.h"
#include "components/strings/grit/components_strings.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/url_data_source.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/common/url_constants.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/webui/resources/cr_components/searchbox/searchbox.mojom.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

using ntp_background_images::NTPCustomImagesSource;

namespace {

std::string GetSearchWidgetDefaultHost(
    regional_capabilities::RegionalCapabilitiesService* regional_capabilities) {
  constexpr char kLuxxleSearchHost[] = "search.luxxle.com";
  constexpr char kYahooSearchHost[] = "search.yahoo.co.jp";

  regional_capabilities::CountryIdHolder country_id =
      regional_capabilities->GetCountryId();
  regional_capabilities::CountryIdHolder japan_country_id(
      country_codes::CountryId("JP"));
  if (country_id == japan_country_id) {
    return kYahooSearchHost;
  }

  return kLuxxleSearchHost;
}

}  // namespace

LuxxleNewTabUI::LuxxleNewTabUI(
    content::WebUI* web_ui,
    const std::string& name,
    luxxle_ads::AdsService* ads_service,
    ntp_background_images::ViewCounterService* view_counter_service,
    regional_capabilities::RegionalCapabilitiesService* regional_capabilities)
    : ui::MojoWebUIController(
          web_ui,
          true /* Needed for legacy non-mojom message handler */),
      page_factory_receiver_(this),
      regional_capabilities_(regional_capabilities) {
  content::WebContents* web_contents = web_ui->GetWebContents();
  CHECK(web_contents);

  content::NavigationEntry* navigation_entry =
      web_contents->GetController().GetLastCommittedEntry();
  const bool was_restored =
      navigation_entry ? navigation_entry->IsRestored() : false;

  Profile* profile = Profile::FromWebUI(web_ui);
  web_ui->OverrideTitle(l10n_util::GetStringUTF16(IDS_NEW_TAB_TITLE));

  if (luxxle::ShouldNewTabShowBlankpage(profile)) {
    content::WebUIDataSource* source =
        content::WebUIDataSource::CreateAndAdd(profile, name);
    source->SetDefaultResource(/* REMOVED: IDR_LUXXLE_BLANK_NEW_TAB_HTML */ 0);
    AddBackgroundColorToSource(source, web_contents);
    return;
  }

  // Non blank NTP.
  content::WebUIDataSource* source = CreateAndAddWebUIDataSource(
      web_ui, name, kLuxxleNewTabGenerated, /* REMOVED: IDR_LUXXLE_NEW_TAB_HTML */ 0);

  web_ui->AddRequestableScheme(content::kChromeUIUntrustedScheme);

  AddBackgroundColorToSource(source, web_contents);

  // Lottie animations tick on a worker thread and requires the document CSP to
  // be set to "worker-src blob: 'self';".
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::WorkerSrc,
      "worker-src blob: chrome://resources 'self';");

  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::TrustedTypes,
      "trusted-types static-types lottie-worker-script-loader lit-html-desktop "
      "default; ");

  source->AddBoolean(
      "featureCustomBackgroundEnabled",
      !profile->GetPrefs()->IsManagedPreference(GetThemePrefNameInMigration(
          ThemePrefInMigration::kNtpCustomBackgroundDict)));

  // Let frontend know about feature flags
  source->AddBoolean("featureFlagLuxxleNewsPromptEnabled",
                     base::FeatureList::IsEnabled(
                         luxxle_news::features::kLuxxleNewsCardPeekFeature));

  source->AddBoolean(
      "featureFlagLuxxleNewsFeedV2Enabled",
      base::FeatureList::IsEnabled(luxxle_news::features::kLuxxleNewsFeedUpdate));

  source->AddBoolean(
      "featureFlagSearchWidget",
      base::FeatureList::IsEnabled(features::kLuxxleNtpSearchWidget));
  source->AddString("searchWidgetDefaultHost",
                    GetSearchWidgetDefaultHost(regional_capabilities_));

  source->AddString("newTabTakeoverLearnMoreLinkUrl",
                    ntp_background_images::kNewTabTakeoverLearnMoreLinkUrl);

  source->AddBoolean("vpnWidgetSupported",
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
                     luxxle_vpn::IsLuxxleVPNEnabled(profile->GetPrefs())
#else
                     false
#endif
  );

  web_ui->AddMessageHandler(base::WrapUnique(
      LuxxleNewTabMessageHandler::Create(source, profile, was_restored)));
  web_ui->AddMessageHandler(
      base::WrapUnique(new TopSitesMessageHandler(profile)));

  // For custom background images.
  if (auto* ntp_custom_background_images_service =
          LuxxleNTPCustomBackgroundServiceFactory::GetForContext(profile)) {
    content::URLDataSource::Add(profile,
                                std::make_unique<NTPCustomImagesSource>(
                                    ntp_custom_background_images_service));
  }

  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FrameSrc,
      base::StringPrintf("frame-src %s;", kNTPNewTabTakeoverRichMediaUrl));
  source->AddString("ntpNewTabTakeoverRichMediaUrl",
                    kNTPNewTabTakeoverRichMediaUrl);

  ntp_background_images::NTPP3AHelper* ntp_p3a_helper = nullptr;
  if (view_counter_service != nullptr) {
    ntp_p3a_helper = view_counter_service->GetP3AHelper();
  }
  rich_media_ad_event_handler_ = std::make_unique<
      ntp_background_images::NTPSponsoredRichMediaAdEventHandler>(
      ads_service, ntp_p3a_helper);

  // Add a SanitizedImageSource to allow fetching images for Luxxle News.
  content::URLDataSource::Add(profile,
                              std::make_unique<SanitizedImageSource>(profile));
}

LuxxleNewTabUI::~LuxxleNewTabUI() = default;

void LuxxleNewTabUI::BindInterface(
    mojo::PendingReceiver<luxxle_news::mojom::LuxxleNewsController> receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  DCHECK(profile);
  // Wire up JS mojom to service
  auto* luxxle_news_controller =
      luxxle_news::LuxxleNewsControllerFactory::GetForBrowserContext(profile);
  if (luxxle_news_controller) {
    luxxle_news_controller->Bind(std::move(receiver));
  }
}

void LuxxleNewTabUI::BindInterface(
    mojo::PendingReceiver<luxxle_new_tab_page::mojom::PageHandlerFactory>
        pending_receiver) {
  if (page_factory_receiver_.is_bound()) {
    page_factory_receiver_.reset();
  }

  page_factory_receiver_.Bind(std::move(pending_receiver));
}

void LuxxleNewTabUI::BindInterface(
    mojo::PendingReceiver<searchbox::mojom::PageHandler> pending_page_handler) {
  auto* profile = Profile::FromWebUI(web_ui());
  DCHECK(profile);

  realbox_handler_ = std::make_unique<RealboxHandler>(
      std::move(pending_page_handler), profile, web_ui()->GetWebContents(),
      /*metrics_reporter=*/nullptr,
      /*omnibox_controller=*/nullptr);
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
void LuxxleNewTabUI::BindInterface(
    mojo::PendingReceiver<luxxle_vpn::mojom::ServiceHandler>
        pending_vpn_service_handler) {
  auto* profile = Profile::FromWebUI(web_ui());
  CHECK(profile);
  auto* vpn_service = luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(profile);
  if (vpn_service) {
    vpn_service->BindInterface(std::move(pending_vpn_service_handler));
  }
}
#endif

void LuxxleNewTabUI::CreatePageHandler(
    mojo::PendingRemote<luxxle_new_tab_page::mojom::Page> pending_page,
    mojo::PendingReceiver<luxxle_new_tab_page::mojom::PageHandler>
        pending_page_handler,
    mojo::PendingReceiver<luxxle_new_tab_page::mojom::NewTabMetrics>
        pending_new_tab_metrics,
    mojo::PendingReceiver<
        ntp_background_images::mojom::SponsoredRichMediaAdEventHandler>
        pending_rich_media_ad_event_handler) {
  DCHECK(pending_page.is_valid());
  Profile* profile = Profile::FromWebUI(web_ui());
  page_handler_ = std::make_unique<LuxxleNewTabPageHandler>(
      std::move(pending_page_handler), std::move(pending_page), profile,
      web_ui()->GetWebContents());
  g_luxxle_browser_process->process_misc_metrics()->new_tab_metrics()->Bind(
      std::move(pending_new_tab_metrics));
  rich_media_ad_event_handler_->Bind(
      std::move(pending_rich_media_ad_event_handler));
}

WEB_UI_CONTROLLER_TYPE_IMPL(LuxxleNewTabUI)
