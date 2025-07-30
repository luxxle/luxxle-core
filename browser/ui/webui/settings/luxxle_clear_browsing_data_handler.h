// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_CLEAR_BROWSING_DATA_HANDLER_H_
#define LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_CLEAR_BROWSING_DATA_HANDLER_H_

#include "base/values.h"
#include "chrome/browser/ui/webui/settings/settings_clear_browsing_data_handler.h"
#include "components/prefs/pref_change_registrar.h"

class Profile;

namespace settings {

class LuxxleClearBrowsingDataHandler : public ClearBrowsingDataHandler {
 public:
  LuxxleClearBrowsingDataHandler(content::WebUI* webui, Profile* profile);
  LuxxleClearBrowsingDataHandler(const LuxxleClearBrowsingDataHandler&) = delete;
  LuxxleClearBrowsingDataHandler& operator=(
      const LuxxleClearBrowsingDataHandler&) = delete;
  ~LuxxleClearBrowsingDataHandler() override;

 private:
  // ClearBrowsingDataHandler:
  void RegisterMessages() override;

  void HandleGetLuxxleRewardsEnabled(const base::Value::List& args);

  void HandleClearLuxxleAdsData(const base::Value::List& args);

  void OnRewardsEnabledPreferenceChanged();

  raw_ptr<Profile> profile_ = nullptr;

  PrefChangeRegistrar pref_change_registrar_;
};

}  // namespace settings

#endif  // LUXXLE_BROWSER_UI_WEBUI_SETTINGS_LUXXLE_CLEAR_BROWSING_DATA_HANDLER_H_
