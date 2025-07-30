/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_EXTENSIONS_API_LUXXLE_REWARDS_API_H_
#define LUXXLE_BROWSER_EXTENSIONS_API_LUXXLE_REWARDS_API_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

// REMOVED: #include "luxxle/components/luxxle_ads/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "extensions/browser/extension_function.h"

namespace extensions {
namespace api {

class LuxxleRewardsIsSupportedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.isSupported", UNKNOWN)

 protected:
  ~LuxxleRewardsIsSupportedFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsRecordNTPPanelTriggerFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.recordNTPPanelTrigger", UNKNOWN)

 protected:
  ~LuxxleRewardsRecordNTPPanelTriggerFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsOpenRewardsPanelFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.openRewardsPanel", UNKNOWN)

 protected:
  ~LuxxleRewardsOpenRewardsPanelFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsGetPublisherInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getPublisherInfo", UNKNOWN)

 protected:
  ~LuxxleRewardsGetPublisherInfoFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherInfo(const luxxle_rewards::mojom::Result result,
                          luxxle_rewards::mojom::PublisherInfoPtr info);
};

class LuxxleRewardsGetPublisherInfoForTabFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getPublisherInfoForTab", UNKNOWN)

 protected:
  ~LuxxleRewardsGetPublisherInfoForTabFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetPublisherPanelInfo(luxxle_rewards::mojom::Result result,
                               luxxle_rewards::mojom::PublisherInfoPtr info);
};

class LuxxleRewardsGetRewardsParametersFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getRewardsParameters", UNKNOWN)

 protected:
  ~LuxxleRewardsGetRewardsParametersFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetRewardsParameters(
      luxxle_rewards::mojom::RewardsParametersPtr parameters);
};

class LuxxleRewardsCreateRewardsWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.createRewardsWallet", UNKNOWN)

 protected:
  ~LuxxleRewardsCreateRewardsWalletFunction() override;

  ResponseAction Run() override;

 private:
  void CreateRewardsWalletCallback(
      luxxle_rewards::mojom::CreateRewardsWalletResult result);
};

class LuxxleRewardsGetAvailableCountriesFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getAvailableCountries", UNKNOWN)

 protected:
  ~LuxxleRewardsGetAvailableCountriesFunction() override;

 private:
  void GetAvailableCountriesCallback(std::vector<std::string> countries);

  ResponseAction Run() override;
};

class LuxxleRewardsGetDefaultCountryFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getDefaultCountry", UNKNOWN)

 protected:
  ~LuxxleRewardsGetDefaultCountryFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsGetDeclaredCountryFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getDeclaredCountry", UNKNOWN)
 protected:
  ~LuxxleRewardsGetDeclaredCountryFunction() override;
  ResponseAction Run() override;
};

class LuxxleRewardsGetUserTypeFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getUserType", UNKNOWN)

 protected:
  ~LuxxleRewardsGetUserTypeFunction() override;
  ResponseAction Run() override;

 private:
  void Callback(luxxle_rewards::mojom::UserType user_type);
};

class LuxxleRewardsGetPublishersVisitedCountFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getPublishersVisitedCount", UNKNOWN)

 protected:
  ~LuxxleRewardsGetPublishersVisitedCountFunction() override;

  ResponseAction Run() override;

 private:
  void Callback(int count);
};

class LuxxleRewardsGetBalanceReportFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getBalanceReport", UNKNOWN)

 protected:
  ~LuxxleRewardsGetBalanceReportFunction() override;

  ResponseAction Run() override;

 private:
  void OnBalanceReport(const luxxle_rewards::mojom::Result result,
                       luxxle_rewards::mojom::BalanceReportInfoPtr report);
};

class LuxxleRewardsSaveRecurringTipFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.saveRecurringTip", UNKNOWN)

 protected:
  ~LuxxleRewardsSaveRecurringTipFunction() override;

  ResponseAction Run() override;

 private:
  void OnSaveRecurringTip(luxxle_rewards::mojom::Result result);
};

class LuxxleRewardsRemoveRecurringTipFunction :
  public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.removeRecurringTip", UNKNOWN)

 protected:
  ~LuxxleRewardsRemoveRecurringTipFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsGetRecurringTipsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getRecurringTips", UNKNOWN)

 protected:
  ~LuxxleRewardsGetRecurringTipsFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetRecurringTips(
      std::vector<luxxle_rewards::mojom::PublisherInfoPtr> list);
};

class LuxxleRewardsRefreshPublisherFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.refreshPublisher", UNKNOWN)

 protected:
  ~LuxxleRewardsRefreshPublisherFunction() override;

  ResponseAction Run() override;

 private:
  void OnRefreshPublisher(const luxxle_rewards::mojom::PublisherStatus status,
                          const std::string& publisher_key);
};

class LuxxleRewardsGetAllNotificationsFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getAllNotifications", UNKNOWN)

 protected:
  ~LuxxleRewardsGetAllNotificationsFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsFetchBalanceFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.fetchBalance", UNKNOWN)

 protected:
  ~LuxxleRewardsFetchBalanceFunction() override;

  ResponseAction Run() override;

 private:
  void OnFetchBalance(luxxle_rewards::mojom::BalancePtr balance);
};

class LuxxleRewardsGetExternalWalletProvidersFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getExternalWalletProviders", UNKNOWN)

 protected:
  ~LuxxleRewardsGetExternalWalletProvidersFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsGetExternalWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getExternalWallet", UNKNOWN)

 protected:
  ~LuxxleRewardsGetExternalWalletFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetExternalWallet(luxxle_rewards::mojom::ExternalWalletPtr wallet);
};

class LuxxleRewardsGetRewardsEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getRewardsEnabled", UNKNOWN)

 protected:
  ~LuxxleRewardsGetRewardsEnabledFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsGetAdsAccountStatementFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getAdsAccountStatement", UNKNOWN)

 protected:
  ~LuxxleRewardsGetAdsAccountStatementFunction() override;

  ResponseAction Run() override;

 private:
  void OnGetAdsAccountStatement(luxxle_ads::mojom::StatementInfoPtr statement);
};

class LuxxleRewardsIsInitializedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.isInitialized", UNKNOWN)

 protected:
  ~LuxxleRewardsIsInitializedFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsSelfCustodyInviteDismissedFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.selfCustodyInviteDismissed", UNKNOWN)

 protected:
  ~LuxxleRewardsSelfCustodyInviteDismissedFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsDismissSelfCustodyInviteFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.dismissSelfCustodyInvite", UNKNOWN)

 protected:
  ~LuxxleRewardsDismissSelfCustodyInviteFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsIsTermsOfServiceUpdateRequiredFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.isTermsOfServiceUpdateRequired",
                             UNKNOWN)

 protected:
  ~LuxxleRewardsIsTermsOfServiceUpdateRequiredFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsAcceptTermsOfServiceUpdateFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.acceptTermsOfServiceUpdate", UNKNOWN)

 protected:
  ~LuxxleRewardsAcceptTermsOfServiceUpdateFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsGetScheduledCaptchaInfoFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.getScheduledCaptchaInfo", UNKNOWN)

 protected:
  ~LuxxleRewardsGetScheduledCaptchaInfoFunction() override;

  ResponseAction Run() override;
};

class LuxxleRewardsUpdateScheduledCaptchaResultFunction
    : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("luxxleRewards.updateScheduledCaptchaResult",
                             UNKNOWN)

 protected:
  ~LuxxleRewardsUpdateScheduledCaptchaResultFunction() override;

  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // LUXXLE_BROWSER_EXTENSIONS_API_LUXXLE_REWARDS_API_H_
