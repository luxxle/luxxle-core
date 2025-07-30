/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_privacy_handler.h"

#include "base/functional/bind.h"
#include "base/values.h"
#include "luxxle/components/ai_chat/core/browser/utils.h"
#include "luxxle/components/ai_chat/core/common/features.h"
#include "luxxle/components/luxxle_shields/core/common/features.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/de_amp/common/features.h"
#include "luxxle/components/debounce/core/common/features.h"
#include "luxxle/components/google_sign_in_permission/google_sign_in_permission_util.h"
#include "luxxle/components/p3a/pref_names.h"
#include "luxxle/components/request_otr/common/buildflags/buildflags.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "components/gcm_driver/gcm_buildflags.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"
#include "content/public/browser/web_ui_data_source.h"
#include "third_party/blink/public/common/peerconnection/webrtc_ip_handling_policy.h"

#if BUILDFLAG(ENABLE_REQUEST_OTR)
#include "luxxle/components/request_otr/common/features.h"
#endif

#if !BUILDFLAG(USE_GCM_FROM_PLATFORM)
#include "luxxle/browser/gcm_driver/luxxle_gcm_channel_status.h"
#endif

LuxxlePrivacyHandler::LuxxlePrivacyHandler() {
  local_state_change_registrar_.Init(g_browser_process->local_state());
  local_state_change_registrar_.Add(
      kStatsReportingEnabled,
      base::BindRepeating(&LuxxlePrivacyHandler::OnStatsUsagePingEnabledChanged,
                          base::Unretained(this)));
  local_state_change_registrar_.Add(
      p3a::kP3AEnabled,
      base::BindRepeating(&LuxxlePrivacyHandler::OnP3AEnabledChanged,
                          base::Unretained(this)));
}

LuxxlePrivacyHandler::~LuxxlePrivacyHandler() {
  local_state_change_registrar_.RemoveAll();
}

void LuxxlePrivacyHandler::RegisterMessages() {
  profile_ = Profile::FromWebUI(web_ui());

  web_ui()->RegisterMessageCallback(
      "setP3AEnabled", base::BindRepeating(&LuxxlePrivacyHandler::SetP3AEnabled,
                                           base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getP3AEnabled", base::BindRepeating(&LuxxlePrivacyHandler::GetP3AEnabled,
                                           base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setStatsUsagePingEnabled",
      base::BindRepeating(&LuxxlePrivacyHandler::SetStatsUsagePingEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getStatsUsagePingEnabled",
      base::BindRepeating(&LuxxlePrivacyHandler::GetStatsUsagePingEnabled,
                          base::Unretained(this)));
}

// static
void LuxxlePrivacyHandler::AddLoadTimeData(content::WebUIDataSource* data_source,
                                          Profile* profile) {
#if BUILDFLAG(USE_GCM_FROM_PLATFORM)
  data_source->AddBoolean("pushMessagingEnabledAtStartup", true);
#else
  gcm::LuxxleGCMChannelStatus* gcm_channel_status =
      gcm::LuxxleGCMChannelStatus::GetForProfile(profile);

  DCHECK(gcm_channel_status);
  data_source->AddBoolean("pushMessagingEnabledAtStartup",
                          gcm_channel_status->IsGCMEnabled());
#endif
  data_source->AddBoolean(
      "isDeAmpFeatureEnabled",
      base::FeatureList::IsEnabled(de_amp::features::kLuxxleDeAMP));
  data_source->AddBoolean(
      "isDebounceFeatureEnabled",
      base::FeatureList::IsEnabled(debounce::features::kLuxxleDebounce));
#if BUILDFLAG(ENABLE_REQUEST_OTR)
  data_source->AddBoolean(
      "isRequestOTRFeatureEnabled",
      base::FeatureList::IsEnabled(request_otr::features::kLuxxleRequestOTRTab));
#endif
  data_source->AddBoolean(
      "isGoogleSignInFeatureEnabled",
      google_sign_in_permission::IsGoogleSignInFeatureEnabled());
  data_source->AddBoolean(
      "isLocalhostAccessFeatureEnabled",
      base::FeatureList::IsEnabled(
          luxxle_shields::features::kLuxxleLocalhostAccessPermission));
  data_source->AddBoolean(
      "isOpenAIChatFromLuxxleSearchEnabled",
      ai_chat::IsAIChatEnabled(profile->GetPrefs()) &&
          ai_chat::features::IsOpenAIChatFromLuxxleSearchEnabled());
}

void LuxxlePrivacyHandler::SetLocalStateBooleanEnabled(
    const std::string& path,
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  if (!args[0].is_bool()) {
    return;
  }

  bool enabled = args[0].GetBool();
  PrefService* local_state = g_browser_process->local_state();
  local_state->SetBoolean(path, enabled);
}

void LuxxlePrivacyHandler::GetLocalStateBooleanEnabled(
    const std::string& path,
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);

  PrefService* local_state = g_browser_process->local_state();
  bool enabled = local_state->GetBoolean(path);

  AllowJavascript();
  ResolveJavascriptCallback(args[0], base::Value(enabled));
}

void LuxxlePrivacyHandler::SetStatsUsagePingEnabled(
    const base::Value::List& args) {
  SetLocalStateBooleanEnabled(kStatsReportingEnabled, args);
}

void LuxxlePrivacyHandler::GetStatsUsagePingEnabled(
    const base::Value::List& args) {
  GetLocalStateBooleanEnabled(kStatsReportingEnabled, args);
}

void LuxxlePrivacyHandler::OnStatsUsagePingEnabledChanged() {
  if (IsJavascriptAllowed()) {
    PrefService* local_state = g_browser_process->local_state();
    bool enabled = local_state->GetBoolean(kStatsReportingEnabled);

    FireWebUIListener("stats-usage-ping-enabled-changed", base::Value(enabled));
  }
}

void LuxxlePrivacyHandler::SetP3AEnabled(const base::Value::List& args) {
  SetLocalStateBooleanEnabled(p3a::kP3AEnabled, args);
}

void LuxxlePrivacyHandler::GetP3AEnabled(const base::Value::List& args) {
  GetLocalStateBooleanEnabled(p3a::kP3AEnabled, args);
}

void LuxxlePrivacyHandler::OnP3AEnabledChanged() {
  if (IsJavascriptAllowed()) {
    PrefService* local_state = g_browser_process->local_state();
    bool enabled = local_state->GetBoolean(p3a::kP3AEnabled);

    FireWebUIListener("p3a-enabled-changed", base::Value(enabled));
  }
}
