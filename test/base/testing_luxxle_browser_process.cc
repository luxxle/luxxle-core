/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/test/base/testing_luxxle_browser_process.h"

#include <utility>

#include "base/files/file_path.h"
#include "base/functional/callback_helpers.h"
#include "base/memory/scoped_refptr.h"
#include "base/task/sequenced_task_runner.h"
#include "base/task/thread_pool.h"
#include "luxxle/components/luxxle_shields/content/browser/ad_block_service.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/tor/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif
namespace tor {
class LuxxleTorClientUpdater;
}

// static
TestingLuxxleBrowserProcess* TestingLuxxleBrowserProcess::GetGlobal() {
  return static_cast<TestingLuxxleBrowserProcess*>(g_luxxle_browser_process);
}

// static
void TestingLuxxleBrowserProcess::CreateInstance() {
  DCHECK(!g_luxxle_browser_process);
  TestingLuxxleBrowserProcess* process = new TestingLuxxleBrowserProcess;
  g_luxxle_browser_process = process;
}

// static
void TestingLuxxleBrowserProcess::DeleteInstance() {
  LuxxleBrowserProcess* browser_process = g_luxxle_browser_process;
  g_luxxle_browser_process = nullptr;
  delete browser_process;
}

// static
void TestingLuxxleBrowserProcess::StartTearDown() {}

// static
void TestingLuxxleBrowserProcess::TearDownAndDeleteInstance() {
  TestingLuxxleBrowserProcess::StartTearDown();
  TestingLuxxleBrowserProcess::DeleteInstance();
}

TestingLuxxleBrowserProcess::TestingLuxxleBrowserProcess() = default;

TestingLuxxleBrowserProcess::~TestingLuxxleBrowserProcess() = default;

void TestingLuxxleBrowserProcess::StartLuxxleServices() {}

luxxle_shields::AdBlockService* TestingLuxxleBrowserProcess::ad_block_service() {
  if (!ad_block_service_) {
    scoped_refptr<base::SequencedTaskRunner> task_runner(
        base::ThreadPool::CreateSequencedTaskRunner(
            {base::MayBlock(), base::TaskPriority::USER_BLOCKING,
             base::TaskShutdownBehavior::SKIP_ON_SHUTDOWN}));
    ad_block_service_ = std::make_unique<luxxle_shields::AdBlockService>(
        /*local_state*/ nullptr, /*locale*/ "en", /*component_updater*/ nullptr,
        task_runner,
        /*subscription_download_manager_getter*/ base::DoNothing(),
        /*profile_dir*/ base::FilePath(FILE_PATH_LITERAL("")));
  }
  return ad_block_service_.get();
}

debounce::DebounceComponentInstaller*
TestingLuxxleBrowserProcess::debounce_component_installer() {
  return nullptr;
}

#if BUILDFLAG(ENABLE_REQUEST_OTR)
request_otr::RequestOTRComponentInstallerPolicy*
TestingLuxxleBrowserProcess::request_otr_component_installer() {
  return nullptr;
}
#endif

luxxle::URLSanitizerComponentInstaller*
TestingLuxxleBrowserProcess::URLSanitizerComponentInstaller() {
  return nullptr;
}

https_upgrade_exceptions::HttpsUpgradeExceptionsService*
TestingLuxxleBrowserProcess::https_upgrade_exceptions_service() {
  return nullptr;
}

localhost_permission::LocalhostPermissionComponent*
TestingLuxxleBrowserProcess::localhost_permission_component() {
  return nullptr;
}

luxxle_component_updater::LocalDataFilesService*
TestingLuxxleBrowserProcess::local_data_files_service() {
  return nullptr;
}

#if BUILDFLAG(ENABLE_TOR)
tor::LuxxleTorClientUpdater* TestingLuxxleBrowserProcess::tor_client_updater() {
  return nullptr;
}

tor::LuxxleTorPluggableTransportUpdater*
TestingLuxxleBrowserProcess::tor_pluggable_transport_updater() {
  return nullptr;
}
#endif


p3a::P3AService* TestingLuxxleBrowserProcess::p3a_service() {
  return nullptr;
}

luxxle::LuxxleReferralsService*
TestingLuxxleBrowserProcess::luxxle_referrals_service() {
  return nullptr;
}

luxxle_stats::LuxxleStatsUpdater*
TestingLuxxleBrowserProcess::luxxle_stats_updater() {
  return nullptr;
}

luxxle_ads::LuxxleStatsHelper*
TestingLuxxleBrowserProcess::ads_luxxle_stats_helper() {
  return nullptr;
}

ntp_background_images::NTPBackgroundImagesService*
TestingLuxxleBrowserProcess::ntp_background_images_service() {
  return nullptr;
}

#if BUILDFLAG(ENABLE_SPEEDREADER)
speedreader::SpeedreaderRewriterService*
TestingLuxxleBrowserProcess::speedreader_rewriter_service() {
  return nullptr;
}
#endif

luxxle_ads::ResourceComponent* TestingLuxxleBrowserProcess::resource_component() {
  return nullptr;
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
luxxle_vpn::LuxxleVPNConnectionManager*
TestingLuxxleBrowserProcess::luxxle_vpn_connection_manager() {
  return luxxle_vpn_connection_manager_.get();
}
void TestingLuxxleBrowserProcess::SetLuxxleVPNConnectionManagerForTesting(
    std::unique_ptr<luxxle_vpn::LuxxleVPNConnectionManager> manager) {
  luxxle_vpn_connection_manager_ = std::move(manager);
}
#endif

misc_metrics::ProcessMiscMetrics*
TestingLuxxleBrowserProcess::process_misc_metrics() {
  return nullptr;
}

void TestingLuxxleBrowserProcess::SetAdBlockService(
    std::unique_ptr<luxxle_shields::AdBlockService> service) {
  ad_block_service_ = std::move(service);
}
///////////////////////////////////////////////////////////////////////////////

TestingLuxxleBrowserProcessInitializer::TestingLuxxleBrowserProcessInitializer() {
  TestingLuxxleBrowserProcess::CreateInstance();
}

TestingLuxxleBrowserProcessInitializer::
    ~TestingLuxxleBrowserProcessInitializer() {
  TestingLuxxleBrowserProcess::DeleteInstance();
}
