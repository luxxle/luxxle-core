// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/perf/luxxle_perf_features_processor.h"

#include "base/command_line.h"
#include "base/task/sequenced_task_runner.h"
#include "luxxle/browser/luxxle_browser_process.h"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/browser/perf/luxxle_perf_switches.h"
#include "luxxle/components/ai_chat/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_ads/.*"
#include "luxxle/components/luxxle_news/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/luxxle_shields/content/browser/ad_block_service.h"
#include "luxxle/components/luxxle_shields/core/browser/ad_block_component_service_manager.h"
#include "luxxle/components/luxxle_shields/core/common/luxxle_shield_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/chrome_switches.h"
#include "components/prefs/pref_service.h"

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "luxxle/components/speedreader/speedreader_pref_names.h"
#endif

namespace {
void FakeCallback(luxxle_rewards::mojom::CreateRewardsWalletResult) {}

void EnableAdblockCookieList(base::WeakPtr<Profile> profile) {
  if (!profile) {
    return;
  }

  // Obtrusive cookie notices list in cosmetic filters.
  auto* component_service_manager =
      g_luxxle_browser_process->ad_block_service()->component_service_manager();
  if (!component_service_manager ||
      !component_service_manager->IsFilterListAvailable(
          luxxle_shields::kCookieListUuid)) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
        FROM_HERE, base::BindOnce(&EnableAdblockCookieList, profile),
        base::Seconds(1));
    return;
  }

  component_service_manager->EnableFilterList(luxxle_shields::kCookieListUuid,
                                              true);
}

}  // namespace

namespace perf {

void MaybeEnableLuxxleFeatureForPerfTesting(Profile* profile) {
  auto* cmd = base::CommandLine::ForCurrentProcess();
  if (!cmd->HasSwitch(switches::kEnableLuxxleFeaturesForPerfTesting) ||
      !cmd->HasSwitch(::switches::kUserDataDir)) {
    return;
  }

  // Notification Ads
  profile->GetPrefs()->SetBoolean(luxxle_ads::prefs::kOptedInToNotificationAds,
                                  true);

  // Rewards
  auto* rewards_service =
      luxxle_rewards::RewardsServiceFactory::GetForProfile(profile);
  rewards_service->CreateRewardsWallet("US", base::BindOnce(&FakeCallback));

  // Luxxle news
  profile->GetPrefs()->SetBoolean(luxxle_news::prefs::kNewTabPageShowToday,
                                  true);
  profile->GetPrefs()->SetBoolean(luxxle_news::prefs::kLuxxleNewsOptedIn, true);

#if BUILDFLAG(ENABLE_SPEEDREADER)
  // Speedreader
  profile->GetPrefs()->SetBoolean(speedreader::kSpeedreaderPrefEnabled, true);
#endif

  profile->GetPrefs()->SetTime(ai_chat::prefs::kLastAcceptedDisclaimer,
                               base::Time::Now());
  profile->GetPrefs()->SetBoolean(
      ai_chat::prefs::kLuxxleChatAutocompleteProviderEnabled, true);

  // Adblock
  EnableAdblockCookieList(profile->GetWeakPtr());
}

}  // namespace perf
