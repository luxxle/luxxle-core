/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/luxxle_browser_features.h"

#include "build/build_config.h"

namespace features {

BASE_FEATURE(kLuxxleNewTabPageRefreshEnabled,
             "LuxxleNewTabPageRefreshEnabled",
             base::FEATURE_DISABLED_BY_DEFAULT);

// Cleanup Session Cookies on browser restart if Session Restore is enabled.
BASE_FEATURE(kLuxxleCleanupSessionCookiesOnSessionRestore,
             "LuxxleCleanupSessionCookiesOnSessionRestore",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Sanitize url before copying, replaces default ctrl+c hotkey for urls.
BASE_FEATURE(kLuxxleCopyCleanLinkByDefault,
             "luxxle-copy-clean-link-by-default",
#if BUILDFLAG(IS_MAC)
             base::FEATURE_DISABLED_BY_DEFAULT
#else
             base::FEATURE_ENABLED_BY_DEFAULT
#endif
);

BASE_FEATURE(kLuxxleCopyCleanLinkFromJs,
             "LuxxleCopyCleanLinkFromJs",
             base::FEATURE_DISABLED_BY_DEFAULT);

// Disable download warnings for dangerous files when Safe Browsing is
// disabled.
BASE_FEATURE(kLuxxleOverrideDownloadDangerLevel,
             "luxxle-override-download-danger-level",
             base::FEATURE_DISABLED_BY_DEFAULT);

// Adds rounded corners and a drop shadow to the main web content area and side
// bar panels.
BASE_FEATURE(kLuxxleWebViewRoundedCorners,
             "luxxle-web-view-rounded-corners",
             base::FEATURE_DISABLED_BY_DEFAULT);

// Enable "day zero" experiment modifications, for potential user
// retention improvements.
BASE_FEATURE(kLuxxleDayZeroExperiment,
             "LuxxleDayZeroExperiment",
             base::FEATURE_DISABLED_BY_DEFAULT);

#if BUILDFLAG(IS_ANDROID)
// Enable new onboarding on Android
BASE_FEATURE(kNewAndroidOnboarding,
             "NewAndroidOnboarding",
             base::FEATURE_DISABLED_BY_DEFAULT);
#endif  // BUILDFLAG(IS_ANDROID)

// The variant of the "day zero" experiment. i.e. A, B, C, D, etc.
const base::FeatureParam<std::string> kLuxxleDayZeroExperimentVariant{
    &kLuxxleDayZeroExperiment,
    /*name=*/"variant",
    /*default_value=*/""};

}  // namespace features
