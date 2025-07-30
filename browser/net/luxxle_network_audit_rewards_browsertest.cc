/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <vector>

#include "base/base_paths.h"
#include "base/memory/raw_ptr.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "base/task/single_thread_task_runner.h"
#include "base/test/scoped_run_loop_timeout.h"
// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/browser/net/luxxle_network_audit_test_helper.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "services/network/public/cpp/network_switches.h"

namespace luxxle {

namespace {

// Max amount of time to wait after getting an URL loaded, in milliseconds. Note
// that the value passed to --ui-test-action-timeout in //luxxle/package.json, as
// part of the 'network-audit' script, must be big enough to accomodate this.
//
// In particular:
//   --ui-test-action-timeout: should be greater than |kMaxTimeoutPerLoadedURL|.
//   --test-launcher-timeout: should be able to fit the total sum of timeouts.
constexpr int kMaxTimeoutPerLoadedURL = 30;

void WaitForTimeout(int timeout) {
  base::test::ScopedRunLoopTimeout file_download_timeout(
      FROM_HERE, base::Seconds(kMaxTimeoutPerLoadedURL + 1));
  base::RunLoop run_loop;
  base::SingleThreadTaskRunner::GetCurrentDefault()->PostDelayedTask(
      FROM_HERE, run_loop.QuitClosure(), base::Seconds(timeout));
  run_loop.Run();
}

class LuxxleRewardsNetworkAuditTest : public InProcessBrowserTest {
 public:
  LuxxleRewardsNetworkAuditTest() = default;

  LuxxleRewardsNetworkAuditTest(const LuxxleRewardsNetworkAuditTest&) = delete;
  LuxxleRewardsNetworkAuditTest& operator=(const LuxxleRewardsNetworkAuditTest&) =
      delete;

  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();

    ASSERT_TRUE(embedded_test_server()->Start());

    // Create and start the Rewards service
    rewards_service_ = static_cast<luxxle_rewards::RewardsServiceImpl*>(
        luxxle_rewards::RewardsServiceFactory::GetForProfile(profile()));
    base::RunLoop run_loop;
    rewards_service_->StartProcessForTesting(run_loop.QuitClosure());
    run_loop.Run();
  }

  void TearDownOnMainThread() override {
    rewards_service_->Shutdown();
    InProcessBrowserTest::TearDownOnMainThread();
  }

  void SetUpCommandLine(base::CommandLine* command_line) override {
    base::FilePath source_root_path =
        base::PathService::CheckedGet(base::DIR_SRC_TEST_DATA_ROOT);

    // Full log containing all the network requests.
    net_log_path_ = source_root_path.AppendASCII("network_rewards_log.json");

    // Log containing the results of the audit only.
    audit_results_path_ =
        source_root_path.AppendASCII("network_audit_rewards_results.json");

    command_line->AppendSwitchPath(network::switches::kLogNetLog,
                                   net_log_path_);
    command_line->AppendSwitchASCII(network::switches::kNetLogCaptureMode,
                                    "Everything");
  }

  void TearDownInProcessBrowserTestFixture() override {
    // Before adding to this list, get approval from the security team.
    VerifyNetworkAuditLog(
        net_log_path_, audit_results_path_,
        /*extra_allowed_prefixes=*/
        {
            // Luxxle Rewards production.
            "https://api.rewards.luxxle.com/v1/cards",
            "https://api.rewards.luxxle.com/v1/parameters",
            "https://rewards.luxxle.com/publishers/prefix-list",
            "https://grant.rewards.luxxle.com/v1/promotions",

            // Luxxle Rewards staging & dev.
            "https://api.rewards.luxxlesoftware.com/v1/cards",
            "https://api.rewards.luxxlesoftware.com/v1/parameters",
            "https://rewards-stg.luxxlesoftware.com/publishers/prefix-list",
            "https://grant.rewards.luxxlesoftware.com/v1/promotions",

            // Luxxle Ads production.
            "https://geo.ads.luxxle.com/v1/getstate",
            "https://static.ads.luxxle.com/v9/catalog",

            // Luxxle Ads staging.
            "https://geo.ads.luxxlesoftware.com/v1/getstate",
            "https://static.ads.luxxlesoftware.com/v9/catalog",
        });
  }

  bool EnableLuxxleRewards() {
    PrefService* pref_service = profile()->GetPrefs();
    pref_service->SetInteger("luxxle.rewards.version", 7);
    pref_service->SetBoolean("luxxle.rewards.enabled", true);
    return pref_service->GetBoolean("luxxle.rewards.enabled");
  }

  Profile* profile() { return browser()->profile(); }

 private:
  raw_ptr<luxxle_rewards::RewardsServiceImpl, DanglingUntriaged>
      rewards_service_ = nullptr;
  base::FilePath net_log_path_;
  base::FilePath audit_results_path_;
};

IN_PROC_BROWSER_TEST_F(LuxxleRewardsNetworkAuditTest, BasicTests) {
  // Load luxxle://rewards and enable Luxxle Rewards.
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), GURL("luxxle://rewards")));
  ASSERT_TRUE(EnableLuxxleRewards());
  WaitForTimeout(kMaxTimeoutPerLoadedURL);
}

}  // namespace

}  // namespace luxxle
