/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_wallet/market/market_ui.h"

#include <string>

#include "luxxle/browser/ui/webui/untrusted_sanitized_image_source.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/constants/webui_url_constants.h"
#include "luxxle/components/market_display/resources/grit/market_display_generated_map.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/grit/browser_resources.h"
#include "chrome/grit/generated_resources.h"
#include "components/grit/luxxle_components_resources.h"
#include "content/public/browser/url_data_source.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/webui/resources/grit/webui_resources.h"
#include "ui/webui/webui_util.h"

namespace market {

UntrustedMarketUI::UntrustedMarketUI(content::WebUI* web_ui)
    : ui::UntrustedWebUIController(web_ui) {
  auto* untrusted_source = content::WebUIDataSource::CreateAndAdd(
      web_ui->GetWebContents()->GetBrowserContext(), kUntrustedMarketURL);

  for (const auto& str : luxxle_wallet::kLocalizedStrings) {
    std::u16string l10n_str = l10n_util::GetStringUTF16(str.id);
    untrusted_source->AddString(str.name, l10n_str);
  }
  untrusted_source->SetDefaultResource(/* REMOVED: IDR_LUXXLE_WALLET_MARKET_DISPLAY_HTML */ 0);
  untrusted_source->AddResourcePaths(kMarketDisplayGenerated);
  untrusted_source->AddFrameAncestor(GURL(kLuxxleUIWalletPageURL));
  untrusted_source->AddFrameAncestor(GURL(kLuxxleUIWalletPanelURL));
  webui::SetupWebUIDataSource(untrusted_source, kMarketDisplayGenerated,
                              /* REMOVED: IDR_LUXXLE_WALLET_MARKET_DISPLAY_HTML */ 0);

  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ScriptSrc,
      std::string("script-src 'self' chrome-untrusted://resources;"));

  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::StyleSrc,
      std::string("style-src 'self' 'unsafe-inline';"));
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::StyleSrc,
      std::string(
          "style-src 'self' 'unsafe-inline' chrome-untrusted://resources;"));
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::FontSrc,
      std::string("font-src 'self' data: chrome-untrusted://resources;"));
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::ImgSrc,
      std::string("img-src 'self' https://assets.cgproxy.luxxle.com "
                  "chrome-untrusted://resources chrome-untrusted://image;"));

  untrusted_source->AddResourcePath("load_time_data_deprecated.js",
                                    IDR_WEBUI_JS_LOAD_TIME_DATA_DEPRECATED_JS);
  untrusted_source->UseStringsJs();
  untrusted_source->AddString("luxxleWalletTrezorBridgeUrl",
                              kUntrustedTrezorURL);
  untrusted_source->AddString("luxxleWalletLedgerBridgeUrl",
                              kUntrustedLedgerURL);
  untrusted_source->AddString("luxxleWalletNftBridgeUrl", kUntrustedNftURL);
  untrusted_source->AddString("luxxleWalletMarketUiBridgeUrl",
                              kUntrustedMarketURL);

  Profile* profile = Profile::FromWebUI(web_ui);
  content::URLDataSource::Add(
      profile, std::make_unique<UntrustedSanitizedImageSource>(profile));
}

UntrustedMarketUI::~UntrustedMarketUI() = default;

std::unique_ptr<content::WebUIController>
UntrustedMarketUIConfig::CreateWebUIController(content::WebUI* web_ui,
                                               const GURL& url) {
  return std::make_unique<UntrustedMarketUI>(web_ui);
}

UntrustedMarketUIConfig::UntrustedMarketUIConfig()
    : WebUIConfig(content::kChromeUIUntrustedScheme, kUntrustedMarketHost) {}

}  // namespace market
