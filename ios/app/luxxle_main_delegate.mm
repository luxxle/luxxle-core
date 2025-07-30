/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/app/luxxle_main_delegate.h"

#include "base/apple/bundle_locations.h"
#include "base/base_paths.h"
#include "base/base_switches.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/path_service.h"
#include "luxxle/components/luxxle_component_updater/browser/luxxle_component.h"
#include "luxxle/components/luxxle_component_updater/browser/features.h"
#include "luxxle/components/luxxle_component_updater/browser/switches.h"
#include "luxxle/components/luxxle_sync/buildflags.h"
#include "luxxle/components/update_client/buildflags.h"
#include "luxxle/components/variations/command_line_utils.h"
#include "components/browser_sync/browser_sync_switches.h"
#include "components/component_updater/component_updater_switches.h"
#include "components/sync/base/command_line_switches.h"
#include "components/sync/base/data_type.h"
#include "components/variations/variations_switches.h"
#include "ios/chrome/browser/flags/chrome_switches.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

// Dummy class used to locate the containing NSBundle
@interface BundleLookupClass : NSObject
@end

@implementation BundleLookupClass
@end

namespace {

std::string GetUpdateURLHost() {
  const base::CommandLine& command_line =
      *base::CommandLine::ForCurrentProcess();
  if (!command_line.HasSwitch(luxxle_component_updater::kUseGoUpdateDev) &&
      !base::FeatureList::IsEnabled(
          luxxle_component_updater::kUseDevUpdaterUrl)) {
    return BUILDFLAG(UPDATER_PROD_ENDPOINT);
  }
  return BUILDFLAG(UPDATER_DEV_ENDPOINT);
}

}  // namespace

LuxxleMainDelegate::LuxxleMainDelegate() {
  NSBundle* bundle = [NSBundle bundleForClass:[BundleLookupClass class]];
  base::apple::SetOverrideOuterBundle(bundle);
  base::apple::SetOverrideFrameworkBundle(bundle);
}

LuxxleMainDelegate::~LuxxleMainDelegate() {}

void LuxxleMainDelegate::BasicStartupComplete() {
  auto* command_line(base::CommandLine::ForCurrentProcess());
  if (!command_line->HasSwitch(switches::kComponentUpdater)) {
    std::string source = "url-source=" + ::GetUpdateURLHost();
    command_line->AppendSwitchASCII(switches::kComponentUpdater,
                                    source.c_str());
  }

  // Luxxle's sync protocol does not use the sync service url
  if (!command_line->HasSwitch(syncer::kSyncServiceURL)) {
    command_line->AppendSwitchASCII(syncer::kSyncServiceURL,
                                    BUILDFLAG(LUXXLE_SYNC_ENDPOINT));
  }

  variations::AppendLuxxleCommandLineOptions(*command_line);

  if (!command_line->HasSwitch(switches::kVModule)) {
    command_line->AppendSwitchASCII(switches::kVModule, "*/luxxle/*=0");
  }

  IOSChromeMainDelegate::BasicStartupComplete();
}
