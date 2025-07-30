/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_OPENTABS_LUXXLE_OPENTABS_API_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_OPENTABS_LUXXLE_OPENTABS_API_PRIVATE_H_

#import <Foundation/Foundation.h>

#include "luxxle/ios/browser/api/opentabs/luxxle_opentabs_api.h"

NS_ASSUME_NONNULL_BEGIN

namespace syncer {
class SyncService;
}  // namespace syncer

namespace sync_sessions {
class SessionSyncService;
}  // namespace sync_sessions

@interface LuxxleOpenTabsAPI (Private)
- (instancetype)initWithSyncService:(syncer::SyncService*)syncService
                 sessionSyncService:
                     (sync_sessions::SessionSyncService*)sessionSyncService;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_OPENTABS_LUXXLE_OPENTABS_API_PRIVATE_H_
