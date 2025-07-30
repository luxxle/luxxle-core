/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_PROFILES_LUXXLE_RENDERER_UPDATER_FACTORY_H_
#define LUXXLE_BROWSER_PROFILES_LUXXLE_RENDERER_UPDATER_FACTORY_H_

#include <memory>

#include "base/no_destructor.h"
#include "chrome/browser/profiles/profile_keyed_service_factory.h"

class Profile;
class LuxxleRendererUpdater;

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

// Singleton that creates/deletes LuxxleRendererUpdater as new Profiles are
// created/shutdown.
class LuxxleRendererUpdaterFactory : public ProfileKeyedServiceFactory {
 public:
  // Returns an instance of the LuxxleRendererUpdaterFactory singleton.
  static LuxxleRendererUpdaterFactory* GetInstance();

  // Returns the instance of RendererUpdater for the passed |profile|.
  static LuxxleRendererUpdater* GetForProfile(Profile* profile);

  LuxxleRendererUpdaterFactory(const LuxxleRendererUpdaterFactory&) = delete;
  LuxxleRendererUpdaterFactory& operator=(const LuxxleRendererUpdaterFactory&) =
      delete;

 protected:
  std::unique_ptr<KeyedService> BuildServiceInstanceForBrowserContext(
      content::BrowserContext* profile) const override;
  bool ServiceIsCreatedWithBrowserContext() const override;

 private:
  friend base::NoDestructor<LuxxleRendererUpdaterFactory>;

  LuxxleRendererUpdaterFactory();
  ~LuxxleRendererUpdaterFactory() override;
};

#endif  // LUXXLE_BROWSER_PROFILES_LUXXLE_RENDERER_UPDATER_FACTORY_H_
