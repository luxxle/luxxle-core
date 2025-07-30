// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_API_LUXXLE_USER_AGENT_LUXXLE_USER_AGENT_EXCEPTIONS_IOS_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_LUXXLE_USER_AGENT_LUXXLE_USER_AGENT_EXCEPTIONS_IOS_PRIVATE_H_

#include "luxxle/components/luxxle_user_agent/browser/luxxle_user_agent_exceptions.h"
#include "luxxle/ios/browser/api/luxxle_user_agent/luxxle_user_agent_exceptions_ios.h"

@interface LuxxleUserAgentExceptionsIOS (Private)
- (instancetype)initWithLuxxleUserAgentExceptions:
    (luxxle_user_agent::LuxxleUserAgentExceptions*)luxxleUserAgentExceptions;
@end

#endif  // LUXXLE_IOS_BROWSER_API_LUXXLE_USER_AGENT_LUXXLE_USER_AGENT_EXCEPTIONS_IOS_PRIVATE_H_
