// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/luxxle_ui_features.h"

namespace features {

BASE_FEATURE(kLuxxleNtpSearchWidget,
             "LuxxleNtpSearchWidget",
             base::FEATURE_ENABLED_BY_DEFAULT);

#if BUILDFLAG(IS_WIN)
// Enables window cloaking on window creation to prevent a white flash.
BASE_FEATURE(kLuxxleWorkaroundNewWindowFlash,
             "LuxxleWorkaroundNewWindowFlash",
             base::FEATURE_ENABLED_BY_DEFAULT);
#endif  // BUILDFLAG(IS_WIN)

}  // namespace features
