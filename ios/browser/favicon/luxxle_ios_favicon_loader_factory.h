/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_FAVICON_LUXXLE_IOS_FAVICON_LOADER_FACTORY_H_
#define LUXXLE_IOS_BROWSER_FAVICON_LUXXLE_IOS_FAVICON_LOADER_FACTORY_H_

#include <memory>

#include "base/no_destructor.h"
#include "ios/chrome/browser/shared/model/profile/profile_keyed_service_factory_ios.h"

class ProfileIOS;

namespace luxxle_favicon {
class LuxxleFaviconLoader;

// Singleton that owns all FaviconLoaders and associates them with
// ProfileIOS.
class LuxxleIOSFaviconLoaderFactory : public ProfileKeyedServiceFactoryIOS {
 public:
  static LuxxleFaviconLoader* GetForProfile(ProfileIOS* profile);
  static LuxxleFaviconLoader* GetForProfileIfExists(ProfileIOS* profile);
  static LuxxleIOSFaviconLoaderFactory* GetInstance();
  static TestingFactory GetDefaultFactory();

  LuxxleIOSFaviconLoaderFactory(const LuxxleIOSFaviconLoaderFactory&) = delete;
  LuxxleIOSFaviconLoaderFactory& operator=(const LuxxleIOSFaviconLoaderFactory&) =
      delete;

 private:
  friend class base::NoDestructor<LuxxleIOSFaviconLoaderFactory>;

  LuxxleIOSFaviconLoaderFactory();
  ~LuxxleIOSFaviconLoaderFactory() override;

  // ProfileKeyedServiceFactoryIOS implementation.
  std::unique_ptr<KeyedService> BuildServiceInstanceFor(
      web::BrowserState* context) const override;
};
}  // namespace luxxle_favicon

#endif  // LUXXLE_IOS_BROWSER_FAVICON_LUXXLE_IOS_FAVICON_LOADER_FACTORY_H_
