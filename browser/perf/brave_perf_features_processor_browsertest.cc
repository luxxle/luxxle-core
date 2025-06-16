/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/scoped_observation.h"
// REMOVED: #include "luxxle/browser/brave_rewards/.*"
#include "luxxle/browser/perf/brave_perf_switches.h"
#include "luxxle/components/ai_chat/core/browser/utils.h"
// REMOVED: #include "luxxle/components/brave_ads/.*"
#include "luxxle/components/brave_news/common/pref_names.h"
// REMOVED: #include "luxxle/components/brave_rewards/.*"
// REMOVED: #include "luxxle/components/brave_rewards/.*"
#include "luxxle/components/speedreader/common/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "luxxle/browser/speedreader/speedreader_service_factory.h"
#include "luxxle/components/speedreader/speedreader_service.h"
#endif

namespace {
class TestRewardsServiceObserver
    : public brave_rewards::RewardsServiceObserver {
 public:
  void WaitForServiceInitialized(brave_rewards::RewardsService* service) {
    if (service->IsInitialized()) {
      return;
    }

    observer_.Observe(service);
    run_loop_.Run();
  }

 private:
  void OnRewardsInitialized(brave_rewards::RewardsService*) override {
    run_loop_.Quit();
  }

  base::RunLoop run_loop_;
  base::ScopedObservation<brave_rewards::RewardsService,
                          brave_rewards::RewardsServiceObserver>
      observer_{this};
};
}  // namespace

class BraveSpeedFeatureProcessorBrowserTest : public InProcessBrowserTest {
 protected:
  void SetUpCommandLine(base::CommandLine* command_line) override {
    InProcessBrowserTest::SetUpCommandLine(command_line);
    command_line->AppendSwitch(
        perf::switches::kEnableBraveFeaturesForPerfTesting);
  }

#if BUILDFLAG(ENABLE_SPEEDREADER)
  bool SpeedreaderIsEnabled() {
    auto* speedreader_service =
        speedreader::SpeedreaderServiceFactory::GetForBrowserContext(
            browser()->profile());
    return speedreader_service->IsEnabledForAllSites();
  }
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)

  bool BraveNewsAreEnabled() {
    return brave_news::IsEnabled(browser()->profile()->GetPrefs());
  }

  bool HasOptedInToNotificationAds() {
    return browser()->profile()->GetPrefs()->GetBoolean(
        brave_ads::prefs::kOptedInToNotificationAds);
  }

  void WaitForRewardsServiceInitialized() {
    auto* rewards_service = brave_rewards::RewardsServiceFactory::GetForProfile(
        browser()->profile());
    TestRewardsServiceObserver observer;
    observer.WaitForServiceInitialized(rewards_service);
  }
};

IN_PROC_BROWSER_TEST_F(BraveSpeedFeatureProcessorBrowserTest, PRE_Default) {
  WaitForRewardsServiceInitialized();
}

IN_PROC_BROWSER_TEST_F(BraveSpeedFeatureProcessorBrowserTest, Default) {
#if BUILDFLAG(ENABLE_SPEEDREADER)
  EXPECT_TRUE(SpeedreaderIsEnabled());
#endif
  EXPECT_TRUE(HasOptedInToNotificationAds());
  EXPECT_TRUE(BraveNewsAreEnabled());
  WaitForRewardsServiceInitialized();

  auto* prefs = browser()->profile()->GetPrefs();
  EXPECT_TRUE(ai_chat::IsAIChatEnabled(prefs));
  EXPECT_TRUE(ai_chat::HasUserOptedIn(prefs));
}
