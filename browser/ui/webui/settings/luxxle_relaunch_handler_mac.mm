/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_relaunch_handler_mac.h"

#import "luxxle/browser/mac/sparkle_glue.h"
#include "luxxle/browser/mac_features.h"

namespace luxxle_relaunch_handler {

bool RelaunchOnMac() {
  return !luxxle::ShouldUseOmaha4() && [SparkleGlue sharedSparkleGlue] &&
         [[SparkleGlue sharedSparkleGlue] relaunch];
}

}  // namespace luxxle_relaunch_handler