/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_API_H_
#define LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_API_H_

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class LuxxleSyncInternalsController;

typedef NSInteger LuxxleSyncAPISyncProtocolErrorResult
    NS_TYPED_ENUM NS_SWIFT_NAME(LuxxleSyncAPI.SyncProtocolErrorResult);

OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultSuccess;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultNotMyBirthday;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultThrottled;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultTransientError;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultMigrationDone;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultDisabledByAdmin;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultPartialFailure;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultDataObsolete;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultEncryptionObsolete;
OBJC_EXPORT LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultUnknown;

typedef NSInteger LuxxleSyncAPIQrCodeDataValidationResult NS_TYPED_ENUM
    NS_SWIFT_NAME(LuxxleSyncAPI.QrCodeDataValidationResult);

OBJC_EXPORT LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultValid;
OBJC_EXPORT LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultNotWellFormed;
OBJC_EXPORT LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultVersionDeprecated;
OBJC_EXPORT LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultExpired;
OBJC_EXPORT LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultValidForTooLong;

typedef NSInteger LuxxleSyncAPIWordsValidationStatus NS_TYPED_ENUM
    NS_SWIFT_NAME(LuxxleSyncAPI.WordsValidationStatus);

OBJC_EXPORT LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusValid;
OBJC_EXPORT LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusNotValidPureWords;
OBJC_EXPORT LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusVersionDeprecated;
OBJC_EXPORT LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusExpired;
OBJC_EXPORT LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusValidForTooLong;
OBJC_EXPORT LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusWrongWordsNumber;

OBJC_EXPORT
@interface LuxxleSyncAPI : NSObject

@property(nonatomic, readonly) bool canSyncFeatureStart;
@property(nonatomic, readonly) bool isSyncFeatureActive;
@property(nonatomic, readonly) bool isInitialSyncFeatureSetupComplete;
@property(nonatomic) bool isSyncAccountDeletedNoticePending;
@property(nonatomic, readonly) bool isFailedDecryptSeedNoticeDismissed;

- (instancetype)init NS_UNAVAILABLE;

- (void)requestSync;

- (void)setSetupComplete;

- (void)resetSync;

- (void)setDidJoinSyncChain:(void (^)(bool))completion;

- (void)permanentlyDeleteAccount:
    (void (^)(LuxxleSyncAPISyncProtocolErrorResult))completion;

- (void)deleteDevice:(NSString*)guid;

- (bool)isValidSyncCode:(NSString*)syncCode;

- (NSString*)getSyncCode;

// returns false is sync is already configured or if the sync code is invalid
- (bool)setSyncCode:(NSString*)syncCode;

- (NSString*)syncCodeFromHexSeed:(NSString*)hexSeed;

- (NSString*)hexSeedFromSyncCode:(NSString*)syncCode;

- (NSString*)qrCodeJsonFromHexSeed:(NSString*)hexSeed;

- (void)dismissFailedDecryptSeedNotice;

- (LuxxleSyncAPIQrCodeDataValidationResult)getQRCodeValidationResult:
    (NSString*)json;

- (LuxxleSyncAPIWordsValidationStatus)getWordsValidationResult:
    (NSString*)timeLimitedWords;

- (NSString*)getWordsFromTimeLimitedWords:(NSString*)timeLimitedWords;

- (NSString*)getTimeLimitedWordsFromWords:(NSString*)words;

- (NSDate*)getExpirationFromTimeLimitedWords:(NSString*)timeLimitedWords;

- (NSString*)getHexSeedFromQrCodeJson:(NSString*)json;

- (nullable UIImage*)getQRCodeImage:(CGSize)size;

- (nullable NSString*)getDeviceListJSON;

- (id)createSyncDeviceObserver:(void (^)())onDeviceInfoChanged;
- (id)createSyncServiceObserver:(void (^)())onSyncServiceStateChanged
          onSyncServiceShutdown:(void (^)())onSyncServiceShutdown;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_API_H_
