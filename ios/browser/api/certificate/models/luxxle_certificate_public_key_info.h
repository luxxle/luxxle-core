/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_PUBLIC_KEY_INFO_H_
#define LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_PUBLIC_KEY_INFO_H_

#import <Foundation/Foundation.h>
#include "luxxle_certificate_enums.h"  // NOLINT

NS_ASSUME_NONNULL_BEGIN

OBJC_EXPORT
@interface LuxxleCertificatePublicKeyInfo : NSObject
@property(nonatomic, readonly) LuxxlePublicKeyType type;
@property(nonatomic, readonly) NSString* algorithm;
@property(nonatomic, readonly) NSData* objectIdentifier;
@property(nonatomic, readonly) NSString* absoluteObjectIdentifier;
@property(nonatomic, readonly) NSString* curveName;
@property(nonatomic, readonly) NSString* nistCurveName;
@property(nonatomic, readonly) NSString* parameters;
@property(nonatomic, readonly) NSString* keyHexEncoded;
@property(nonatomic, readonly) NSUInteger keyBytesSize;
@property(nonatomic, readonly) NSUInteger effectiveSize;
@property(nonatomic, readonly) NSUInteger exponent;
@property(nonatomic, readonly) NSUInteger keySizeInBits;
@property(nonatomic, readonly) LuxxlePublicKeyUsage keyUsage;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_PUBLIC_KEY_INFO_H_
