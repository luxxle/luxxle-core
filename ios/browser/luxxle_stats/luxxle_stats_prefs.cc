/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/luxxle_stats/luxxle_stats_prefs.h"

#include "base/time/time.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "components/prefs/pref_registry_simple.h"

namespace luxxle_stats {

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  registry->RegisterTimePref(kLuxxleWalletPingReportedUnlockTime, base::Time());
}

}  // namespace luxxle_stats
