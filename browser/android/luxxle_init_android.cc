// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include <string>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "luxxle/browser/android/safe_browsing/buildflags.h"
#include "luxxle/browser/luxxle_browser_process.h"
#include "luxxle/browser/luxxle_stats/luxxle_stats_updater.h"
#include "chrome/android/chrome_jni_headers/LuxxleActivity_jni.h"

namespace chrome {
namespace android {

static void JNI_LuxxleActivity_RestartStatsUpdater(JNIEnv* env) {
  g_luxxle_browser_process->luxxle_stats_updater()->Stop();
  g_luxxle_browser_process->luxxle_stats_updater()->Start();
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_LuxxleActivity_GetSafeBrowsingApiKey(JNIEnv* env) {
  return base::android::ConvertUTF8ToJavaString(
      env, BUILDFLAG(SAFEBROWSING_API_KEY));
}

}  // namespace android
}  // namespace chrome
