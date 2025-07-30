// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_API_LUXXLE_USER_AGENT_LUXXLE_USER_AGENT_EXCEPTIONS_IOS_H_
#define LUXXLE_IOS_BROWSER_API_LUXXLE_USER_AGENT_LUXXLE_USER_AGENT_EXCEPTIONS_IOS_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface LuxxleUserAgentExceptionsIOS : NSObject

- (instancetype)init NS_UNAVAILABLE;
/// This returns if a url can show Luxxle in User Agent
- (bool)canShowLuxxle:(NSURL*)url;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_LUXXLE_USER_AGENT_LUXXLE_USER_AGENT_EXCEPTIONS_IOS_H_
