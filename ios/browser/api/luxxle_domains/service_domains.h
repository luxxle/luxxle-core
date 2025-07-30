// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_API_LUXXLE_DOMAINS_SERVICE_DOMAINS_H_
#define LUXXLE_IOS_BROWSER_API_LUXXLE_DOMAINS_SERVICE_DOMAINS_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSInteger LuxxleServicesEnvironmentIOS
    NS_TYPED_ENUM NS_SWIFT_NAME(LuxxleDomains.ServicesEnvironment);

OBJC_EXPORT LuxxleServicesEnvironmentIOS const
    LuxxleServicesEnvironmentIOSDevelopment;
OBJC_EXPORT LuxxleServicesEnvironmentIOS const
    LuxxleServicesEnvironmentIOSStaging;
OBJC_EXPORT LuxxleServicesEnvironmentIOS const
    LuxxleServicesEnvironmentIOSProduction;

OBJC_EXPORT
@interface LuxxleDomains : NSObject
@property(class, nonatomic, readonly) LuxxleServicesEnvironmentIOS environment;

+ (LuxxleServicesEnvironmentIOS)environmentWithPrefix:(NSString* _Nonnull)prefix
    NS_SWIFT_NAME(enviroment(prefix:));
+ (NSString*)serviceDomainWithPrefix:(NSString*)prefix
    NS_SWIFT_NAME(serviceDomain(prefix:));
+ (NSString*)serviceDomainWithPrefix:(NSString*)prefix
                         environment:(LuxxleServicesEnvironmentIOS)environment
    NS_SWIFT_NAME(serviceDomain(prefix:environment:));

- (instancetype)init NS_UNAVAILABLE;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_LUXXLE_DOMAINS_SERVICE_DOMAINS_H_
