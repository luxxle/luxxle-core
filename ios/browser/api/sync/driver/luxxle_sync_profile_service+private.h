/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_SYNC_DRIVER_LUXXLE_SYNC_PROFILE_SERVICE_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_SYNC_DRIVER_LUXXLE_SYNC_PROFILE_SERVICE_PRIVATE_H_

#import <Foundation/Foundation.h>

#import "luxxle/ios/browser/api/sync/driver/luxxle_sync_profile_service.h"

NS_ASSUME_NONNULL_BEGIN

namespace syncer {
class SyncService;
}

@interface LuxxleSyncProfileServiceIOS (Private)
- (instancetype)initWithProfileSyncService:(syncer::SyncService*)syncService;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_SYNC_DRIVER_LUXXLE_SYNC_PROFILE_SERVICE_PRIVATE_H_
