/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_VERSION_INFO_VERSION_INFO_IOS_H_
#define LUXXLE_IOS_BROWSER_API_VERSION_INFO_VERSION_INFO_IOS_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSInteger LuxxleCoreVersionInfoChannel
    NS_TYPED_ENUM NS_SWIFT_NAME(LuxxleCoreVersionInfo.Channel);

OBJC_EXPORT LuxxleCoreVersionInfoChannel const LuxxleCoreVersionInfoChannelStable;
OBJC_EXPORT LuxxleCoreVersionInfoChannel const LuxxleCoreVersionInfoChannelBeta;
OBJC_EXPORT LuxxleCoreVersionInfoChannel const
    LuxxleCoreVersionInfoChannelDevelopment;
OBJC_EXPORT LuxxleCoreVersionInfoChannel const
    LuxxleCoreVersionInfoChannelNightly;
OBJC_EXPORT LuxxleCoreVersionInfoChannel const
    LuxxleCoreVersionInfoChannelUnknown;

OBJC_EXPORT
@interface LuxxleCoreVersionInfo : NSObject
@property(class, readonly) NSString* luxxleCoreVersion;
@property(class, readonly) NSString* chromiumVersion;
@property(class, readonly) NSString* channelString;
@property(class, readonly) LuxxleCoreVersionInfoChannel channel;

- (instancetype)init NS_UNAVAILABLE;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_VERSION_INFO_VERSION_INFO_IOS_H_
