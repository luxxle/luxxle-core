/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_FINGERPRINT_H_
#define LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_FINGERPRINT_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, LuxxleFingerprintType);

OBJC_EXPORT
@interface LuxxleCertificateFingerprint : NSObject
@property(nonatomic, readonly) LuxxleFingerprintType type;
@property(nonatomic, readonly) NSString* fingerprintHexEncoded;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_FINGERPRINT_H_
