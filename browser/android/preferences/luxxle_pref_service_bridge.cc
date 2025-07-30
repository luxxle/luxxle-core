/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <jni.h>

#include <string>

#include "base/android/jni_string.h"
#include "luxxle/build/android/jni_headers/LuxxlePrefServiceBridge_jni.h"
#include "luxxle/components/luxxle_news/common/pref_names.h"
#include "luxxle/components/luxxle_perf_predictor/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_util.h"
#include "luxxle/components/luxxle_sync/luxxle_sync_prefs.h"
#include "luxxle/components/constants/pref_names.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/content_settings/cookie_settings_factory.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/common/pref_names.h"
#include "components/content_settings/core/browser/cookie_settings.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/prefs/pref_service.h"
#include "third_party/blink/public/common/peerconnection/webrtc_ip_handling_policy.h"
#include "url/gurl.h"

using base::android::ConvertUTF8ToJavaString;
using base::android::JavaParamRef;
using base::android::ScopedJavaLocalRef;
using luxxle_shields::ControlType;

namespace {

Profile* GetOriginalProfile() {
  return ProfileManager::GetActiveUserProfile()->GetOriginalProfile();
}

enum WebRTCIPHandlingPolicy {
  DEFAULT,
  DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES,
  DEFAULT_PUBLIC_INTERFACE_ONLY,
  DISABLE_NON_PROXIED_UDP,
};

WebRTCIPHandlingPolicy GetWebRTCIPHandlingPolicy(
    const std::string& preference) {
  if (preference == blink::kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces)
    return DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES;
  if (preference == blink::kWebRTCIPHandlingDefaultPublicInterfaceOnly)
    return DEFAULT_PUBLIC_INTERFACE_ONLY;
  if (preference == blink::kWebRTCIPHandlingDisableNonProxiedUdp)
    return DISABLE_NON_PROXIED_UDP;
  return DEFAULT;
}

std::string GetWebRTCIPHandlingPreference(WebRTCIPHandlingPolicy policy) {
  if (policy == DEFAULT_PUBLIC_AND_PRIVATE_INTERFACES)
    return blink::kWebRTCIPHandlingDefaultPublicAndPrivateInterfaces;
  if (policy == DEFAULT_PUBLIC_INTERFACE_ONLY)
    return blink::kWebRTCIPHandlingDefaultPublicInterfaceOnly;
  if (policy == DISABLE_NON_PROXIED_UDP)
    return blink::kWebRTCIPHandlingDisableNonProxiedUdp;
  return blink::kWebRTCIPHandlingDefault;
}

}  // namespace

namespace chrome {
namespace android {

// This file is deprecated, prefs should be accessed directly from Java
void JNI_LuxxlePrefServiceBridge_SetCookiesBlockType(
    JNIEnv* env,
    const base::android::JavaParamRef<jstring>& type) {
  luxxle_shields::SetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      GetOriginalProfile()->GetPrefs(),
      luxxle_shields::ControlTypeFromString(
          base::android::ConvertJavaStringToUTF8(env, type)),
      GURL(), g_browser_process->local_state());
}

base::android::ScopedJavaLocalRef<jstring>
JNI_LuxxlePrefServiceBridge_GetCookiesBlockType(JNIEnv* env) {
  luxxle_shields::ControlType control_type = luxxle_shields::GetCookieControlType(
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile()),
      CookieSettingsFactory::GetForProfile(GetOriginalProfile()).get(), GURL());
  return base::android::ConvertUTF8ToJavaString(
      env, luxxle_shields::ControlTypeToString(control_type));
}

void JNI_LuxxlePrefServiceBridge_SetPlayYTVideoInBrowserEnabled(
    JNIEnv* env,
    jboolean enabled) {
  return GetOriginalProfile()->GetPrefs()->SetBoolean(
      kPlayYTVideoInBrowserEnabled, enabled);
}

jboolean JNI_LuxxlePrefServiceBridge_GetPlayYTVideoInBrowserEnabled(
    JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      kPlayYTVideoInBrowserEnabled);
}

void JNI_LuxxlePrefServiceBridge_SetDesktopModeEnabled(JNIEnv* env,
                                                      jboolean enabled) {
  return GetOriginalProfile()->GetPrefs()->SetBoolean(kDesktopModeEnabled,
                                                      enabled);
}

jboolean JNI_LuxxlePrefServiceBridge_GetDesktopModeEnabled(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(kDesktopModeEnabled);
}

jlong JNI_LuxxlePrefServiceBridge_GetTrackersBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
  Profile* profile = Profile::FromJavaObject(j_profile);
  return profile->GetPrefs()->GetUint64(kTrackersBlocked);
}

jlong JNI_LuxxlePrefServiceBridge_GetAdsBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
  Profile* profile = Profile::FromJavaObject(j_profile);
  return profile->GetPrefs()->GetUint64(kAdsBlocked);
}

jlong JNI_LuxxlePrefServiceBridge_GetDataSaved(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile) {
  Profile* profile = Profile::FromJavaObject(j_profile);
  return profile->GetPrefs()->GetUint64(
      luxxle_perf_predictor::prefs::kBandwidthSavedBytes);
}

void JNI_LuxxlePrefServiceBridge_SetOldTrackersBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = Profile::FromJavaObject(j_profile);
  profile->GetPrefs()->SetUint64(
      kTrackersBlocked,
      count + profile->GetPrefs()->GetUint64(kTrackersBlocked));
}

void JNI_LuxxlePrefServiceBridge_SetOldAdsBlockedCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = Profile::FromJavaObject(j_profile);
  profile->GetPrefs()->SetUint64(
      kAdsBlocked, count + profile->GetPrefs()->GetUint64(kAdsBlocked));
}

void JNI_LuxxlePrefServiceBridge_SetOldHttpsUpgradesCount(
    JNIEnv* env,
    const base::android::JavaParamRef<jobject>& j_profile,
    jlong count) {
  if (count <= 0) {
    return;
  }
  Profile* profile = Profile::FromJavaObject(j_profile);
  profile->GetPrefs()->SetUint64(
      kHttpsUpgrades, count + profile->GetPrefs()->GetUint64(kHttpsUpgrades));
}

void JNI_LuxxlePrefServiceBridge_ResetPromotionLastFetchStamp(JNIEnv* env) {
  GetOriginalProfile()->GetPrefs()->SetUint64(
      luxxle_rewards::prefs::kPromotionLastFetchStamp, 0);
}

jboolean JNI_LuxxlePrefServiceBridge_GetBooleanForContentSetting(JNIEnv* env,
                                                                jint type) {
  HostContentSettingsMap* content_settings =
      HostContentSettingsMapFactory::GetForProfile(GetOriginalProfile());
  switch (content_settings->GetDefaultContentSetting((ContentSettingsType)type,
                                                     nullptr)) {
    case CONTENT_SETTING_ALLOW:
      return true;
    case CONTENT_SETTING_BLOCK:
    default:
      return false;
  }
}

jint JNI_LuxxlePrefServiceBridge_GetWebrtcPolicy(JNIEnv* env) {
  return static_cast<int>(
      GetWebRTCIPHandlingPolicy(GetOriginalProfile()->GetPrefs()->GetString(
          prefs::kWebRTCIPHandlingPolicy)));
}

void JNI_LuxxlePrefServiceBridge_SetWebrtcPolicy(JNIEnv* env, jint policy) {
  GetOriginalProfile()->GetPrefs()->SetString(
      prefs::kWebRTCIPHandlingPolicy,
      GetWebRTCIPHandlingPreference((WebRTCIPHandlingPolicy)policy));
}

void JNI_LuxxlePrefServiceBridge_SetNewsOptIn(JNIEnv* env, jboolean value) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      luxxle_news::prefs::kLuxxleNewsOptedIn, value);
}

jboolean JNI_LuxxlePrefServiceBridge_GetNewsOptIn(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      luxxle_news::prefs::kLuxxleNewsOptedIn);
}

void JNI_LuxxlePrefServiceBridge_SetShowNews(JNIEnv* env, jboolean value) {
  GetOriginalProfile()->GetPrefs()->SetBoolean(
      luxxle_news::prefs::kNewTabPageShowToday, value);
}

jboolean JNI_LuxxlePrefServiceBridge_GetShowNews(JNIEnv* env) {
  return GetOriginalProfile()->GetPrefs()->GetBoolean(
      luxxle_news::prefs::kNewTabPageShowToday);
}

}  // namespace android
}  // namespace chrome
