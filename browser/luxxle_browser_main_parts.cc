/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/luxxle_browser_main_parts.h"

#include <memory>
#include <utility>

#include "base/command_line.h"
#include "base/path_service.h"
// Removed Brave-specific includes - using Chromium defaults
// #include "brave/browser/browsing_data/brave_clear_browsing_data.h"
// #include "brave/components/brave_component_updater/browser/brave_on_demand_updater.h"
// #include "brave/components/brave_rewards/core/rewards_flags.h"
// #include "brave/components/brave_rewards/core/rewards_util.h"
// #include "brave/components/brave_sync/features.h"
// #include "brave/components/constants/brave_constants.h"
// #include "brave/components/constants/pref_names.h"
// #include "brave/components/ipfs/buildflags/buildflags.h"
// #include "brave/components/speedreader/common/buildflags/buildflags.h"
// #include "brave/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/chrome_features.h"
#include "chrome/common/chrome_paths.h"
#include "components/component_updater/component_updater_service.h"
#include "components/prefs/pref_service.h"
#include "components/sync/base/command_line_switches.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_user_settings.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"
#include "extensions/buildflags/buildflags.h"
#include "media/base/media_switches.h"

// Removed Brave-specific conditional includes - using Chromium defaults
#if !BUILDFLAG(IS_ANDROID)
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "components/infobars/content/content_infobar_manager.h"
#endif

ChromeBrowserMainParts::ChromeBrowserMainParts(bool is_integration_test,
                                               StartupData* startup_data)
    : ChromeBrowserMainParts_ChromiumImpl(is_integration_test, startup_data) {}

ChromeBrowserMainParts::~ChromeBrowserMainParts() = default;

int ChromeBrowserMainParts::PreMainMessageLoopRun() {
  // Removed Brave component updater - using Chromium defaults
  // brave_component_updater::BraveOnDemandUpdater::GetInstance()
  //     ->RegisterOnDemandUpdater(
  //         &g_browser_process->component_updater()->GetOnDemandUpdater());

  return ChromeBrowserMainParts_ChromiumImpl::PreMainMessageLoopRun();
}

void ChromeBrowserMainParts::PreBrowserStart() {
  // Removed Brave speedreader - using Chromium defaults
  // #if BUILDFLAG(ENABLE_SPEEDREADER)
  //   DCHECK(sessions::ContentSerializedNavigationDriver::GetInstance());
  //   speedreader::SpeedreaderExtendedInfoHandler::Register();
  // #endif

  ChromeBrowserMainParts_ChromiumImpl::PreBrowserStart();
}

void ChromeBrowserMainParts::PostBrowserStart() {
  ChromeBrowserMainParts_ChromiumImpl::PostBrowserStart();

  // Removed Brave-specific startup code - using Chromium defaults
  // - Tor profile cleanup
  // - Brave infobars (P3A, sync, etc.)
  // - IPFS component cleanup
}

void ChromeBrowserMainParts::PreShutdown() {
  // Removed Brave clear browsing data - using Chromium defaults
  // content::BraveClearBrowsingData::ClearOnExit();
  ChromeBrowserMainParts_ChromiumImpl::PreShutdown();
}

void ChromeBrowserMainParts::PreProfileInit() {
  ChromeBrowserMainParts_ChromiumImpl::PreProfileInit();
  // Removed Brave sync feature check - using Chromium defaults
  // #if !BUILDFLAG(IS_ANDROID)
  //   auto* command_line = base::CommandLine::ForCurrentProcess();
  //   if (!base::FeatureList::IsEnabled(brave_sync::features::kBraveSync)) {
  //     if (!command_line->HasSwitch(syncer::kDisableSync))
  //       command_line->AppendSwitch(syncer::kDisableSync);
  //   } else {
  //     command_line->RemoveSwitch(syncer::kDisableSync);
  //   }
  // #endif
}

void ChromeBrowserMainParts::PostProfileInit(Profile* profile,
                                             bool is_initial_profile) {
  ChromeBrowserMainParts_ChromiumImpl::PostProfileInit(profile,
                                                       is_initial_profile);

  // Removed Brave background video playback - using Chromium defaults
  // #if BUILDFLAG(IS_ANDROID)
  //   if (base::FeatureList::IsEnabled(
  //           preferences::features::kBraveBackgroundVideoPlayback) &&
  //       profile->GetPrefs()->GetBoolean(kBackgroundVideoPlaybackEnabled)) {
  //     auto* command_line = base::CommandLine::ForCurrentProcess();
  //     command_line->AppendSwitch(switches::kDisableBackgroundMediaSuspend);
  //   }
  // #endif
}
