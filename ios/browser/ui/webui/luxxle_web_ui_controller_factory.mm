// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/ui/webui/luxxle_web_ui_controller_factory.h"

#include <memory>

#include "base/feature_list.h"
#include "base/memory/ptr_util.h"
#include "base/no_destructor.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/constants/url_constants.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "luxxle/ios/browser/ui/webui/ads/ads_internals_ui.h"
#include "luxxle/ios/browser/ui/webui/skus/skus_internals_ui.h"
#include "build/build_config.h"
#include "components/prefs/pref_service.h"
#include "ios/chrome/browser/shared/model/url/chrome_url_constants.h"
#include "ios/components/webui/web_ui_url_constants.h"
#include "url/gurl.h"

using web::WebUIIOS;
using web::WebUIIOSController;

namespace luxxle {

// A function for creating a new WebUIIOS.
using WebUIIOSFactoryFunction =
    std::unique_ptr<WebUIIOSController> (*)(WebUIIOS* web_ui, const GURL& url);

// Template for defining WebUIIOSFactoryFunction.
template <class T>
std::unique_ptr<WebUIIOSController> NewWebUIIOS(WebUIIOS* web_ui,
                                                const GURL& url) {
  return std::make_unique<T>(web_ui, url);
}

// Returns a function that can be used to create the right type of WebUIIOS for
// a tab, based on its URL. Returns nullptr if the URL doesn't have WebUIIOS
// associated with it.
WebUIIOSFactoryFunction GetWebUIIOSFactoryFunction(const GURL& url) {
  const char kChromeUIUntrustedScheme[] = "chrome-untrusted";

  // This will get called a lot to check all URLs, so do a quick check of other
  // schemes to filter out most URLs.
  if (!url.SchemeIs(kLuxxleUIScheme) && !url.SchemeIs(kChromeUIScheme) &&
      !url.SchemeIs(kChromeUIUntrustedScheme)) {
    return nullptr;
  }

  // TODO: Handle RewardsInternalUI, AdsInternalUI, AdblockInternalUI URLs here
  // ProfileIOS* browser_state =
  // ProfileIOS::FromWebUIIOS(web_ui);

  const std::string url_host = url.host();
  /*if (url_host == kAdblockInternalsHost) {
    return &NewWebUIIOS<LuxxleAdblockInternalsUI>;
  } if (url_host == kRewardsInternalsHost &&
             luxxle_rewards::IsSupportedForProfile(browser_state)) {
    return &NewWebUIIOS<LuxxleRewardsInternalsUI>;
  }*/

  if (url_host == kAdsInternalsHost) {
    return &NewWebUIIOS<AdsInternalsUI>;
  } else if (url_host == kSkusInternalsHost) {
    return &NewWebUIIOS<SkusInternalsUI>;
  }
  return nullptr;
}

}  // namespace luxxle

NSInteger LuxxleWebUIControllerFactory::GetErrorCodeForWebUIURL(
    const GURL& url) const {
  if (url.host() == kChromeUIDinoHost) {
    return NSURLErrorNotConnectedToInternet;
  }

  if (luxxle::GetWebUIIOSFactoryFunction(url)) {
    return 0;
  }

  return ChromeWebUIIOSControllerFactory::GetErrorCodeForWebUIURL(url);
}

std::unique_ptr<WebUIIOSController>
LuxxleWebUIControllerFactory::CreateWebUIIOSControllerForURL(
    WebUIIOS* web_ui,
    const GURL& url) const {
  luxxle::WebUIIOSFactoryFunction function =
      luxxle::GetWebUIIOSFactoryFunction(url);
  if (!function) {
    return ChromeWebUIIOSControllerFactory::CreateWebUIIOSControllerForURL(
        web_ui, url);
  }

  return (*function)(web_ui, url);
}

// static
LuxxleWebUIControllerFactory* LuxxleWebUIControllerFactory::GetInstance() {
  static base::NoDestructor<LuxxleWebUIControllerFactory> instance;
  return instance.get();
}

LuxxleWebUIControllerFactory::LuxxleWebUIControllerFactory() = default;

LuxxleWebUIControllerFactory::~LuxxleWebUIControllerFactory() = default;
