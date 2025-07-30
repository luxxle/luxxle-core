// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/webui/settings/luxxle_clear_browsing_data_handler.h"

#include "base/functional/bind.h"
#include "base/functional/callback_helpers.h"
// REMOVED: #include "luxxle/browser/luxxle_ads/.*"
// REMOVED: #include "luxxle/components/luxxle_ads/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"

namespace settings {

LuxxleClearBrowsingDataHandler::LuxxleClearBrowsingDataHandler(
    content::WebUI* webui,
    Profile* profile)
    : ClearBrowsingDataHandler(webui, profile), profile_(profile) {
  CHECK(profile_);

  pref_change_registrar_.Init(profile_->GetPrefs());
  pref_change_registrar_.Add(
      luxxle_rewards::prefs::kEnabled,
      base::BindRepeating(
          &LuxxleClearBrowsingDataHandler::OnRewardsEnabledPreferenceChanged,
          base::Unretained(this)));
}

LuxxleClearBrowsingDataHandler::~LuxxleClearBrowsingDataHandler() = default;

void LuxxleClearBrowsingDataHandler::RegisterMessages() {
  ClearBrowsingDataHandler::RegisterMessages();

  web_ui()->RegisterMessageCallback(
      "getLuxxleRewardsEnabled",
      base::BindRepeating(
          &LuxxleClearBrowsingDataHandler::HandleGetLuxxleRewardsEnabled,
          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "clearLuxxleAdsData",
      base::BindRepeating(
          &LuxxleClearBrowsingDataHandler::HandleClearLuxxleAdsData,
          base::Unretained(this)));
}

void LuxxleClearBrowsingDataHandler::HandleGetLuxxleRewardsEnabled(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);

  const bool rewards_enabled =
      profile_->GetPrefs()->GetBoolean(luxxle_rewards::prefs::kEnabled);

  AllowJavascript();
  ResolveJavascriptCallback(args[0], rewards_enabled);
}

void LuxxleClearBrowsingDataHandler::HandleClearLuxxleAdsData(
    const base::Value::List& /*args*/) {
  if (auto* ads_service =
          luxxle_ads::AdsServiceFactory::GetForProfile(profile_)) {
    ads_service->ClearData(/*intentional*/ base::DoNothing());
  }
}

void LuxxleClearBrowsingDataHandler::OnRewardsEnabledPreferenceChanged() {
  if (!IsJavascriptAllowed()) {
    return;
  }

  const bool rewards_enabled =
      profile_->GetPrefs()->GetBoolean(luxxle_rewards::prefs::kEnabled);
  FireWebUIListener("luxxle-rewards-enabled-changed",
                    base::Value(rewards_enabled));
}

}  // namespace settings
