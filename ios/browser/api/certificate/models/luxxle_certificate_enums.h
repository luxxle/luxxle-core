/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_ENUMS_H_
#define LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_ENUMS_H_

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSUInteger, LuxxlePublicKeyUsage) {
  LuxxlePublicKeyUsage_INVALID = 1 << 0,
  LuxxlePublicKeyUsage_ENCRYPT = 1 << 1,
  LuxxlePublicKeyUsage_DECRYPT = 1 << 2,
  LuxxlePublicKeyUsage_SIGN = 1 << 3,
  LuxxlePublicKeyUsage_VERIFY = 1 << 4,
  LuxxlePublicKeyUsage_WRAP = 1 << 5,
  LuxxlePublicKeyUsage_DERIVE = 1 << 6,
  LuxxlePublicKeyUsage_ANY = 1 << 7
};

typedef NS_ENUM(NSUInteger, LuxxlePublicKeyType) {
  LuxxlePublicKeyType_UNKNOWN,
  LuxxlePublicKeyType_RSA,
  LuxxlePublicKeyType_DSA,
  LuxxlePublicKeyType_DH,
  LuxxlePublicKeyType_EC
};

typedef NS_ENUM(NSUInteger, LuxxleFingerprintType) {
  LuxxleFingerprintType_SHA1,
  LuxxleFingerprintType_SHA256
};

#endif  // LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_ENUMS_H_
