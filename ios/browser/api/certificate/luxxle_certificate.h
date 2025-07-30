/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_CERTIFICATE_LUXXLE_CERTIFICATE_H_
#define LUXXLE_IOS_BROWSER_API_CERTIFICATE_LUXXLE_CERTIFICATE_H_

#import <Foundation/Foundation.h>
#import <Security/Security.h>

NS_ASSUME_NONNULL_BEGIN

@class LuxxleCertificateRDNSequence;
@class LuxxleCertificateSignature;
@class LuxxleCertificatePublicKeyInfo;
@class LuxxleCertificateFingerprint;
// @class LuxxleCertificateExtensionModel;

OBJC_EXPORT
@interface LuxxleCertificateModel : NSObject
@property(nonatomic, readonly) bool isRootCertificate;
@property(nonatomic, readonly) bool isCertificateAuthority;
@property(nonatomic, readonly) bool isSelfSigned;
@property(nonatomic, readonly) bool isSelfIssued;
@property(nonatomic, readonly) LuxxleCertificateRDNSequence* subjectName;
@property(nonatomic, readonly) LuxxleCertificateRDNSequence* issuerName;
@property(nonatomic, readonly) NSString* serialNumber;
@property(nonatomic, readonly) NSUInteger version;
@property(nonatomic, readonly) LuxxleCertificateSignature* signature;
@property(nonatomic, readonly) NSDate* notValidBefore;
@property(nonatomic, readonly) NSDate* notValidAfter;
@property(nonatomic, readonly) LuxxleCertificatePublicKeyInfo* publicKeyInfo;
// @property(nonatomic, readonly)
// NSArray<LuxxleCertificateExtensionModel*>* extensions;
@property(nonatomic, readonly) LuxxleCertificateFingerprint* sha1Fingerprint;
@property(nonatomic, readonly) LuxxleCertificateFingerprint* sha256Fingerprint;

- (nullable instancetype)initWithCertificate:(SecCertificateRef)certificate;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_CERTIFICATE_LUXXLE_CERTIFICATE_H_
