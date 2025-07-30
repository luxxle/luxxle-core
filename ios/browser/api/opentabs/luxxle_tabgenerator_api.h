/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_OPENTABS_LUXXLE_TABGENERATOR_API_H_
#define LUXXLE_IOS_BROWSER_API_OPENTABS_LUXXLE_TABGENERATOR_API_H_

#import <Foundation/Foundation.h>

@class WebState;

NS_ASSUME_NONNULL_BEGIN

NS_SWIFT_NAME(LuxxleSyncTab)
OBJC_EXPORT
@interface LuxxleSyncTab : NSObject

@property(nonatomic, strong, readonly) WebState* webState;

- (instancetype)init NS_UNAVAILABLE;

/// Function setting Title for the created Sync Tab
/// @param title Title for the Tab represantation
- (void)setTitle:(NSString*)title;
/// Function setting URL for the created Sync Tab
/// @param url URL for the tab represantation
- (void)setURL:(NSURL*)url;
@end

NS_SWIFT_NAME(LuxxleTabGeneratorAPI)
OBJC_EXPORT
@interface LuxxleTabGeneratorAPI : NSObject

- (instancetype)init NS_UNAVAILABLE;

- (LuxxleSyncTab*)createLuxxleSyncTab:(bool)isOffTheRecord
    NS_SWIFT_NAME(createLuxxleSyncTab(isOffTheRecord:));
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_OPENTABS_LUXXLE_TABGENERATOR_API_H_
