/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_rewards/rewards_web_ui_utils.h"

// REMOVED: #include "luxxle/browser/luxxle_rewards/.*"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/browser_context.h"
#include "url/gurl.h"

namespace luxxle_rewards {

bool ShouldBlockRewardsWebUI(content::BrowserContext* browser_context,
                             const GURL& url) {
  if (url.host_piece() != kRewardsPageHost &&
#if !BUILDFLAG(IS_ANDROID)
      url.host_piece() != kRewardsPageTopHost &&
#endif  // !BUILDFLAG(IS_ANDROID)
      url.host_piece() != kRewardsInternalsHost) {
    return false;
  }

  Profile* profile = Profile::FromBrowserContext(browser_context);
  if (profile) {
    if (!luxxle_rewards::IsSupportedForProfile(
            profile, url.host_piece() == kRewardsPageHost
                         ? luxxle_rewards::IsSupportedOptions::kSkipRegionCheck
                         : luxxle_rewards::IsSupportedOptions::kNone)) {
      return true;
    }
  }
  return false;
}

}  // namespace luxxle_rewards
