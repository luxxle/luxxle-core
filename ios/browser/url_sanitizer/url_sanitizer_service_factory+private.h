// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_URL_SANITIZER_URL_SANITIZER_SERVICE_FACTORY_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_URL_SANITIZER_URL_SANITIZER_SERVICE_FACTORY_PRIVATE_H_

#include <memory>

#include "brave/ios/browser/keyed_service/keyed_service_factory_wrapper.h"
#include "ios/chrome/browser/shared/model/profile/profile_keyed_service_factory_ios.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace web {
class BrowserState;
}  // namespace web

class KeyedService;
class ProfileIOS;

namespace luxxle {
class URLSanitizerService;

class URLSanitizerServiceFactory : public ProfileKeyedServiceFactoryIOS {
 public:
  static luxxle::URLSanitizerService* GetServiceForState(ProfileIOS* profile);

  static URLSanitizerServiceFactory* GetInstance();

 private:
  friend base::NoDestructor<URLSanitizerServiceFactory>;

  URLSanitizerServiceFactory();
  ~URLSanitizerServiceFactory() override;

  // ProfileKeyedServiceFactoryIOS implementation.
  std::unique_ptr<KeyedService> BuildServiceInstanceFor(
      web::BrowserState* context) const override;
};

}  // namespace brave

#endif  // LUXXLE_IOS_BROWSER_URL_SANITIZER_URL_SANITIZER_SERVICE_FACTORY_PRIVATE_H_
