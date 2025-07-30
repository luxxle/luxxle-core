/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_APP_LUXXLE_CORE_MAIN_H_
#define LUXXLE_IOS_APP_LUXXLE_CORE_MAIN_H_

#import <Foundation/Foundation.h>

#import "luxxle_core_switches.h"  // NOLINT

@class LuxxleP3AUtils;
@class AdblockService;
@class HTTPSUpgradeExceptionsService;
@class LuxxleUserAgentExceptionsIOS;
@class LuxxleProfileController;

NS_ASSUME_NONNULL_BEGIN

typedef int LuxxleCoreLogSeverity NS_TYPED_ENUM;
OBJC_EXPORT const LuxxleCoreLogSeverity LuxxleCoreLogSeverityFatal;
OBJC_EXPORT const LuxxleCoreLogSeverity LuxxleCoreLogSeverityError;
OBJC_EXPORT const LuxxleCoreLogSeverity LuxxleCoreLogSeverityWarning;
OBJC_EXPORT const LuxxleCoreLogSeverity LuxxleCoreLogSeverityInfo;
OBJC_EXPORT const LuxxleCoreLogSeverity LuxxleCoreLogSeverityVerbose;

typedef bool (^LuxxleCoreLogHandler)(LuxxleCoreLogSeverity severity,
                                    NSString* file,
                                    int line,
                                    size_t messageStart,
                                    NSString* formattedMessage);

OBJC_EXPORT
@interface LuxxleCoreMain : NSObject


@property(nonatomic, readonly)
    HTTPSUpgradeExceptionsService* httpsUpgradeExceptionsService;

@property(nonatomic, readonly, nullable)
    LuxxleUserAgentExceptionsIOS* luxxleUserAgentExceptions;

/// Sets the global log handler for Chromium & LuxxleCore logs.
///
/// When a custom log handler is set, it is the responsibility of the client
/// to handle fatal logs from CHECK (and DCHECK on debug builds) by checking
/// the `serverity` passed in.
+ (void)setLogHandler:(nullable LuxxleCoreLogHandler)logHandler;

- (instancetype)init;

- (instancetype)initWithAdditionalSwitches:
    (NSArray<LuxxleCoreSwitch*>*)additionalSwitches;

- (void)scheduleLowPriorityStartupTasks;

- (void)setUserAgent:(NSString*)userAgent;

@property(readonly) AdblockService* adblockService;

- (void)initializeP3AServiceForChannel:(NSString*)channel
                      installationDate:(NSDate*)installDate;

@property(readonly) LuxxleP3AUtils* p3aUtils;

@property(readonly, nullable) LuxxleProfileController* profileController;
- (void)loadDefaultProfile:(void (^)(LuxxleProfileController*))completionHandler;

/// Sets up bundle path overrides and initializes ICU from the LuxxleCore bundle
/// without setting up a LuxxleCoreMain instance.
///
/// Should only be called in unit tests
+ (bool)initializeICUForTesting;

+ (void)initializeResourceBundleForTesting;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_APP_LUXXLE_CORE_MAIN_H_
