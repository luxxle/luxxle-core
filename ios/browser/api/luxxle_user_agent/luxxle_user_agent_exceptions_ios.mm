// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/components/luxxle_user_agent/browser/luxxle_user_agent_exceptions.h"

#include "base/containers/contains.h"
#include "base/feature_list.h"
#include "base/memory/raw_ptr.h"
#include "luxxle/components/luxxle_user_agent/common/features.h"
#include "luxxle/ios/browser/api/luxxle_user_agent/luxxle_user_agent_exceptions_ios+private.h"
#include "luxxle/ios/browser/application_context/luxxle_application_context_impl.h"
#include "ios/chrome/browser/shared/model/application_context/application_context.h"
#include "net/base/apple/url_conversions.h"
#include "net/base/features.h"
#include "url/gurl.h"
#include "url/origin.h"

@interface LuxxleUserAgentExceptionsIOS () {
  raw_ptr<luxxle_user_agent::LuxxleUserAgentExceptions>
      _luxxleUserAgentExceptions;  // NOT OWNED
}

@end

@implementation LuxxleUserAgentExceptionsIOS
- (instancetype)initWithLuxxleUserAgentExceptions:
    (luxxle_user_agent::LuxxleUserAgentExceptions*)luxxleUserAgentExceptions {
  if ((self = [super init])) {
    _luxxleUserAgentExceptions = luxxleUserAgentExceptions;
  }
  return self;
}

- (bool)canShowLuxxle:(NSURL*)url {
  GURL gurl = net::GURLWithNSURL(url);
  if (!base::FeatureList::IsEnabled(
          luxxle_user_agent::features::kUseLuxxleUserAgent) ||
      !gurl.is_valid()) {
    return true;
  }

  return _luxxleUserAgentExceptions->CanShowLuxxle(gurl);
}
@end
