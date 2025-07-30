/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_MODEL_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_MODEL_PRIVATE_H_

#import <Foundation/Foundation.h>
#import <Security/Security.h>

#include "luxxle/ios/browser/api/certificate/models/luxxle_certificate_enums.h"
#include "luxxle/ios/browser/api/certificate/models/luxxle_certificate_fingerprint.h"
#include "luxxle/ios/browser/api/certificate/models/luxxle_certificate_public_key_info.h"
#include "luxxle/ios/browser/api/certificate/models/luxxle_certificate_rdns_sequence.h"
#include "luxxle/ios/browser/api/certificate/models/luxxle_certificate_signature.h"

namespace bssl {
class ParsedCertificate;

namespace der {
class BitString;
class Input;
}  // namespace der
}  // namespace bssl

typedef NS_ENUM(NSUInteger, LuxxleFingerprintType);

NS_ASSUME_NONNULL_BEGIN

@interface LuxxleCertificateSignature ()
- (instancetype)initWithCertificate:(const bssl::ParsedCertificate*)certificate;
@end

@interface LuxxleCertificatePublicKeyInfo ()
- (instancetype)initWithCertificate:(const bssl::ParsedCertificate*)certificate
                            withKey:(SecKeyRef)key;
@end

@interface LuxxleCertificateFingerprint ()
- (instancetype)initWithCertificate:(CFDataRef)cert_data
                           withType:(LuxxleFingerprintType)type;
@end

@interface LuxxleCertificateRDNSequence ()
- (instancetype)initWithBERName:(const bssl::der::Input&)berName
                       uniqueId:(const bssl::der::BitString&)uniqueId;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_CERTIFICATE_MODELS_LUXXLE_CERTIFICATE_MODEL_PRIVATE_H_
