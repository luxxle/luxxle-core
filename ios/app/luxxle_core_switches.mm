/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/app/luxxle_core_switches.h"

#include "base/base_switches.h"
#include "base/strings/sys_string_conversions.h"
#include "luxxle/components/luxxle_component_updater/browser/switches.h"
#include "luxxle/components/p3a/switches.h"
#include "luxxle/components/variations/switches.h"
#include "components/component_updater/component_updater_switches.h"
#include "components/sync/base/command_line_switches.h"
#include "components/variations/variations_switches.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyComponentUpdater =
    base::SysUTF8ToNSString(switches::kComponentUpdater);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyVModule =
    base::SysUTF8ToNSString(switches::kVModule);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeySyncURL =
    base::SysUTF8ToNSString(syncer::kSyncServiceURL);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyVariationsPR =
    base::SysUTF8ToNSString(variations::switches::kVariationsPR);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyVariationsURL =
    base::SysUTF8ToNSString(variations::switches::kVariationsServerURL);
// There is no exposed switch for rewards
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyRewardsFlags = @"rewards";
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AUploadIntervalSeconds =
    base::SysUTF8ToNSString(p3a::switches::kP3AUploadIntervalSeconds);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3ADoNotRandomizeUploadInterval =
    base::SysUTF8ToNSString(p3a::switches::kP3ADoNotRandomizeUploadInterval);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3ATypicalRotationIntervalSeconds =
    base::SysUTF8ToNSString(p3a::switches::kP3ATypicalRotationIntervalSeconds);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AExpressRotationIntervalSeconds =
    base::SysUTF8ToNSString(p3a::switches::kP3AExpressRotationIntervalSeconds);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3ASlowRotationIntervalSeconds =
    base::SysUTF8ToNSString(p3a::switches::kP3ASlowRotationIntervalSeconds);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AFakeTypicalStarEpoch =
    base::SysUTF8ToNSString(p3a::switches::kP3AFakeTypicalStarEpoch);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AFakeSlowStarEpoch =
    base::SysUTF8ToNSString(p3a::switches::kP3AFakeSlowStarEpoch);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AFakeExpressStarEpoch =
    base::SysUTF8ToNSString(p3a::switches::kP3AFakeExpressStarEpoch);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AJsonUploadServerURL =
    base::SysUTF8ToNSString(p3a::switches::kP3AJsonUploadUrl);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3ACreativeUploadServerURL =
    base::SysUTF8ToNSString(p3a::switches::kP3ACreativeUploadUrl);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP2AJsonUploadServerURL =
    base::SysUTF8ToNSString(p3a::switches::kP2AJsonUploadUrl);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AConstellationUploadServerHost =
    base::SysUTF8ToNSString(p3a::switches::kP3AConstellationUploadHost);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3ADisableStarAttestation =
    base::SysUTF8ToNSString(p3a::switches::kP3ADisableStarAttestation);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AStarRandomnessHost =
    base::SysUTF8ToNSString(p3a::switches::kP3AStarRandomnessHost);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyP3AIgnoreServerErrors =
    base::SysUTF8ToNSString(p3a::switches::kP3AIgnoreServerErrors);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyUseDevGoUpdater =
    base::SysUTF8ToNSString(luxxle_component_updater::kUseGoUpdateDev);
const LuxxleCoreSwitchKey LuxxleCoreSwitchKeyServicesEnvironment =
    @"luxxle-services-env";

@implementation LuxxleCoreSwitch
- (instancetype)initWithKey:(LuxxleCoreSwitchKey)key {
  return [self initWithKey:key value:nil];
}
- (instancetype)initWithKey:(LuxxleCoreSwitchKey)key
                      value:(nullable NSString*)value {
  if ((self = [super init])) {
    _key = [key copy];
    _value = [value copy];
  }
  return self;
}
@end
