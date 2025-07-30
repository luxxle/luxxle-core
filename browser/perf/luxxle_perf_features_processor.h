// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_PERF_LUXXLE_PERF_FEATURES_PROCESSOR_H_
#define LUXXLE_BROWSER_PERF_LUXXLE_PERF_FEATURES_PROCESSOR_H_

class Profile;

namespace perf {

// A handler for --enable-luxxle-features-for-perf-testing switch.
void MaybeEnableLuxxleFeatureForPerfTesting(Profile* profile);

}  // namespace perf

#endif  // LUXXLE_BROWSER_PERF_LUXXLE_PERF_FEATURES_PROCESSOR_H_
