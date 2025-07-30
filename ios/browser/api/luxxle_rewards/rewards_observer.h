/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_LUXXLE_REWARDS_REWARDS_OBSERVER_H_
#define LUXXLE_IOS_BROWSER_API_LUXXLE_REWARDS_REWARDS_OBSERVER_H_

#import <Foundation/Foundation.h>
#import "rewards.mojom.objc.h"

@class LuxxleRewardsAPI, RewardsNotification;

NS_ASSUME_NONNULL_BEGIN

/// A rewards observer can get notified when certain actions happen
///
/// Creating a RewardsObserver alone will not respond to any events. Set
/// each closure that you wish to watch based on the data being displayed on
/// screen
OBJC_EXPORT
NS_SWIFT_NAME(RewardsObserver)
@interface RewardsObserver : NSObject

@property(nonatomic, readonly, weak) LuxxleRewardsAPI* rewardsAPI;

- (instancetype)initWithRewardsAPI:(LuxxleRewardsAPI*)rewardsAPI;

/// Executed when the wallet is first initialized
@property(nonatomic, copy, nullable) void (^walletInitalized)
    (LuxxleRewardsResult result);

/// A publisher was fetched by its URL for a specific tab identified by tabId
@property(nonatomic, copy, nullable) void (^fetchedPanelPublisher)
    (LuxxleRewardsPublisherInfo* info, uint64_t tabId);

@property(nonatomic, copy, nullable) void (^publisherListUpdated)();

@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_LUXXLE_REWARDS_REWARDS_OBSERVER_H_
