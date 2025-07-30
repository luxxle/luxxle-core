/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/permissions/permission_manager_factory.h"

#include "luxxle/browser/geolocation/luxxle_geolocation_permission_context_delegate.h"
#include "luxxle/browser/permissions/permission_lifetime_manager_factory.h"
#include "luxxle/components/permissions/luxxle_permission_manager.h"
#include "luxxle/components/permissions/contexts/luxxle_google_sign_in_permission_context.h"
#include "luxxle/components/permissions/contexts/luxxle_localhost_permission_context.h"
#include "luxxle/components/permissions/contexts/luxxle_open_ai_chat_permission_context.h"
#include "luxxle/components/permissions/contexts/luxxle_wallet_permission_context.h"
#include "luxxle/components/permissions/permission_lifetime_manager.h"
#include "components/permissions/features.h"

#define GeolocationPermissionContextDelegate \
  LuxxleGeolocationPermissionContextDelegate

#define BuildServiceInstanceForBrowserContext \
  BuildServiceInstanceForBrowserContext_ChromiumImpl

#include "src/chrome/browser/permissions/permission_manager_factory.cc"

#undef GeolocationPermissionContextDelegate
#undef BuildServiceInstanceForBrowserContext

std::unique_ptr<KeyedService>
PermissionManagerFactory::BuildServiceInstanceForBrowserContext(
    content::BrowserContext* context) const {
  Profile* profile = Profile::FromBrowserContext(context);
  auto permission_contexts = CreatePermissionContexts(profile);

  permission_contexts[ContentSettingsType::LUXXLE_ETHEREUM] =
      std::make_unique<permissions::LuxxleWalletPermissionContext>(
          profile, ContentSettingsType::LUXXLE_ETHEREUM);
  permission_contexts[ContentSettingsType::LUXXLE_SOLANA] =
      std::make_unique<permissions::LuxxleWalletPermissionContext>(
          profile, ContentSettingsType::LUXXLE_SOLANA);
  permission_contexts[ContentSettingsType::LUXXLE_CARDANO] =
      std::make_unique<permissions::LuxxleWalletPermissionContext>(
          profile, ContentSettingsType::LUXXLE_CARDANO);
  permission_contexts[ContentSettingsType::LUXXLE_GOOGLE_SIGN_IN] =
      std::make_unique<permissions::LuxxleGoogleSignInPermissionContext>(
          profile);
  permission_contexts[ContentSettingsType::LUXXLE_LOCALHOST_ACCESS] =
      std::make_unique<permissions::LuxxleLocalhostPermissionContext>(profile);
  permission_contexts[ContentSettingsType::LUXXLE_OPEN_AI_CHAT] =
      std::make_unique<permissions::LuxxleOpenAIChatPermissionContext>(profile);

  if (base::FeatureList::IsEnabled(
          permissions::features::kPermissionLifetime)) {
    auto factory =
        base::BindRepeating(&PermissionLifetimeManagerFactory::GetForProfile);
    for (auto& permission_context : permission_contexts) {
      permission_context.second->SetPermissionLifetimeManagerFactory(factory);
    }
  }

  return std::make_unique<permissions::LuxxlePermissionManager>(
      profile, std::move(permission_contexts));
}
