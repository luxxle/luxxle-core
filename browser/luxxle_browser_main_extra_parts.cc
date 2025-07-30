/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/luxxle_browser_main_extra_parts.h"

#include "base/metrics/histogram_macros.h"
#include "components/metrics/metrics_pref_names.h"
#include "components/prefs/pref_service.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"

#if !BUILDFLAG(IS_ANDROID)
#include "chrome/browser/first_run/first_run.h"
#endif  // !BUILDFLAG(IS_ANDROID)

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "extensions/common/extension.h"
#endif  // BUILDFLAG(ENABLE_EXTENSIONS)

namespace {

// Records default values for some histograms because we want these stats to be
// uploaded anyways. Corresponding components will write new values according
// to their usage scenarios.
//
// For profile specific values, see browser/profiles/profile_util.cc
void RecordInitialP3AValues() {
#if !BUILDFLAG(IS_ANDROID)
  // Removed Luxxle importer P3A - using Chromium defaults
  // if (first_run::IsChromeFirstRun()) {
  //   RecordImporterP3A(importer::ImporterType::TYPE_UNKNOWN);
  // }

  // Removed Luxxle new tab message handler - using Chromium defaults
  // LuxxleNewTabMessageHandler::RecordInitialP3AValues(
  //     g_browser_process->local_state());
#endif  // !BUILDFLAG(IS_ANDROID)

  // Removed Luxxle shields P3A - using Chromium defaults
  // luxxle_shields::MaybeRecordShieldsUsageP3A(luxxle_shields::kNeverClicked,
  //                                           g_browser_process->local_state());

  // Record crash reporting status stats.
  const bool crash_reports_enabled = g_browser_process->local_state()->
      GetBoolean(metrics::prefs::kMetricsReportingEnabled);
  UMA_HISTOGRAM_BOOLEAN("Luxxle.Core.CrashReportsEnabled",
                        crash_reports_enabled);
}

}  // namespace

LuxxleBrowserMainExtraParts::LuxxleBrowserMainExtraParts() = default;

LuxxleBrowserMainExtraParts::~LuxxleBrowserMainExtraParts() = default;

void LuxxleBrowserMainExtraParts::PreProfileInit() {
#if BUILDFLAG(ENABLE_EXTENSIONS)
  // Disable warnings related to Manifest V2 deprecation
  extensions::Extension::
      set_silence_deprecated_manifest_version_warnings_for_testing(true);
#endif  // BUILDFLAG(ENABLE_EXTENSIONS)
}

void LuxxleBrowserMainExtraParts::PostBrowserStart() {
  // Removed Luxxle browser process - using Chromium defaults
  // g_luxxle_browser_process->StartLuxxleServices();
}

void LuxxleBrowserMainExtraParts::PreMainMessageLoopRun() {
  // Removed Luxxle P3A service - using Chromium defaults
  // if (g_luxxle_browser_process->p3a_service() != nullptr) {
  //   g_luxxle_browser_process->p3a_service()->Init(
  //       g_browser_process->shared_url_loader_factory());
  // }

  RecordInitialP3AValues();

  // Removed Luxxle window tracker - using Chromium defaults
  // #if !BUILDFLAG(IS_ANDROID)
  // luxxle::LuxxleWindowTracker::CreateInstance(g_browser_process->local_state());
  // #endif  // !BUILDFLAG(IS_ANDROID)
  
  // Removed Luxxle uptime monitor - using Chromium defaults
  // g_luxxle_browser_process->process_misc_metrics()->uptime_monitor()->Init();
}
