// Copyright (c) 2021 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/luxxle_shields/shields_panel_ui.h"

#include <utility>

#include "luxxle/browser/ui/luxxle_browser_window.h"
#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_util.h"
#include "luxxle/components/luxxle_shields/core/common/luxxle_shield_localized_strings.h"
#include "luxxle/components/luxxle_shields/core/common/features.h"
#include "luxxle/components/luxxle_shields/resources/panel/grit/luxxle_shields_panel_generated_map.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "luxxle/components/webcompat/core/common/features.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/webui/favicon_source.h"
#include "components/favicon_base/favicon_url_parser.h"
#include "components/grit/luxxle_components_resources.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "net/base/features.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/webui/webui_util.h"

// Cache active Browser instance's TabStripModel to give
// to ShieldsPanelDataHandler when this is created because
// CreatePanelHandler() is run in async.
ShieldsPanelUI::ShieldsPanelUI(content::WebUI* web_ui)
    : TopChromeWebUIController(web_ui, true),
      profile_(Profile::FromWebUI(web_ui)) {
  browser_ = chrome::FindLastActiveWithProfile(profile_);

  content::WebUIDataSource* source = content::WebUIDataSource::CreateAndAdd(
      web_ui->GetWebContents()->GetBrowserContext(), kShieldsPanelHost);

  for (const auto& str : luxxle_shields::kLocalizedStrings) {
    std::u16string l10n_str = l10n_util::GetStringUTF16(str.id);
    source->AddString(str.name, l10n_str);
  }

  source->AddBoolean("isAdvancedViewEnabled", profile_->GetPrefs()->GetBoolean(
                                                  kShieldsAdvancedViewEnabled));

  source->AddBoolean("isHttpsByDefaultEnabled",
                     luxxle_shields::IsHttpsByDefaultFeatureEnabled());

  source->AddBoolean(
      "showStrictFingerprintingMode",
      base::FeatureList::IsEnabled(
          luxxle_shields::features::kLuxxleShowStrictFingerprintingMode));

  source->AddBoolean("isTorProfile", profile_->IsTor());

  source->AddBoolean("isForgetFirstPartyStorageEnabled",
                     base::FeatureList::IsEnabled(
                         net::features::kLuxxleForgetFirstPartyStorage));

  source->AddBoolean(
      "isWebcompatExceptionsServiceEnabled",
      base::FeatureList::IsEnabled(
          webcompat::features::kLuxxleWebcompatExceptionsService));

  content::URLDataSource::Add(
      profile_, std::make_unique<FaviconSource>(
                    profile_, chrome::FaviconUrlFormat::kFavicon2));
  webui::SetupWebUIDataSource(source, kLuxxleShieldsPanelGenerated,
                              IDR_SHIELDS_PANEL_HTML);
}

ShieldsPanelUI::~ShieldsPanelUI() = default;

WEB_UI_CONTROLLER_TYPE_IMPL(ShieldsPanelUI)

void ShieldsPanelUI::BindInterface(
    mojo::PendingReceiver<luxxle_shields::mojom::PanelHandlerFactory> receiver) {
  panel_factory_receiver_.reset();
  panel_factory_receiver_.Bind(std::move(receiver));
}

void ShieldsPanelUI::CreatePanelHandler(
    mojo::PendingReceiver<luxxle_shields::mojom::PanelHandler> panel_receiver,
    mojo::PendingReceiver<luxxle_shields::mojom::DataHandler>
        data_handler_receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  DCHECK(profile);

  panel_handler_ = std::make_unique<ShieldsPanelHandler>(
      std::move(panel_receiver), this,
      static_cast<LuxxleBrowserWindow*>(browser_->window()), profile);
  data_handler_ = std::make_unique<ShieldsPanelDataHandler>(
      std::move(data_handler_receiver), this, browser_->tab_strip_model());
}

ShieldsPanelUIConfig::ShieldsPanelUIConfig()
    : DefaultTopChromeWebUIConfig(content::kChromeUIScheme, kShieldsPanelHost) {
}

bool ShieldsPanelUIConfig::IsWebUIEnabled(
    content::BrowserContext* browser_context) {
  return true;
}

bool ShieldsPanelUIConfig::ShouldAutoResizeHost() {
  return true;
}
