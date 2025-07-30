// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/api/translate/features.h"

namespace luxxle::features {

BASE_FEATURE(kLuxxleTranslateEnabled,
             "LuxxleTranslateEnabled",
             base::FEATURE_DISABLED_BY_DEFAULT);

BASE_FEATURE(kLuxxleAppleTranslateEnabled,
             "LuxxleAppleTranslateEnabled",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace luxxle::features
