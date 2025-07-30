/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_LUXXLE_BROWSER_FEATURES_H_
#define LUXXLE_BROWSER_LUXXLE_BROWSER_FEATURES_H_

#include <string>

#include "base/feature_list.h"
#include "base/metrics/field_trial_params.h"

namespace features {

BASE_DECLARE_FEATURE(kLuxxleNewTabPageRefreshEnabled);
BASE_DECLARE_FEATURE(kLuxxleCleanupSessionCookiesOnSessionRestore);
BASE_DECLARE_FEATURE(kLuxxleCopyCleanLinkByDefault);
BASE_DECLARE_FEATURE(kLuxxleCopyCleanLinkFromJs);
BASE_DECLARE_FEATURE(kLuxxleOverrideDownloadDangerLevel);
BASE_DECLARE_FEATURE(kLuxxleWebViewRoundedCorners);
BASE_DECLARE_FEATURE(kLuxxleDayZeroExperiment);
#if BUILDFLAG(IS_ANDROID)
BASE_DECLARE_FEATURE(kNewAndroidOnboarding);
#endif  // BUILDFLAG(IS_ANDROID)

extern const base::FeatureParam<std::string> kLuxxleDayZeroExperimentVariant;

}  // namespace features

#endif  // LUXXLE_BROWSER_LUXXLE_BROWSER_FEATURES_H_
