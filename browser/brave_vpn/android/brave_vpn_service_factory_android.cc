/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/browser/brave_vpn/brave_vpn_service_factory.h"

#include "base/android/jni_android.h"
#include "chrome/android/chrome_jni_headers/BraveVpnServiceFactoryAndroid_jni.h"
#include "chrome/browser/profiles/profile.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

namespace chrome {
namespace android {
static jlong JNI_BraveVpnServiceFactoryAndroid_GetInterfaceToVpnService(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& profile_android) {
  auto* profile = Profile::FromJavaObject(profile_android);
  if (profile == nullptr) {
    return static_cast<jlong>(-1);
  }
  auto pending =
      brave_vpn::BraveVpnServiceFactory::GetInstance()->GetRemoteForProfile(
          profile);

  return static_cast<jlong>(pending.PassPipe().release().value());
}

}  // namespace android
}  // namespace chrome
