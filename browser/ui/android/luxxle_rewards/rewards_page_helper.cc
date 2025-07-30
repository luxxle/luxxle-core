// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/android/luxxle_rewards/rewards_page_helper.h"

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "luxxle/build/android/jni_headers/TabUtils_jni.h"

namespace luxxle_rewards {

void OpenURLForRewardsPage(const std::string& url) {
  Java_TabUtils_openURLWithLuxxleActivity(
      base::android::AttachCurrentThread(),
      base::android::ConvertUTF8ToJavaString(
          base::android::AttachCurrentThread(), url));
}

}  // namespace luxxle_rewards
