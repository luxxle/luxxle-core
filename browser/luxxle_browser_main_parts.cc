/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/luxxle_browser_main_parts.h"

#include <memory>
#include <utility>

#include "base/command_line.h"
#include "base/path_service.h"
// Removed Luxxle-specific includes - using Chromium defaults
// #include "luxxle/browser/browsing_data/luxxle_clear_browsing_data.h"
// #include "luxxle/components/luxxle_component_updater/browser/luxxle_on_demand_updater.h"
// // REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// // REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// #include "luxxle/components/luxxle_sync/features.h"
// #include "luxxle/components/constants/luxxle_constants.h"
// #include "luxxle/components/constants/pref_names.h"
// #include "luxxle/components/ipfs/buildflags/buildflags.h"
// #include "luxxle/components/speedreader/common/buildflags/buildflags.h"
// #include "luxxle/components/tor/buildflags/buildflags.h"
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

// Removed Luxxle-specific conditional includes - using Chromium defaults
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
  // Removed Luxxle component updater - using Chromium defaults
  // luxxle_component_updater::LuxxleOnDemandUpdater::GetInstance()
  //     ->RegisterOnDemandUpdater(
  //         &g_browser_process->component_updater()->GetOnDemandUpdater());

  return ChromeBrowserMainParts_ChromiumImpl::PreMainMessageLoopRun();
}

void ChromeBrowserMainParts::PreBrowserStart() {
  // Removed Luxxle speedreader - using Chromium defaults
  // #if BUILDFLAG(ENABLE_SPEEDREADER)
  //   DCHECK(sessions::ContentSerializedNavigationDriver::GetInstance());
  //   speedreader::SpeedreaderExtendedInfoHandler::Register();
  // #endif

  ChromeBrowserMainParts_ChromiumImpl::PreBrowserStart();
}

void ChromeBrowserMainParts::PostBrowserStart() {
  ChromeBrowserMainParts_ChromiumImpl::PostBrowserStart();

  // Removed Luxxle-specific startup code - using Chromium defaults
  // - Tor profile cleanup
  // - Luxxle infobars (P3A, sync, etc.)
  // - IPFS component cleanup
}

void ChromeBrowserMainParts::PreShutdown() {
  // Removed Luxxle clear browsing data - using Chromium defaults
  // content::LuxxleClearBrowsingData::ClearOnExit();
  ChromeBrowserMainParts_ChromiumImpl::PreShutdown();
}

void ChromeBrowserMainParts::PreProfileInit() {
  ChromeBrowserMainParts_ChromiumImpl::PreProfileInit();
  // Removed Luxxle sync feature check - using Chromium defaults
  // #if !BUILDFLAG(IS_ANDROID)
  //   auto* command_line = base::CommandLine::ForCurrentProcess();
  //   if (!base::FeatureList::IsEnabled(luxxle_sync::features::kLuxxleSync)) {
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

  // Removed Luxxle background video playback - using Chromium defaults
  // #if BUILDFLAG(IS_ANDROID)
  //   if (base::FeatureList::IsEnabled(
  //           preferences::features::kLuxxleBackgroundVideoPlayback) &&
  //       profile->GetPrefs()->GetBoolean(kBackgroundVideoPlaybackEnabled)) {
  //     auto* command_line = base::CommandLine::ForCurrentProcess();
  //     command_line->AppendSwitch(switches::kDisableBackgroundMediaSuspend);
  //   }
  // #endif
}
