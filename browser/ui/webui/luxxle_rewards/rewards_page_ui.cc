/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_rewards/rewards_page_ui.h"

#include <utility>

#include "luxxle/browser/luxxle_adaptive_captcha/luxxle_adaptive_captcha_service_factory.h"
// REMOVED: #include "luxxle/browser/luxxle_ads/.*"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/browser/ui/webui/luxxle_rewards/rewards_page_data_source.h"
#include "luxxle/browser/ui/webui/luxxle_rewards/rewards_page_handler.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"

#if BUILDFLAG(IS_ANDROID)
#include "luxxle/browser/ui/android/luxxle_rewards/rewards_page_helper.h"
#endif

using luxxle_adaptive_captcha::LuxxleAdaptiveCaptchaServiceFactory;

namespace luxxle_rewards {

namespace {

#if BUILDFLAG(IS_ANDROID)

class AndroidBubbleDelegate : public RewardsPageHandler::BubbleDelegate {
 public:
  AndroidBubbleDelegate() = default;
  ~AndroidBubbleDelegate() override = default;

  void ShowUI() override {}

  void OpenTab(const std::string& url) override {
    luxxle_rewards::OpenURLForRewardsPage(url);
  }

  std::string GetPublisherIdForActiveTab() override { return ""; }
};

std::unique_ptr<RewardsPageHandler::BubbleDelegate> MakeBubbleDelegate() {
  return std::make_unique<AndroidBubbleDelegate>();
}

#else

std::unique_ptr<RewardsPageHandler::BubbleDelegate> MakeBubbleDelegate() {
  return nullptr;
}

#endif  // BUILDFLAG(IS_ANDROID)

}  // namespace

RewardsPageUI::RewardsPageUI(content::WebUI* web_ui, const std::string& host)
    : WebUIController(web_ui) {
  CreateAndAddRewardsPageDataSource(*web_ui, host);
}

RewardsPageUI::~RewardsPageUI() = default;

void RewardsPageUI::BindInterface(
    mojo::PendingReceiver<mojom::RewardsPageHandler> receiver) {
  auto* profile = Profile::FromWebUI(web_ui());
  CHECK(profile);

  handler_ = std::make_unique<RewardsPageHandler>(
      std::move(receiver), MakeBubbleDelegate(),
      RewardsServiceFactory::GetForProfile(profile),
      luxxle_ads::AdsServiceFactory::GetForProfile(profile),
      LuxxleAdaptiveCaptchaServiceFactory::GetForProfile(profile),
      profile->GetPrefs());
}

WEB_UI_CONTROLLER_TYPE_IMPL(RewardsPageUI)

}  // namespace luxxle_rewards
