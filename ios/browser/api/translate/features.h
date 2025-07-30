// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_API_TRANSLATE_FEATURES_H_
#define LUXXLE_IOS_BROWSER_API_TRANSLATE_FEATURES_H_

#import "base/feature_list.h"

namespace luxxle::features {

// Whether or not to use the new Luxxle-Translate feature
BASE_DECLARE_FEATURE(kLuxxleTranslateEnabled);

// Whether or not to use the new Luxxle-Translate with Apple feature
BASE_DECLARE_FEATURE(kLuxxleAppleTranslateEnabled);

}  // namespace luxxle::features

#endif  // LUXXLE_IOS_BROWSER_API_TRANSLATE_FEATURES_H_
