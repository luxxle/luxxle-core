// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/perf/brave_perf_features_processor.h"

#include "base/command_line.h"
#include "base/task/sequenced_task_runner.h"
#include "luxxle/browser/brave_browser_process.h"
// REMOVED: #include "luxxle/browser/brave_rewards/.*"
#include "luxxle/browser/perf/brave_perf_switches.h"
#include "luxxle/components/ai_chat/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/brave_ads/.*"
#include "luxxle/components/brave_news/common/pref_names.h"
// REMOVED: #include "luxxle/components/brave_rewards/.*"
#include "luxxle/components/brave_shields/content/browser/ad_block_service.h"
#include "luxxle/components/brave_shields/core/browser/ad_block_component_service_manager.h"
#include "luxxle/components/brave_shields/core/common/brave_shield_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/chrome_switches.h"
#include "components/prefs/pref_service.h"

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "luxxle/components/speedreader/speedreader_pref_names.h"
#endif

namespace {
void FakeCallback(brave_rewards::mojom::CreateRewardsWalletResult) {}

void EnableAdblockCookieList(base::WeakPtr<Profile> profile) {
  if (!profile) {
    return;
  }

  // Obtrusive cookie notices list in cosmetic filters.
  auto* component_service_manager =
      g_brave_browser_process->ad_block_service()->component_service_manager();
  if (!component_service_manager ||
      !component_service_manager->IsFilterListAvailable(
          brave_shields::kCookieListUuid)) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostDelayedTask(
        FROM_HERE, base::BindOnce(&EnableAdblockCookieList, profile),
        base::Seconds(1));
    return;
  }

  component_service_manager->EnableFilterList(brave_shields::kCookieListUuid,
                                              true);
}

}  // namespace

namespace perf {

void MaybeEnableBraveFeatureForPerfTesting(Profile* profile) {
  auto* cmd = base::CommandLine::ForCurrentProcess();
  if (!cmd->HasSwitch(switches::kEnableBraveFeaturesForPerfTesting) ||
      !cmd->HasSwitch(::switches::kUserDataDir)) {
    return;
  }

  // Notification Ads
  profile->GetPrefs()->SetBoolean(brave_ads::prefs::kOptedInToNotificationAds,
                                  true);

  // Rewards
  auto* rewards_service =
      brave_rewards::RewardsServiceFactory::GetForProfile(profile);
  rewards_service->CreateRewardsWallet("US", base::BindOnce(&FakeCallback));

  // Brave news
  profile->GetPrefs()->SetBoolean(brave_news::prefs::kNewTabPageShowToday,
                                  true);
  profile->GetPrefs()->SetBoolean(brave_news::prefs::kBraveNewsOptedIn, true);

#if BUILDFLAG(ENABLE_SPEEDREADER)
  // Speedreader
  profile->GetPrefs()->SetBoolean(speedreader::kSpeedreaderPrefEnabled, true);
#endif

  profile->GetPrefs()->SetTime(ai_chat::prefs::kLastAcceptedDisclaimer,
                               base::Time::Now());
  profile->GetPrefs()->SetBoolean(
      ai_chat::prefs::kBraveChatAutocompleteProviderEnabled, true);

  // Adblock
  EnableAdblockCookieList(profile->GetWeakPtr());
}

}  // namespace perf
