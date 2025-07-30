/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_wallet/ledger/ledger_ui.h"

#include <string>

#include "luxxle/components/constants/webui_url_constants.h"
#include "luxxle/components/ledger_bridge/resources/grit/ledger_bridge_generated_map.h"
#include "components/grit/luxxle_components_resources.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui_data_source.h"
#include "ui/webui/resources/grit/webui_resources.h"

namespace ledger {

UntrustedLedgerUI::UntrustedLedgerUI(content::WebUI* web_ui)
    : ui::UntrustedWebUIController(web_ui) {
  auto* untrusted_source = content::WebUIDataSource::CreateAndAdd(
      web_ui->GetWebContents()->GetBrowserContext(), kUntrustedLedgerURL);
  untrusted_source->SetDefaultResource(/* REMOVED: IDR_LUXXLE_WALLET_LEDGER_BRIDGE_HTML */ 0);
  untrusted_source->AddResourcePaths(kLedgerBridgeGenerated);
  untrusted_source->AddFrameAncestor(GURL(kLuxxleUIWalletPageURL));
  untrusted_source->AddFrameAncestor(GURL(kLuxxleUIWalletPanelURL));
  untrusted_source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::StyleSrc,
      std::string("style-src 'unsafe-inline';"));
  untrusted_source->AddResourcePath("load_time_data_deprecated.js",
                                    IDR_WEBUI_JS_LOAD_TIME_DATA_DEPRECATED_JS);
  untrusted_source->UseStringsJs();
  untrusted_source->AddString("luxxleWalletLedgerBridgeUrl",
                              kUntrustedLedgerURL);
}

UntrustedLedgerUI::~UntrustedLedgerUI() = default;

std::unique_ptr<content::WebUIController>
UntrustedLedgerUIConfig::CreateWebUIController(content::WebUI* web_ui,
                                               const GURL& url) {
  return std::make_unique<UntrustedLedgerUI>(web_ui);
}

UntrustedLedgerUIConfig::UntrustedLedgerUIConfig()
    : WebUIConfig(content::kChromeUIUntrustedScheme, kUntrustedLedgerHost) {}

}  // namespace ledger
