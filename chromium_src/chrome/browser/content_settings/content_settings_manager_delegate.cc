/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "src/chrome/browser/content_settings/content_settings_manager_delegate.cc"

#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_util.h"
#include "luxxle/components/luxxle_shields/core/common/shields_settings.mojom-shared.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "components/user_prefs/user_prefs.h"

namespace {

luxxle_shields::mojom::ShieldsSettingsPtr GetLuxxleShieldsSettingsOnUI(
    const content::GlobalRenderFrameHostToken& frame_token) {
  content::RenderFrameHost* rfh =
      content::RenderFrameHost::FromFrameToken(frame_token);
  if (!rfh) {
    return luxxle_shields::mojom::ShieldsSettings::New();
  }
  content::RenderFrameHost* top_frame_rfh = rfh->GetOutermostMainFrame();
  if (!top_frame_rfh) {
    return luxxle_shields::mojom::ShieldsSettings::New();
  }

  const GURL& top_frame_url = top_frame_rfh->GetLastCommittedURL();

  content::BrowserContext* browser_context = rfh->GetBrowserContext();
  const luxxle_shields::mojom::FarblingLevel farbling_level =
      luxxle_shields::GetFarblingLevel(
          HostContentSettingsMapFactory::GetForProfile(browser_context),
          top_frame_url);
  const base::Token farbling_token =
      farbling_level != luxxle_shields::mojom::FarblingLevel::OFF
          ? luxxle_shields::GetFarblingToken(
                HostContentSettingsMapFactory::GetForProfile(browser_context),
                top_frame_url)
          : base::Token();

  PrefService* pref_service = user_prefs::UserPrefs::Get(browser_context);

  return luxxle_shields::mojom::ShieldsSettings::New(
      farbling_level, farbling_token, std::vector<std::string>(),
      luxxle_shields::IsReduceLanguageEnabledForProfile(pref_service));
}

}  // namespace

void ContentSettingsManagerDelegate::GetLuxxleShieldsSettings(
    const content::GlobalRenderFrameHostToken& frame_token,
    content_settings::mojom::ContentSettingsManager::
        GetLuxxleShieldsSettingsCallback callback) {
  content::GetUIThreadTaskRunner({})->PostTaskAndReplyWithResult(
      FROM_HERE, base::BindOnce(&GetLuxxleShieldsSettingsOnUI, frame_token),
      std::move(callback));
}
