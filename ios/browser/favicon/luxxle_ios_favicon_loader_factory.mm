/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "luxxle/ios/browser/favicon/luxxle_ios_favicon_loader_factory.h"

#include "base/no_destructor.h"
#import "luxxle/ios/browser/favicon/luxxle_ios_favicon_loader.h"
#include "components/keyed_service/core/service_access_type.h"
#include "components/keyed_service/ios/browser_state_keyed_service_factory.h"
#include "ios/chrome/browser/favicon/model/favicon_service_factory.h"
#import "ios/chrome/browser/favicon/model/ios_chrome_large_icon_service_factory.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

namespace {

std::unique_ptr<KeyedService> BuildFaviconLoader(web::BrowserState* context) {
  ProfileIOS* profile = ProfileIOS::FromBrowserState(context);
  return std::make_unique<luxxle_favicon::LuxxleFaviconLoader>(
      ios::FaviconServiceFactory::GetForProfile(
          profile, ServiceAccessType::EXPLICIT_ACCESS));
}

}  // namespace

namespace luxxle_favicon {
LuxxleFaviconLoader* LuxxleIOSFaviconLoaderFactory::GetForProfile(
    ProfileIOS* profile) {
  return GetInstance()->GetServiceForProfileAs<LuxxleFaviconLoader>(profile,
                                                                   true);
}

LuxxleFaviconLoader* LuxxleIOSFaviconLoaderFactory::GetForProfileIfExists(
    ProfileIOS* profile) {
  return GetInstance()->GetServiceForProfileAs<LuxxleFaviconLoader>(profile,
                                                                   false);
}

LuxxleIOSFaviconLoaderFactory* LuxxleIOSFaviconLoaderFactory::GetInstance() {
  static base::NoDestructor<LuxxleIOSFaviconLoaderFactory> instance;
  return instance.get();
}

// static
BrowserStateKeyedServiceFactory::TestingFactory
LuxxleIOSFaviconLoaderFactory::GetDefaultFactory() {
  return base::BindRepeating(&BuildFaviconLoader);
}

LuxxleIOSFaviconLoaderFactory::LuxxleIOSFaviconLoaderFactory()
    : ProfileKeyedServiceFactoryIOS("LuxxleFaviconLoader",
                                    ProfileSelection::kRedirectedInIncognito,
                                    ServiceCreation::kCreateLazily,
                                    TestingCreation::kNoServiceForTests) {
  DependsOn(ios::FaviconServiceFactory::GetInstance());
  DependsOn(IOSChromeLargeIconServiceFactory::GetInstance());
}

LuxxleIOSFaviconLoaderFactory::~LuxxleIOSFaviconLoaderFactory() {}

std::unique_ptr<KeyedService>
LuxxleIOSFaviconLoaderFactory::BuildServiceInstanceFor(
    web::BrowserState* context) const {
  return BuildFaviconLoader(context);
}
}  // namespace luxxle_favicon
