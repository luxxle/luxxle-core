/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/android/luxxle_shields_content_settings.h"

#include <string>

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "luxxle/browser/luxxle_browser_process.h"
#include "luxxle/components/luxxle_shields/content/browser/ad_block_service.h"
#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_util.h"
#include "chrome/android/chrome_jni_headers/LuxxleShieldsContentSettings_jni.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/cookie_settings_factory.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "components/content_settings/core/browser/content_settings_utils.h"
#include "components/content_settings/core/browser/cookie_settings.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "url/gurl.h"

namespace chrome {
namespace android {


// That class is linked to a global toolbar. It's a one instance on Android
LuxxleShieldsContentSettings* g_luxxle_shields_content_settings = nullptr;

static void JNI_LuxxleShieldsContentSettings_Init(JNIEnv* env,
    const base::android::JavaParamRef<jobject>& jcaller) {
  g_luxxle_shields_content_settings =
      new LuxxleShieldsContentSettings(env, jcaller);
}

LuxxleShieldsContentSettings::LuxxleShieldsContentSettings(
    JNIEnv* env,
    const base::android::JavaRef<jobject>& obj)
    : jobj_(base::android::ScopedJavaGlobalRef<jobject>(obj)) {
  Java_LuxxleShieldsContentSettings_setNativePtr(env, obj,
      reinterpret_cast<intptr_t>(this));
}

LuxxleShieldsContentSettings::~LuxxleShieldsContentSettings() {
}

void LuxxleShieldsContentSettings::Destroy(JNIEnv* env) {
  g_luxxle_shields_content_settings = nullptr;
  delete this;
}

void LuxxleShieldsContentSettings::DispatchBlockedEventToJava(int tab_id,
        const std::string& block_type, const std::string& subresource) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_LuxxleShieldsContentSettings_blockedEvent(
      env, jobj_, tab_id,
      base::android::ConvertUTF8ToJavaString(env, block_type),
      base::android::ConvertUTF8ToJavaString(env, subresource));
}

void LuxxleShieldsContentSettings::DispatchSavedBandwidthToJava(
  uint64_t savings) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_LuxxleShieldsContentSettings_savedBandwidth(env, jobj_, savings);
}

void LuxxleShieldsContentSettings::DispatchSavedBandwidth(uint64_t savings) {
  DCHECK(g_luxxle_shields_content_settings);
  if (!g_luxxle_shields_content_settings) {
    return;
  }
  g_luxxle_shields_content_settings->DispatchSavedBandwidthToJava(savings);
}

// static
void LuxxleShieldsContentSettings::DispatchBlockedEvent(int tab_id,
  const std::string& block_type, const std::string& subresource) {
  DCHECK(g_luxxle_shields_content_settings);
  if (!g_luxxle_shields_content_settings) {
    return;
  }
  g_luxxle_shields_content_settings->DispatchBlockedEventToJava(tab_id,
      block_type, subresource);
}

void JNI_LuxxleShieldsContentSettings_SetLuxxleShieldsEnabled(JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::SetLuxxleShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      enabled, GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_LuxxleShieldsContentSettings_GetLuxxleShieldsEnabled(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return luxxle_shields::GetLuxxleShieldsEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

void JNI_LuxxleShieldsContentSettings_SetAdControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::SetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      luxxle_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_LuxxleShieldsContentSettings_GetAdControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::ControlType control_type = luxxle_shields::GetAdControlType(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      luxxle_shields::ControlTypeToString(control_type));
}

void JNI_LuxxleShieldsContentSettings_SetCookieControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      Profile::FromJavaObject(j_profile)->GetPrefs(),
      luxxle_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

void JNI_LuxxleShieldsContentSettings_SetCosmeticFilteringControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::SetCosmeticFilteringControlType(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      luxxle_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state(),
      Profile::FromJavaObject(j_profile)->GetPrefs());
}

void JNI_LuxxleShieldsContentSettings_ResetCosmeticFilter(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url) {
  g_luxxle_browser_process->ad_block_service()->ResetCosmeticFilter(
      GURL(base::android::ConvertJavaStringToUTF8(env, url)).host());
}

jboolean JNI_LuxxleShieldsContentSettings_AreAnyBlockedElementsPresent(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url) {
  return g_luxxle_browser_process->ad_block_service()
      ->AreAnyBlockedElementsPresent(
          GURL(base::android::ConvertJavaStringToUTF8(env, url)).host());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_LuxxleShieldsContentSettings_GetCookieControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::ControlType control_type = luxxle_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      CookieSettingsFactory::GetForProfile(Profile::FromJavaObject(j_profile))
          .get(),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      luxxle_shields::ControlTypeToString(control_type));
}

void JNI_LuxxleShieldsContentSettings_SetFingerprintingControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::SetFingerprintingControlType(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      luxxle_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state(),
      Profile::FromJavaObject(j_profile)->GetPrefs());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_LuxxleShieldsContentSettings_GetFingerprintingControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::ControlType control_type =
      luxxle_shields::GetFingerprintingControlType(
          HostContentSettingsMapFactory::GetForProfile(
              Profile::FromJavaObject(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(
      env, luxxle_shields::ControlTypeToString(control_type));
}

void JNI_LuxxleShieldsContentSettings_SetHttpsUpgradeControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::SetHttpsUpgradeControlType(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      luxxle_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
JNI_LuxxleShieldsContentSettings_GetHttpsUpgradeControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::ControlType control_type =
      luxxle_shields::GetHttpsUpgradeControlType(
          HostContentSettingsMapFactory::GetForProfile(
              Profile::FromJavaObject(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      luxxle_shields::ControlTypeToString(control_type));
}

base::android::ScopedJavaLocalRef<jstring>
JNI_LuxxleShieldsContentSettings_GetCosmeticFilteringControlType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::ControlType cosmetic_type =
      luxxle_shields::GetCosmeticFilteringControlType(
          HostContentSettingsMapFactory::GetForProfile(
              Profile::FromJavaObject(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(
      env, luxxle_shields::ControlTypeToString(cosmetic_type));
}

void JNI_LuxxleShieldsContentSettings_SetNoScriptControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::SetNoScriptControlType(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      luxxle_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
    JNI_LuxxleShieldsContentSettings_GetNoScriptControlType(JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::ControlType control_type =
      luxxle_shields::GetNoScriptControlType(
          HostContentSettingsMapFactory::GetForProfile(
              Profile::FromJavaObject(j_profile)),
          GURL(base::android::ConvertJavaStringToUTF8(env, url)));

  return base::android::ConvertUTF8ToJavaString(env,
      luxxle_shields::ControlTypeToString(control_type));
}

void JNI_LuxxleShieldsContentSettings_SetForgetFirstPartyStorageEnabled(
    JNIEnv* env,
    jboolean enabled,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  luxxle_shields::SetForgetFirstPartyStorageEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      enabled, GURL(base::android::ConvertJavaStringToUTF8(env, url)),
      g_browser_process->local_state());
}

jboolean JNI_LuxxleShieldsContentSettings_GetForgetFirstPartyStorageEnabled(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& url,
    const base::android::JavaParamRef<jobject>& j_profile) {
  return luxxle_shields::GetForgetFirstPartyStorageEnabled(
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromJavaObject(j_profile)),
      GURL(base::android::ConvertJavaStringToUTF8(env, url)));
}

}  // namespace android
}  // namespace chrome
