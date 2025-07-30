/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/api/luxxle_rewards_api.h"

#include <string>
#include <utility>

#include "base/functional/bind.h"
#include "base/strings/string_number_conversions.h"
#include "luxxle/browser/luxxle_adaptive_captcha/luxxle_adaptive_captcha_service_factory.h"
// REMOVED: #include "luxxle/browser/luxxle_ads/.*"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/browser/ui/luxxle_rewards/rewards_panel_coordinator.h"
#include "luxxle/common/extensions/api/luxxle_rewards.h"
#include "luxxle/components/luxxle_adaptive_captcha/luxxle_adaptive_captcha_service.h"
// REMOVED: #include "luxxle/components/luxxle_ads/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/extensions/window_controller.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_finder.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/event_router.h"

using luxxle_ads::AdsService;
using luxxle_ads::AdsServiceFactory;
using luxxle_rewards::RewardsPanelCoordinator;
using luxxle_rewards::RewardsService;
using luxxle_rewards::RewardsServiceFactory;
using luxxle_rewards::RewardsTabHelper;

namespace {

RewardsTabHelper* GetRewardsTabHelperForTabId(
    int tab_id,
    content::BrowserContext* browser_context) {
  DCHECK(browser_context);
  content::WebContents* web_contents = nullptr;
  extensions::WindowController* window = nullptr;
  bool found = extensions::ExtensionTabUtil::GetTabById(
      tab_id, browser_context, /*incognito_enabled=*/false, &window,
      &web_contents, /*tab_index=*/nullptr);
  if (!found || !window || !web_contents) {
    return nullptr;
  }
  return RewardsTabHelper::FromWebContents(web_contents);
}

RewardsPanelCoordinator* GetPanelCoordinator(
    content::WebContents* web_contents) {
  DCHECK(web_contents);
  auto* browser = chrome::FindBrowserWithTab(web_contents);
  return browser ? RewardsPanelCoordinator::FromBrowser(browser) : nullptr;
}

RewardsPanelCoordinator* GetPanelCoordinator(ExtensionFunction* function) {
  DCHECK(function);
  auto* web_contents = function->GetSenderWebContents();
  if (!web_contents) {
    return nullptr;
  }
  return GetPanelCoordinator(web_contents);
}

std::string StringifyResult(
    luxxle_rewards::mojom::CreateRewardsWalletResult result) {
  switch (result) {
    case luxxle_rewards::mojom::CreateRewardsWalletResult::kSuccess:
      return "success";
    case luxxle_rewards::mojom::CreateRewardsWalletResult::
        kWalletGenerationDisabled:
      return "wallet-generation-disabled";
    case luxxle_rewards::mojom::CreateRewardsWalletResult::
        kGeoCountryAlreadyDeclared:
      return "country-already-declared";
    case luxxle_rewards::mojom::CreateRewardsWalletResult::kUnexpected:
      return "unexpected-error";
  }
}

}  // namespace

namespace extensions::api {

LuxxleRewardsIsSupportedFunction::~LuxxleRewardsIsSupportedFunction() = default;

ExtensionFunction::ResponseAction LuxxleRewardsIsSupportedFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  bool is_supported = ::luxxle_rewards::IsSupportedForProfile(profile);
  return RespondNow(WithArguments(is_supported));
}

LuxxleRewardsRecordNTPPanelTriggerFunction::
    ~LuxxleRewardsRecordNTPPanelTriggerFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsRecordNTPPanelTriggerFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  if (!profile->GetPrefs()->GetBoolean(::luxxle_rewards::prefs::kEnabled)) {
    rewards_service->GetP3AConversionMonitor()->RecordPanelTrigger(
        ::luxxle_rewards::p3a::PanelTrigger::kNTP);
  }

  return RespondNow(NoArguments());
}

LuxxleRewardsOpenRewardsPanelFunction::~LuxxleRewardsOpenRewardsPanelFunction() =
    default;

ExtensionFunction::ResponseAction LuxxleRewardsOpenRewardsPanelFunction::Run() {
  if (auto* coordinator = GetPanelCoordinator(this)) {
    coordinator->OpenRewardsPanel();
  }
  return RespondNow(NoArguments());
}

LuxxleRewardsGetPublisherInfoFunction::~LuxxleRewardsGetPublisherInfoFunction() =
    default;

ExtensionFunction::ResponseAction LuxxleRewardsGetPublisherInfoFunction::Run() {
  std::optional<luxxle_rewards::GetPublisherInfo::Params> params =
      luxxle_rewards::GetPublisherInfo::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetPublisherInfo(
      params->publisher_key,
      base::BindOnce(&LuxxleRewardsGetPublisherInfoFunction::OnGetPublisherInfo,
                     this));

  return RespondLater();
}

void LuxxleRewardsGetPublisherInfoFunction::OnGetPublisherInfo(
    const ::luxxle_rewards::mojom::Result result,
    ::luxxle_rewards::mojom::PublisherInfoPtr info) {
  if (!info) {
    Respond(WithArguments(static_cast<int>(result)));
    return;
  }

  base::Value::Dict dict;
  dict.Set("publisherKey", info->id);
  dict.Set("name", info->name);
  dict.Set("percentage", static_cast<int>(info->percent));
  dict.Set("status", static_cast<int>(info->status));
  dict.Set("excluded", info->excluded ==
                           ::luxxle_rewards::mojom::PublisherExclude::EXCLUDED);
  dict.Set("url", info->url);
  dict.Set("provider", info->provider);
  dict.Set("favIconUrl", info->favicon_url);

  Respond(WithArguments(static_cast<int>(result), std::move(dict)));
}

LuxxleRewardsGetPublisherInfoForTabFunction::
    ~LuxxleRewardsGetPublisherInfoForTabFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetPublisherInfoForTabFunction::Run() {
  auto params = luxxle_rewards::GetPublisherInfoForTab::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  auto* profile = Profile::FromBrowserContext(browser_context());

  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  auto* tab_helper = GetRewardsTabHelperForTabId(params->tab_id, profile);
  if (!tab_helper) {
    return RespondNow(NoArguments());
  }

  std::string publisher_id = tab_helper->GetPublisherIdForTab();
  if (publisher_id.empty()) {
    return RespondNow(NoArguments());
  }

  rewards_service->GetPublisherPanelInfo(
      publisher_id,
      base::BindOnce(
          &LuxxleRewardsGetPublisherInfoForTabFunction::OnGetPublisherPanelInfo,
          this));

  return RespondLater();
}

void LuxxleRewardsGetPublisherInfoForTabFunction::OnGetPublisherPanelInfo(
    ::luxxle_rewards::mojom::Result result,
    ::luxxle_rewards::mojom::PublisherInfoPtr info) {
  if (!info) {
    Respond(NoArguments());
    return;
  }

  base::Value::Dict dict;
  dict.Set("publisherKey", info->id);
  dict.Set("name", info->name);
  dict.Set("percentage", static_cast<int>(info->percent));
  dict.Set("status", static_cast<int>(info->status));
  dict.Set("excluded", info->excluded ==
                           ::luxxle_rewards::mojom::PublisherExclude::EXCLUDED);
  dict.Set("url", info->url);
  dict.Set("provider", info->provider);
  dict.Set("favIconUrl", info->favicon_url);

  Respond(WithArguments(std::move(dict)));
}

LuxxleRewardsGetRewardsParametersFunction::
    ~LuxxleRewardsGetRewardsParametersFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetRewardsParametersFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->GetRewardsParameters(base::BindOnce(
      &LuxxleRewardsGetRewardsParametersFunction::OnGetRewardsParameters, this));
  return RespondLater();
}

void LuxxleRewardsGetRewardsParametersFunction::OnGetRewardsParameters(
    ::luxxle_rewards::mojom::RewardsParametersPtr parameters) {
  base::Value::Dict data;

  if (!parameters) {
    data.Set("rate", 0.0);
    data.Set("monthlyTipChoices", base::Value::List());
    data.Set("payoutStatus", base::Value::Dict());
    data.Set("walletProviderRegions", base::Value::Dict());
    data.Set("vbatExpired", false);
    return Respond(WithArguments(std::move(data)));
  }

  data.Set("rate", parameters->rate);
  base::Value::List monthly_choices;
  for (auto const& item : parameters->monthly_tip_choices) {
    monthly_choices.Append(item);
  }
  data.Set("monthlyTipChoices", std::move(monthly_choices));

  base::Value::Dict payout_status;
  for (const auto& [key, value] : parameters->payout_status) {
    payout_status.Set(key, value);
  }
  data.Set("payoutStatus", std::move(payout_status));

  base::Value::Dict provider_regions;
  for (const auto& [provider, regions] : parameters->wallet_provider_regions) {
    base::Value::List allow;
    for (const auto& country : regions->allow) {
      allow.Append(country);
    }
    base::Value::List block;
    for (const auto& country : regions->block) {
      block.Append(country);
    }
    base::Value::Dict regions_dict;
    regions_dict.Set("allow", std::move(allow));
    regions_dict.Set("block", std::move(block));
    provider_regions.Set(provider, std::move(regions_dict));
  }
  data.Set("walletProviderRegions", std::move(provider_regions));

  if (!parameters->vbat_deadline.is_null()) {
    data.Set("vbatDeadline",
             floor(parameters->vbat_deadline.InSecondsFSinceUnixEpoch() *
                   base::Time::kMillisecondsPerSecond));
  }
  data.Set("vbatExpired", parameters->vbat_expired);

  Respond(WithArguments(std::move(data)));
}

LuxxleRewardsCreateRewardsWalletFunction::
    ~LuxxleRewardsCreateRewardsWalletFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsCreateRewardsWalletFunction::Run() {
  auto params = luxxle_rewards::CreateRewardsWallet::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("RewardsService not available"));
  }

  rewards_service->CreateRewardsWallet(
      params->country,
      base::BindOnce(
          &LuxxleRewardsCreateRewardsWalletFunction::CreateRewardsWalletCallback,
          this));

  return RespondLater();
}

void LuxxleRewardsCreateRewardsWalletFunction::CreateRewardsWalletCallback(
    ::luxxle_rewards::mojom::CreateRewardsWalletResult result) {
  Respond(WithArguments(StringifyResult(result)));
}

LuxxleRewardsGetAvailableCountriesFunction::
    ~LuxxleRewardsGetAvailableCountriesFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetAvailableCountriesFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetAvailableCountries(base::BindOnce(
      &LuxxleRewardsGetAvailableCountriesFunction::GetAvailableCountriesCallback,
      this));

  return RespondLater();
}

void LuxxleRewardsGetAvailableCountriesFunction::GetAvailableCountriesCallback(
    std::vector<std::string> countries) {
  base::Value::List country_list;
  for (auto& country : countries) {
    country_list.Append(std::move(country));
  }
  Respond(WithArguments(std::move(country_list)));
}

LuxxleRewardsGetDefaultCountryFunction::
    ~LuxxleRewardsGetDefaultCountryFunction() = default;

ExtensionFunction::ResponseAction LuxxleRewardsGetDefaultCountryFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  return RespondNow(WithArguments(rewards_service->GetCountryCode()));
}

LuxxleRewardsGetDeclaredCountryFunction::
    ~LuxxleRewardsGetDeclaredCountryFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetDeclaredCountryFunction::Run() {
  auto* prefs = Profile::FromBrowserContext(browser_context())->GetPrefs();
  std::string country = prefs->GetString(::luxxle_rewards::prefs::kDeclaredGeo);
  return RespondNow(WithArguments(std::move(country)));
}

LuxxleRewardsGetUserTypeFunction::~LuxxleRewardsGetUserTypeFunction() = default;

ExtensionFunction::ResponseAction LuxxleRewardsGetUserTypeFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(WithArguments(std::string()));
  }

  rewards_service->GetUserType(
      base::BindOnce(&LuxxleRewardsGetUserTypeFunction::Callback, this));

  return RespondLater();
}

void LuxxleRewardsGetUserTypeFunction::Callback(
    ::luxxle_rewards::mojom::UserType user_type) {
  auto map_user_type =
      [](::luxxle_rewards::mojom::UserType user_type) -> std::string {
    switch (user_type) {
      case ::luxxle_rewards::mojom::UserType::kConnected:
        return "connected";
      case ::luxxle_rewards::mojom::UserType::kUnconnected:
        return "unconnected";
    }
  };
  Respond(WithArguments(map_user_type(user_type)));
}

LuxxleRewardsGetPublishersVisitedCountFunction::
    ~LuxxleRewardsGetPublishersVisitedCountFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetPublishersVisitedCountFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(WithArguments(0));
  }

  rewards_service->GetPublishersVisitedCount(base::BindOnce(
      &LuxxleRewardsGetPublishersVisitedCountFunction::Callback, this));

  return RespondLater();
}

void LuxxleRewardsGetPublishersVisitedCountFunction::Callback(int count) {
  Respond(WithArguments(count));
}

LuxxleRewardsGetBalanceReportFunction::~LuxxleRewardsGetBalanceReportFunction() =
    default;

ExtensionFunction::ResponseAction LuxxleRewardsGetBalanceReportFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  std::optional<luxxle_rewards::GetBalanceReport::Params> params =
      luxxle_rewards::GetBalanceReport::Params::Create(args());

  rewards_service->GetBalanceReport(
      params->month, params->year,
      base::BindOnce(&LuxxleRewardsGetBalanceReportFunction::OnBalanceReport,
                     this));
  return RespondLater();
}

void LuxxleRewardsGetBalanceReportFunction::OnBalanceReport(
    const ::luxxle_rewards::mojom::Result result,
    ::luxxle_rewards::mojom::BalanceReportInfoPtr report) {
  base::Value::Dict data;
  data.Set("ads", report ? report->earning_from_ads : 0.0);
  data.Set("contribute", 0.0);
  data.Set("tips", report ? report->one_time_donation : 0.0);
  data.Set("monthly", report ? report->recurring_donation : 0.0);
  Respond(WithArguments(std::move(data)));
}

LuxxleRewardsSaveRecurringTipFunction::~LuxxleRewardsSaveRecurringTipFunction() =
    default;

ExtensionFunction::ResponseAction LuxxleRewardsSaveRecurringTipFunction::Run() {
  std::optional<luxxle_rewards::SaveRecurringTip::Params> params =
      luxxle_rewards::SaveRecurringTip::Params::Create(args());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(NoArguments());
  }

  rewards_service->SaveRecurringTip(
      params->publisher_key, params->new_amount,
      base::BindOnce(&LuxxleRewardsSaveRecurringTipFunction::OnSaveRecurringTip,
                     this));

  return RespondLater();
}

void LuxxleRewardsSaveRecurringTipFunction::OnSaveRecurringTip(
    ::luxxle_rewards::mojom::Result result) {
  Respond(result == ::luxxle_rewards::mojom::Result::OK
              ? NoArguments()
              : Error("Failed to save"));
}

LuxxleRewardsRemoveRecurringTipFunction::
    ~LuxxleRewardsRemoveRecurringTipFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsRemoveRecurringTipFunction::Run() {
  std::optional<luxxle_rewards::RemoveRecurringTip::Params> params =
      luxxle_rewards::RemoveRecurringTip::Params::Create(args());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (rewards_service) {
    rewards_service->RemoveRecurringTip(params->publisher_key);
  }

  return RespondNow(NoArguments());
}

LuxxleRewardsGetRecurringTipsFunction::~LuxxleRewardsGetRecurringTipsFunction() =
    default;

ExtensionFunction::ResponseAction LuxxleRewardsGetRecurringTipsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->GetRecurringTips(base::BindOnce(
      &LuxxleRewardsGetRecurringTipsFunction::OnGetRecurringTips, this));
  return RespondLater();
}

void LuxxleRewardsGetRecurringTipsFunction::OnGetRecurringTips(
    std::vector<::luxxle_rewards::mojom::PublisherInfoPtr> list) {
  base::Value::Dict result;
  base::Value::List recurring_tips;

  if (!list.empty()) {
    for (const auto& item : list) {
      base::Value::Dict tip;
      tip.Set("publisherKey", item->id);
      tip.Set("amount", item->weight);
      recurring_tips.Append(std::move(tip));
    }
  }

  result.Set("recurringTips", std::move(recurring_tips));
  Respond(WithArguments(std::move(result)));
}

LuxxleRewardsRefreshPublisherFunction::~LuxxleRewardsRefreshPublisherFunction() =
    default;

ExtensionFunction::ResponseAction LuxxleRewardsRefreshPublisherFunction::Run() {
  std::optional<luxxle_rewards::RefreshPublisher::Params> params =
      luxxle_rewards::RefreshPublisher::Params::Create(args());

  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(WithArguments(false, std::string()));
  }
  rewards_service->RefreshPublisher(
      params->publisher_key,
      base::BindOnce(&LuxxleRewardsRefreshPublisherFunction::OnRefreshPublisher,
                     this));
  return RespondLater();
}

void LuxxleRewardsRefreshPublisherFunction::OnRefreshPublisher(
    const ::luxxle_rewards::mojom::PublisherStatus status,
    const std::string& publisher_key) {
  Respond(WithArguments(static_cast<int>(status), publisher_key));
}

LuxxleRewardsGetAllNotificationsFunction::
    ~LuxxleRewardsGetAllNotificationsFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetAllNotificationsFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);

  base::Value::List list;

  if (!rewards_service) {
    return RespondNow(WithArguments(std::move(list)));
  }

  auto notifications = rewards_service->GetAllNotifications();

  for (auto const& notification : notifications) {
    base::Value::Dict item;
    item.Set("id", notification.second.id_);
    item.Set("type", notification.second.type_);
    item.Set("timestamp", static_cast<double>(notification.second.timestamp_));

    base::Value::List args;
    for (auto const& arg : notification.second.args_) {
      args.Append(arg);
    }

    item.Set("args", std::move(args));
    list.Append(std::move(item));
  }

  return RespondNow(WithArguments(std::move(list)));
}

LuxxleRewardsFetchBalanceFunction::~LuxxleRewardsFetchBalanceFunction() = default;

ExtensionFunction::ResponseAction LuxxleRewardsFetchBalanceFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->FetchBalance(
      base::BindOnce(&LuxxleRewardsFetchBalanceFunction::OnFetchBalance, this));

  return RespondLater();
}

void LuxxleRewardsFetchBalanceFunction::OnFetchBalance(
    ::luxxle_rewards::mojom::BalancePtr balance) {
  Respond(balance ? WithArguments(balance->total) : NoArguments());
}

LuxxleRewardsGetExternalWalletProvidersFunction::
    ~LuxxleRewardsGetExternalWalletProvidersFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetExternalWalletProvidersFunction::Run() {
  base::Value::List providers;

  auto* profile = Profile::FromBrowserContext(browser_context());
  if (auto* rewards_service = RewardsServiceFactory::GetForProfile(profile)) {
    for (auto& provider : rewards_service->GetExternalWalletProviders()) {
      providers.Append(provider);
    }
  }
  return RespondNow(WithArguments(std::move(providers)));
}

LuxxleRewardsGetExternalWalletFunction::
    ~LuxxleRewardsGetExternalWalletFunction() = default;

ExtensionFunction::ResponseAction LuxxleRewardsGetExternalWalletFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  RewardsService* rewards_service =
      RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not available"));
  }

  rewards_service->GetExternalWallet(base::BindOnce(
      &LuxxleRewardsGetExternalWalletFunction::OnGetExternalWallet, this));
  return RespondLater();
}

void LuxxleRewardsGetExternalWalletFunction::OnGetExternalWallet(
    ::luxxle_rewards::mojom::ExternalWalletPtr wallet) {
  if (!wallet) {
    return Respond(NoArguments());
  }

  base::Value::Dict data;
  data.Set("type", wallet->type);
  data.Set("address", wallet->address);
  data.Set("status", static_cast<int>(wallet->status));
  data.Set("userName", wallet->user_name);
  data.Set("accountUrl", wallet->account_url);
  data.Set("activityUrl", wallet->activity_url);

  Respond(WithArguments(std::move(data)));
}

LuxxleRewardsGetRewardsEnabledFunction::
    ~LuxxleRewardsGetRewardsEnabledFunction() = default;

ExtensionFunction::ResponseAction LuxxleRewardsGetRewardsEnabledFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  bool enabled =
      profile->GetPrefs()->GetBoolean(::luxxle_rewards::prefs::kEnabled);
  return RespondNow(WithArguments(enabled));
}

LuxxleRewardsGetAdsAccountStatementFunction::
    ~LuxxleRewardsGetAdsAccountStatementFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetAdsAccountStatementFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);

  if (!ads_service) {
    return RespondNow(Error("Ads service is not initialized"));
  }

  AddRef();  // Balanced in OnGetAdsAccountStatement().

  ads_service->GetStatementOfAccounts(base::BindOnce(
      &LuxxleRewardsGetAdsAccountStatementFunction::OnGetAdsAccountStatement,
      this));
  return RespondLater();
}

void LuxxleRewardsGetAdsAccountStatementFunction::OnGetAdsAccountStatement(
    luxxle_ads::mojom::StatementInfoPtr statement) {
  if (!statement) {
    Respond(WithArguments(false));
  } else {
    base::Value::Dict dict;
    dict.Set("nextPaymentDate",
             statement->next_payment_date.InSecondsFSinceUnixEpoch() * 1000);
    dict.Set("adsReceivedThisMonth", statement->ads_received_this_month);
    dict.Set("minEarningsThisMonth", statement->min_earnings_this_month);
    dict.Set("maxEarningsThisMonth", statement->max_earnings_this_month);
    dict.Set("minEarningsLastMonth", statement->min_earnings_previous_month);
    dict.Set("maxEarningsLastMonth", statement->max_earnings_previous_month);

    Respond(WithArguments(true, std::move(dict)));
  }

  Release();  // Balanced in Run()
}

LuxxleRewardsIsInitializedFunction::~LuxxleRewardsIsInitializedFunction() =
    default;

ExtensionFunction::ResponseAction LuxxleRewardsIsInitializedFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  return RespondNow(
      WithArguments(rewards_service && rewards_service->IsInitialized()));
}

LuxxleRewardsSelfCustodyInviteDismissedFunction::
    ~LuxxleRewardsSelfCustodyInviteDismissedFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsSelfCustodyInviteDismissedFunction::Run() {
  auto* prefs = Profile::FromBrowserContext(browser_context())->GetPrefs();
  return RespondNow(WithArguments(
      prefs->GetBoolean(::luxxle_rewards::prefs::kSelfCustodyInviteDismissed)));
}

LuxxleRewardsDismissSelfCustodyInviteFunction::
    ~LuxxleRewardsDismissSelfCustodyInviteFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsDismissSelfCustodyInviteFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  profile->GetPrefs()->SetBoolean(
      ::luxxle_rewards::prefs::kSelfCustodyInviteDismissed, true);
  if (auto* event_router = extensions::EventRouter::Get(profile)) {
    event_router->BroadcastEvent(std::make_unique<Event>(
        events::LUXXLE_START,
        luxxle_rewards::OnSelfCustodyInviteDismissed::kEventName,
        base::Value::List()));
  }
  return RespondNow(NoArguments());
}

LuxxleRewardsIsTermsOfServiceUpdateRequiredFunction::
    ~LuxxleRewardsIsTermsOfServiceUpdateRequiredFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsIsTermsOfServiceUpdateRequiredFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  return RespondNow(
      WithArguments(rewards_service->IsTermsOfServiceUpdateRequired()));
}

LuxxleRewardsAcceptTermsOfServiceUpdateFunction::
    ~LuxxleRewardsAcceptTermsOfServiceUpdateFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsAcceptTermsOfServiceUpdateFunction::Run() {
  auto* profile = Profile::FromBrowserContext(browser_context());
  auto* rewards_service = RewardsServiceFactory::GetForProfile(profile);
  if (!rewards_service) {
    return RespondNow(Error("Rewards service is not initialized"));
  }

  rewards_service->AcceptTermsOfServiceUpdate();
  return RespondNow(NoArguments());
}

LuxxleRewardsGetScheduledCaptchaInfoFunction::
    ~LuxxleRewardsGetScheduledCaptchaInfoFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsGetScheduledCaptchaInfoFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* luxxle_adaptive_captcha_service =
      luxxle_adaptive_captcha::LuxxleAdaptiveCaptchaServiceFactory::GetForProfile(
          profile);
  if (!luxxle_adaptive_captcha_service) {
    return RespondNow(
        Error("Adaptive captcha service called from incognito or unsupported "
              "profile"));
  }

  std::string url;
  bool max_attempts_exceeded = false;
  luxxle_adaptive_captcha_service->GetScheduledCaptchaInfo(
      &url, &max_attempts_exceeded);

  base::Value::Dict dict;
  dict.Set("url", url);
  dict.Set("maxAttemptsExceeded", max_attempts_exceeded);

  return RespondNow(WithArguments(std::move(dict)));
}

LuxxleRewardsUpdateScheduledCaptchaResultFunction::
    ~LuxxleRewardsUpdateScheduledCaptchaResultFunction() = default;

ExtensionFunction::ResponseAction
LuxxleRewardsUpdateScheduledCaptchaResultFunction::Run() {
  auto params =
      luxxle_rewards::UpdateScheduledCaptchaResult::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* luxxle_adaptive_captcha_service =
      luxxle_adaptive_captcha::LuxxleAdaptiveCaptchaServiceFactory::GetForProfile(
          profile);
  if (!luxxle_adaptive_captcha_service) {
    return RespondNow(
        Error("Adaptive captcha service called from incognito or unsupported "
              "profile"));
  }

  luxxle_adaptive_captcha_service->UpdateScheduledCaptchaResult(params->result);

  AdsService* ads_service = AdsServiceFactory::GetForProfile(profile);
  if (ads_service && params->result) {
    ads_service->NotifyDidSolveAdaptiveCaptcha();
  }

  return RespondNow(NoArguments());
}

}  // namespace extensions::api
