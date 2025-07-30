// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_PERF_LUXXLE_PERF_SWITCHES_H_
#define LUXXLE_BROWSER_PERF_LUXXLE_PERF_SWITCHES_H_

namespace perf::switches {

// All switches in alphabetical order.

// Enables some Luxxle's widely used features for a testing profile in perf
// tests. --user-data-dir should be set.
inline constexpr char kEnableLuxxleFeaturesForPerfTesting[] =
    "enable-luxxle-features-for-perf-testing";

}  // namespace perf::switches

#endif  // LUXXLE_BROWSER_PERF_LUXXLE_PERF_SWITCHES_H_
