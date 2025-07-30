/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

// An implementation of LuxxleBrowserProcess for unit tests that fails for most
// services. By preventing creation of services, we reduce dependencies and
// keep the profile clean. Clients of this class must handle the NULL return
// value, however.

#ifndef LUXXLE_TEST_BASE_TESTING_LUXXLE_BROWSER_PROCESS_H_
#define LUXXLE_TEST_BASE_TESTING_LUXXLE_BROWSER_PROCESS_H_

#include <stdint.h>

#include <memory>
#include <string>

#include "luxxle/browser/luxxle_browser_process.h"

namespace luxxle_shields {
class AdBlockService;
}

namespace luxxle_vpn {
class LuxxleVPNConnectionManager;
}

class TestingLuxxleBrowserProcess : public LuxxleBrowserProcess {
 public:
  // Initializes |g_luxxle_browser_process| with a new
  // TestingLuxxleBrowserProcess.
  static void CreateInstance();

  // Cleanly destroys |g_luxxle_browser_process|.
  static void DeleteInstance();

  // Convenience method to get g_luxxle_browser_process as a
  // TestingLuxxleBrowserProcess*.
  static TestingLuxxleBrowserProcess* GetGlobal();

  // Convenience method to both teardown and destroy the TestingBrowserProcess
  // instance
  static void TearDownAndDeleteInstance();

  TestingLuxxleBrowserProcess(const TestingLuxxleBrowserProcess&) = delete;
  TestingLuxxleBrowserProcess& operator=(const TestingLuxxleBrowserProcess&) =
      delete;

  // LuxxleBrowserProcess overrides:
  void StartLuxxleServices() override;
  luxxle_shields::AdBlockService* ad_block_service() override;
  debounce::DebounceComponentInstaller* debounce_component_installer() override;
#if BUILDFLAG(ENABLE_REQUEST_OTR)
  request_otr::RequestOTRComponentInstallerPolicy*
  request_otr_component_installer() override;
#endif
  luxxle::URLSanitizerComponentInstaller* URLSanitizerComponentInstaller()
      override;
  https_upgrade_exceptions::HttpsUpgradeExceptionsService*
  https_upgrade_exceptions_service() override;
  localhost_permission::LocalhostPermissionComponent*
  localhost_permission_component() override;
  luxxle_component_updater::LocalDataFilesService* local_data_files_service()
      override;
#if BUILDFLAG(ENABLE_TOR)
  tor::LuxxleTorClientUpdater* tor_client_updater() override;
  tor::LuxxleTorPluggableTransportUpdater* tor_pluggable_transport_updater()
      override;
#endif
  p3a::P3AService* p3a_service() override;
  luxxle::LuxxleReferralsService* luxxle_referrals_service() override;
  luxxle_stats::LuxxleStatsUpdater* luxxle_stats_updater() override;
  luxxle_ads::LuxxleStatsHelper* ads_luxxle_stats_helper() override;
  ntp_background_images::NTPBackgroundImagesService*
  ntp_background_images_service() override;
#if BUILDFLAG(ENABLE_SPEEDREADER)
  speedreader::SpeedreaderRewriterService* speedreader_rewriter_service()
      override;
#endif
  luxxle_ads::ResourceComponent* resource_component() override;
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  luxxle_vpn::LuxxleVPNConnectionManager* luxxle_vpn_connection_manager() override;
  void SetLuxxleVPNConnectionManagerForTesting(
      std::unique_ptr<luxxle_vpn::LuxxleVPNConnectionManager> manager);
#endif
  misc_metrics::ProcessMiscMetrics* process_misc_metrics() override;

  // Populate the mock process with services. Consumer is responsible for
  // cleaning these up after completion of a test.
  void SetAdBlockService(std::unique_ptr<luxxle_shields::AdBlockService>);

 private:
  // Perform necessary cleanup prior to destruction of |g_browser_process|
  static void StartTearDown();

  // See CreateInstance() and DestroyInstance() above.
  TestingLuxxleBrowserProcess();
  ~TestingLuxxleBrowserProcess() override;

  std::unique_ptr<luxxle_shields::AdBlockService> ad_block_service_;

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  std::unique_ptr<luxxle_vpn::LuxxleVPNConnectionManager>
      luxxle_vpn_connection_manager_;
#endif
};

class TestingLuxxleBrowserProcessInitializer {
 public:
  TestingLuxxleBrowserProcessInitializer();
  TestingLuxxleBrowserProcessInitializer(
      const TestingLuxxleBrowserProcessInitializer&) = delete;
  TestingLuxxleBrowserProcessInitializer& operator=(
      const TestingLuxxleBrowserProcessInitializer&) = delete;
  ~TestingLuxxleBrowserProcessInitializer();
};

#endif  // LUXXLE_TEST_BASE_TESTING_LUXXLE_BROWSER_PROCESS_H_
