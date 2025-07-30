/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_API_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_API_PRIVATE_H_

#import <Foundation/Foundation.h>

#include "luxxle/ios/browser/api/sync/luxxle_sync_api.h"

NS_ASSUME_NONNULL_BEGIN

class ProfileIOS;

@interface LuxxleSyncAPI (Private)
- (instancetype)initWithBrowserState:(ProfileIOS*)profile;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_API_PRIVATE_H_
