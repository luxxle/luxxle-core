/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/profiles/luxxle_renderer_updater_factory.h"

#include "base/no_destructor.h"
// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
#include "luxxle/browser/profiles/luxxle_renderer_updater.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"

LuxxleRendererUpdaterFactory::LuxxleRendererUpdaterFactory()
    : ProfileKeyedServiceFactory(
          "LuxxleRendererUpdater",
          ProfileSelections::Builder()
              .WithRegular(ProfileSelection::kOwnInstance)
              .WithGuest(ProfileSelection::kOwnInstance)
              .Build()) {
  DependsOn(luxxle_wallet::LuxxleWalletServiceFactory::GetInstance());
}

LuxxleRendererUpdaterFactory::~LuxxleRendererUpdaterFactory() = default;

// static
LuxxleRendererUpdaterFactory* LuxxleRendererUpdaterFactory::GetInstance() {
  static base::NoDestructor<LuxxleRendererUpdaterFactory> instance;
  return instance.get();
}

// static
LuxxleRendererUpdater* LuxxleRendererUpdaterFactory::GetForProfile(
    Profile* profile) {
  return static_cast<LuxxleRendererUpdater*>(
      GetInstance()->GetServiceForBrowserContext(profile, true));
}

std::unique_ptr<KeyedService>
LuxxleRendererUpdaterFactory::BuildServiceInstanceForBrowserContext(
    content::BrowserContext* context) const {
  auto* luxxle_wallet_service =
      luxxle_wallet::LuxxleWalletServiceFactory::GetServiceForContext(context);

  auto* keyring_service =
      luxxle_wallet_service ? luxxle_wallet_service->keyring_service() : 0;
  return std::make_unique<LuxxleRendererUpdater>(
      static_cast<Profile*>(context), keyring_service,
      g_browser_process->local_state());
}

bool LuxxleRendererUpdaterFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}
