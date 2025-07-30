/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/sync/luxxle_sync_alerts_service_factory.h"

#include <memory>

#include "base/no_destructor.h"
#include "luxxle/browser/sync/luxxle_sync_alerts_service.h"
#include "chrome/browser/profiles/incognito_helpers.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/sync/sync_service_factory.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/pref_registry/pref_registry_syncable.h"

// static
LuxxleSyncAlertsService* LuxxleSyncAlertsServiceFactory::GetForBrowserContext(
    content::BrowserContext* context) {
  return static_cast<LuxxleSyncAlertsService*>(
      GetInstance()->GetServiceForBrowserContext(context, true));
}

// static
LuxxleSyncAlertsServiceFactory* LuxxleSyncAlertsServiceFactory::GetInstance() {
  static base::NoDestructor<LuxxleSyncAlertsServiceFactory> instance;
  return instance.get();
}

LuxxleSyncAlertsServiceFactory::LuxxleSyncAlertsServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "LuxxleSyncAlertsService",
          BrowserContextDependencyManager::GetInstance()) {
  DependsOn(SyncServiceFactory::GetInstance());
}

LuxxleSyncAlertsServiceFactory::~LuxxleSyncAlertsServiceFactory() {}

std::unique_ptr<KeyedService>
LuxxleSyncAlertsServiceFactory::BuildServiceInstanceForBrowserContext(
    content::BrowserContext* context) const {
  return std::make_unique<LuxxleSyncAlertsService>(
      Profile::FromBrowserContext(context));
}

content::BrowserContext* LuxxleSyncAlertsServiceFactory::GetBrowserContextToUse(
    content::BrowserContext* context) const {
  return GetBrowserContextRedirectedInIncognito(context);
}

bool LuxxleSyncAlertsServiceFactory::ServiceIsCreatedWithBrowserContext() const {
  return true;
}

bool LuxxleSyncAlertsServiceFactory::ServiceIsNULLWhileTesting() const {
  return true;
}
