/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/welcome_page/luxxle_welcome_ui.h"

#include <algorithm>
#include <memory>
#include <string>

#include "base/feature_list.h"
#include "base/memory/raw_ptr.h"
#include "base/task/single_thread_task_runner.h"
#include "luxxle/browser/luxxle_browser_features.h"
#include "luxxle/browser/ui/webui/luxxle_webui_source.h"
#include "luxxle/browser/ui/webui/settings/luxxle_import_bulk_data_handler.h"
#include "luxxle/browser/ui/webui/settings/luxxle_search_engines_handler.h"
#include "luxxle/browser/ui/webui/welcome_page/welcome_dom_handler.h"
#include "luxxle/components/luxxle_welcome/common/features.h"
#include "luxxle/components/luxxle_welcome/resources/grit/luxxle_welcome_generated_map.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/regional_capabilities/regional_capabilities_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/webui/settings/privacy_sandbox_handler.h"
#include "chrome/browser/ui/webui/settings/settings_default_browser_handler.h"
#include "chrome/common/pref_names.h"
#include "chrome/grit/branded_strings.h"
#include "components/country_codes/country_codes.h"
#include "components/grit/luxxle_components_resources.h"
#include "components/grit/luxxle_components_strings.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/gpu_data_manager.h"
#include "content/public/browser/page_navigator.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/browser/web_ui_message_handler.h"
#include "ui/base/l10n/l10n_util.h"

namespace {

constexpr webui::LocalizedString kLocalizedStrings[] = {
    {"headerText", IDS_WELCOME_HEADER},
    {"luxxleWelcomeTitle", IDS_LUXXLE_WELCOME_TITLE},
    {"luxxleWelcomeDesc", IDS_LUXXLE_WELCOME_DESC},
    {"luxxleWelcomeImportSettingsTitle",
     IDS_LUXXLE_WELCOME_IMPORT_SETTINGS_TITLE},
    {"luxxleWelcomeImportSettingsDesc", IDS_LUXXLE_WELCOME_IMPORT_SETTINGS_DESC},
    {"luxxleWelcomeSelectProfileLabel", IDS_LUXXLE_WELCOME_SELECT_PROFILE_LABEL},
    {"luxxleWelcomeSelectProfileDesc", IDS_LUXXLE_WELCOME_SELECT_PROFILE_DESC},
    {"luxxleWelcomeImportButtonLabel", IDS_LUXXLE_WELCOME_IMPORT_BUTTON_LABEL},
    {"luxxleWelcomeImportProfilesButtonLabel",
     IDS_LUXXLE_WELCOME_IMPORT_PROFILES_BUTTON_LABEL},
    {"luxxleWelcomeSkipButtonLabel", IDS_LUXXLE_WELCOME_SKIP_BUTTON_LABEL},
    {"luxxleWelcomeBackButtonLabel", IDS_LUXXLE_WELCOME_BACK_BUTTON_LABEL},
    {"luxxleWelcomeNextButtonLabel", IDS_LUXXLE_WELCOME_NEXT_BUTTON_LABEL},
    {"luxxleWelcomeFinishButtonLabel", IDS_LUXXLE_WELCOME_FINISH_BUTTON_LABEL},
    {"luxxleWelcomeSetDefaultButtonLabel",
     IDS_LUXXLE_WELCOME_SET_DEFAULT_BUTTON_LABEL},
    {"luxxleWelcomeSelectAllButtonLabel",
     IDS_LUXXLE_WELCOME_SELECT_ALL_BUTTON_LABEL},
    {"luxxleWelcomeHelpImproveLuxxleTitle",
     IDS_LUXXLE_WELCOME_HELP_IMPROVE_LUXXLE_TITLE},
    {"luxxleWelcomeSendReportsLabel", IDS_LUXXLE_WELCOME_SEND_REPORTS_LABEL},
    {"luxxleWelcomeSendInsightsLabel", IDS_LUXXLE_WELCOME_SEND_INSIGHTS_LABEL},
    {"luxxleWelcomeSetupCompleteLabel", IDS_LUXXLE_WELCOME_SETUP_COMPLETE_LABEL},
    {"luxxleWelcomeChangeSettingsNote", IDS_LUXXLE_WELCOME_CHANGE_SETTINGS_NOTE},
    {"luxxleWelcomePrivacyPolicyNote", IDS_LUXXLE_WELCOME_PRIVACY_POLICY_NOTE},
    {"luxxleWelcomeSelectThemeLabel", IDS_LUXXLE_WELCOME_SELECT_THEME_LABEL},
    {"luxxleWelcomeSelectThemeNote", IDS_LUXXLE_WELCOME_SELECT_THEME_NOTE},
    {"luxxleWelcomeSelectThemeSystemLabel",
     IDS_LUXXLE_WELCOME_SELECT_THEME_SYSTEM_LABEL},
    {"luxxleWelcomeSelectThemeLightLabel",
     IDS_LUXXLE_WELCOME_SELECT_THEME_LIGHT_LABEL},
    {"luxxleWelcomeSelectThemeDarkLabel",
     IDS_LUXXLE_WELCOME_SELECT_THEME_DARK_LABEL},
    {"luxxleWelcomeHelpWDPTitle", IDS_LUXXLE_WELCOME_HELP_WDP_TITLE},
    {"luxxleWelcomeHelpWDPSubtitle", IDS_LUXXLE_WELCOME_HELP_WDP_SUBTITLE},
    {"luxxleWelcomeHelpWDPDescription", IDS_LUXXLE_WELCOME_HELP_WDP_DESCRIPTION},
    {"luxxleWelcomeHelpWDPLearnMore", IDS_LUXXLE_WELCOME_HELP_WDP_LEARN_MORE},
    {"luxxleWelcomeHelpWDPAccept", IDS_LUXXLE_WELCOME_HELP_WDP_ACCEPT},
    {"luxxleWelcomeHelpWDPReject", IDS_LUXXLE_WELCOME_HELP_WDP_REJECT}};

void OpenJapanWelcomePage(Profile* profile) {
  DCHECK(profile);
  Browser* browser = chrome::FindBrowserWithProfile(profile);
  if (browser) {
    content::OpenURLParams open_params(
        GURL("https://luxxle.com/ja/desktop-ntp-tutorial"), content::Referrer(),
        WindowOpenDisposition::NEW_BACKGROUND_TAB,
        ui::PAGE_TRANSITION_AUTO_TOPLEVEL, false);
    browser->OpenURL(open_params, /*navigation_handle_callback=*/{});
  }
}

}  // namespace

LuxxleWelcomeUI::LuxxleWelcomeUI(content::WebUI* web_ui, const std::string& name)
    : WebUIController(web_ui) {
  content::WebUIDataSource* source = CreateAndAddWebUIDataSource(
      web_ui, name, kLuxxleWelcomeGenerated, IDR_LUXXLE_WELCOME_HTML,
      /*disable_trusted_types_csp=*/true);

  // Lottie animations tick on a worker thread and requires the document CSP to
  // be set to "worker-src blob: 'self';".
  source->OverrideContentSecurityPolicy(
      network::mojom::CSPDirectiveName::WorkerSrc,
      "worker-src blob: chrome://resources 'self';");

  web_ui->AddMessageHandler(
      std::make_unique<WelcomeDOMHandler>(Profile::FromWebUI(web_ui)));
  web_ui->AddMessageHandler(
      std::make_unique<settings::LuxxleImportBulkDataHandler>());
  web_ui->AddMessageHandler(
      std::make_unique<settings::DefaultBrowserHandler>());  // set default
                                                             // browser

  Profile* profile = Profile::FromWebUI(web_ui);
  // added to allow front end to read/modify default search engine
  web_ui->AddMessageHandler(std::make_unique<
                            settings::LuxxleSearchEnginesHandler>(
      profile,
      regional_capabilities::RegionalCapabilitiesServiceFactory::GetForProfile(
          profile)));

  // Open additional page in Japanese region
  country_codes::CountryId country_id =
      country_codes::GetCountryIDFromPrefs(profile->GetPrefs());
  const bool is_jpn = country_id == country_codes::CountryId("JP");
  if (!profile->GetPrefs()->GetBoolean(prefs::kHasSeenWelcomePage)) {
    if (is_jpn) {
      base::SingleThreadTaskRunner::GetCurrentDefault()->PostDelayedTask(
          FROM_HERE, base::BindOnce(&OpenJapanWelcomePage, profile),
          base::Seconds(3));
    }
  }

  for (const auto& str : kLocalizedStrings) {
    std::u16string l10n_str = l10n_util::GetStringUTF16(str.id);
    source->AddString(str.name, l10n_str);
  }

  // Variables considered when determining which onboarding cards to show
  source->AddString("countryString", country_id.CountryCode());
  source->AddBoolean(
      "showRewardsCard",
      base::FeatureList::IsEnabled(luxxle_welcome::features::kShowRewardsCard));

  source->AddBoolean(
      "hardwareAccelerationEnabledAtStartup",
      content::GpuDataManager::GetInstance()->HardwareAccelerationEnabled());

  profile->GetPrefs()->SetBoolean(prefs::kHasSeenWelcomePage, true);

  AddBackgroundColorToSource(source, web_ui->GetWebContents());
}

LuxxleWelcomeUI::~LuxxleWelcomeUI() = default;
