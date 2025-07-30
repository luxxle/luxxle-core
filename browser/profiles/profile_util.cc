/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/profiles/profile_util.h"

#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_p3a.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/search_engines/luxxle_prepopulated_engines.h"
#include "chrome/browser/profiles/profile.h"
#include "components/content_settings/core/browser/cookie_settings.h"
#include "components/content_settings/core/common/pref_names.h"
#include "components/prefs/pref_service.h"
#include "components/search_engines/search_engines_pref_names.h"

namespace luxxle {

void SetDefaultSearchVersion(Profile* profile, bool is_new_profile) {
  const PrefService::Preference* pref_default_search_version =
      profile->GetPrefs()->FindPreference(prefs::kLuxxleDefaultSearchVersion);
  if (!pref_default_search_version->HasUserSetting()) {
    profile->GetPrefs()->SetInteger(
        prefs::kLuxxleDefaultSearchVersion,
        is_new_profile
            ? TemplateURLPrepopulateData::kLuxxleCurrentDataVersion
            : TemplateURLPrepopulateData::kLuxxleFirstTrackedDataVersion);
  }
}

void SetDefaultThirdPartyCookieBlockValue(Profile* profile) {
  profile->GetPrefs()->SetDefaultPrefValue(
      prefs::kCookieControlsMode,
      base::Value(static_cast<int>(
          content_settings::CookieControlsMode::kBlockThirdParty)));
}

#if BUILDFLAG(ENABLE_LUXXLE_WEBTORRENT)
void SetWebTorrentEnabled(Profile* profile, bool is_new_profile) {
  const PrefService::Preference* pref_webtorrent_enabled =
      profile->GetPrefs()->FindPreference(kWebTorrentEnabled);
  if (!pref_webtorrent_enabled->HasUserSetting()) {
    profile->GetPrefs()->SetBoolean(
        kWebTorrentEnabled,
        is_new_profile ? false
                       : profile->GetPrefs()->GetBoolean(kWebTorrentEnabled));
  }
}
#endif

}  // namespace luxxle
