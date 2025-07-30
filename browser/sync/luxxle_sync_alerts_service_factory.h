/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_ALERTS_SERVICE_FACTORY_H_
#define LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_ALERTS_SERVICE_FACTORY_H_

#include <memory>

#include "components/keyed_service/content/browser_context_keyed_service_factory.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

class LuxxleSyncAlertsService;

class LuxxleSyncAlertsServiceFactory : public BrowserContextKeyedServiceFactory {
 public:
  LuxxleSyncAlertsServiceFactory(const LuxxleSyncAlertsServiceFactory&) = delete;
  LuxxleSyncAlertsServiceFactory& operator=(
      const LuxxleSyncAlertsServiceFactory&) = delete;

  static LuxxleSyncAlertsService* GetForBrowserContext(
      content::BrowserContext* context);

  static LuxxleSyncAlertsServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<LuxxleSyncAlertsServiceFactory>;

  LuxxleSyncAlertsServiceFactory();
  ~LuxxleSyncAlertsServiceFactory() override;

  // BrowserContextKeyedServiceFactory:
  std::unique_ptr<KeyedService> BuildServiceInstanceForBrowserContext(
      content::BrowserContext* context) const override;
  content::BrowserContext* GetBrowserContextToUse(
      content::BrowserContext* context) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;
  bool ServiceIsNULLWhileTesting() const override;
};

#endif  // LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_ALERTS_SERVICE_FACTORY_H_
