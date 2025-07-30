// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/android/ai_chat/luxxle_leo_settings_launcher_helper.h"

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "luxxle/build/android/jni_headers/LuxxleLeoSettingsLauncherHelper_jni.h"
#include "content/public/browser/web_contents.h"

namespace ai_chat {

void ShowLuxxleLeoSettings(content::WebContents* web_contents) {
  Java_LuxxleLeoSettingsLauncherHelper_showLuxxleLeoSettings(
      base::android::AttachCurrentThread(), web_contents->GetJavaWebContents());
}

void GoPremium(content::WebContents* web_contents) {
  Java_LuxxleLeoSettingsLauncherHelper_goPremium(
      base::android::AttachCurrentThread(), web_contents->GetJavaWebContents());
}

void ManagePremium(content::WebContents* web_contents) {
  Java_LuxxleLeoSettingsLauncherHelper_managePremium(
      base::android::AttachCurrentThread(), web_contents->GetJavaWebContents());
}

void OpenURL(const std::string& url) {
  Java_LuxxleLeoSettingsLauncherHelper_openURL(
      base::android::AttachCurrentThread(),
      base::android::ConvertUTF8ToJavaString(
          base::android::AttachCurrentThread(), url));
}

void HandleVoiceRecognition(content::WebContents* web_contents,
                            const std::string& conversation_uuid) {
  Java_LuxxleLeoSettingsLauncherHelper_handleVoiceRecognition(
      base::android::AttachCurrentThread(), web_contents->GetJavaWebContents(),
      base::android::ConvertUTF8ToJavaString(
          base::android::AttachCurrentThread(), conversation_uuid));
}

void CloseActivity(content::WebContents* web_contents) {
  Java_LuxxleLeoSettingsLauncherHelper_closeActivity(
      base::android::AttachCurrentThread(), web_contents->GetJavaWebContents());
}

void HandleShowSoftKeyboard(content::WebContents* web_contents) {
  Java_LuxxleLeoSettingsLauncherHelper_handleShowSoftKeyboard(
      base::android::AttachCurrentThread(), web_contents->GetJavaWebContents());
}

}  // namespace ai_chat
