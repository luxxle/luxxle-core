/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_LUXXLE_STATS_LUXXLE_STATS_PREFS_H_
#define LUXXLE_IOS_BROWSER_LUXXLE_STATS_LUXXLE_STATS_PREFS_H_

class PrefRegistrySimple;

namespace luxxle_stats {

void RegisterLocalStatePrefs(PrefRegistrySimple* registry);

}  // namespace luxxle_stats

#endif  // LUXXLE_IOS_BROWSER_LUXXLE_STATS_LUXXLE_STATS_PREFS_H_
