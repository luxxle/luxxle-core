/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "luxxle/ios/browser/api/sync/luxxle_sync_api.h"

#import <CoreImage/CoreImage.h>

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/functional/bind.h"
#include "base/json/json_writer.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "luxxle/components/luxxle_sync/luxxle_sync_prefs.h"
#include "luxxle/components/luxxle_sync/crypto/crypto.h"
#include "luxxle/components/luxxle_sync/qr_code_validator.h"
#include "luxxle/components/luxxle_sync/time_limited_words.h"
#include "luxxle/components/sync_device_info/luxxle_device_info.h"
#include "luxxle/ios/browser/api/sync/luxxle_sync_worker.h"
#include "components/sync/engine/sync_protocol_error.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_service_impl.h"
#include "components/sync/service/sync_service_observer.h"
#include "components/sync_device_info/device_info.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"
#include "components/sync_device_info/local_device_info_provider.h"
#include "ios/chrome/browser/shared/model/profile/profile_ios.h"
#include "ios/chrome/browser/sync/model/device_info_sync_service_factory.h"
#include "ios/chrome/browser/sync/model/sync_service_factory.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

// MARK: - LuxxleSyncAPISyncProtocolErrorResult

LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultSuccess =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::SYNC_SUCCESS);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultNotMyBirthday =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::NOT_MY_BIRTHDAY);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultThrottled =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::THROTTLED);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultTransientError =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::TRANSIENT_ERROR);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultMigrationDone =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::MIGRATION_DONE);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultDisabledByAdmin = static_cast<NSInteger>(
        syncer::SyncProtocolErrorType::DISABLED_BY_ADMIN);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultPartialFailure =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::PARTIAL_FAILURE);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultDataObsolete = static_cast<NSInteger>(
        syncer::SyncProtocolErrorType::CLIENT_DATA_OBSOLETE);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultEncryptionObsolete =
        static_cast<NSInteger>(
            syncer::SyncProtocolErrorType::ENCRYPTION_OBSOLETE);
LuxxleSyncAPISyncProtocolErrorResult const
    LuxxleSyncAPISyncProtocolErrorResultUnknown =
        static_cast<NSInteger>(syncer::SyncProtocolErrorType::UNKNOWN_ERROR);

// MARK: - QrCodeDataValidationResult

LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultValid =
        static_cast<NSInteger>(luxxle_sync::QrCodeDataValidationResult::kValid);
LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultNotWellFormed =
        static_cast<NSInteger>(
            luxxle_sync::QrCodeDataValidationResult::kNotWellFormed);
LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultVersionDeprecated =
        static_cast<NSInteger>(
            luxxle_sync::QrCodeDataValidationResult::kVersionDeprecated);
LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultExpired = static_cast<NSInteger>(
        luxxle_sync::QrCodeDataValidationResult::kExpired);
LuxxleSyncAPIQrCodeDataValidationResult const
    LuxxleSyncAPIQrCodeDataValidationResultValidForTooLong =
        static_cast<NSInteger>(
            luxxle_sync::QrCodeDataValidationResult::kValidForTooLong);

// MARK: - TimeLimitedWords::ValidationStatus

LuxxleSyncAPIWordsValidationStatus const LuxxleSyncAPIWordsValidationStatusValid =
    static_cast<NSInteger>(
        luxxle_sync::TimeLimitedWords::ValidationStatus::kValid);
LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusNotValidPureWords = static_cast<NSInteger>(
        luxxle_sync::TimeLimitedWords::ValidationStatus::kNotValidPureWords);
LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusVersionDeprecated = static_cast<NSInteger>(
        luxxle_sync::TimeLimitedWords::ValidationStatus::kVersionDeprecated);
LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusExpired = static_cast<NSInteger>(
        luxxle_sync::TimeLimitedWords::ValidationStatus::kExpired);
LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusValidForTooLong = static_cast<NSInteger>(
        luxxle_sync::TimeLimitedWords::ValidationStatus::kValidForTooLong);
LuxxleSyncAPIWordsValidationStatus const
    LuxxleSyncAPIWordsValidationStatusWrongWordsNumber = static_cast<NSInteger>(
        luxxle_sync::TimeLimitedWords::ValidationStatus::kWrongWordsNumber);

// MARK: - LuxxleSyncDeviceObserver

@interface LuxxleSyncDeviceObserver : NSObject {
  std::unique_ptr<LuxxleSyncDeviceTracker> _device_observer;
}
@end

@implementation LuxxleSyncDeviceObserver

- (instancetype)initWithDeviceInfoTracker:(syncer::DeviceInfoTracker*)tracker
                                 callback:(void (^)())onDeviceInfoChanged {
  if ((self = [super init])) {
    _device_observer = std::make_unique<LuxxleSyncDeviceTracker>(
        tracker, base::BindRepeating(onDeviceInfoChanged));
  }
  return self;
}
@end

// MARK: - LuxxleSyncServiceObserver

@interface LuxxleSyncServiceObserver : NSObject {
  std::unique_ptr<LuxxleSyncServiceTracker> _service_tracker;
}
@end

@implementation LuxxleSyncServiceObserver

- (instancetype)initWithSyncServiceImpl:
                    (syncer::SyncServiceImpl*)syncServiceImpl
                   stateChangedCallback:(void (^)())onSyncServiceStateChanged
                   syncShutdownCallback:(void (^)())onSyncServiceShutdown {
  if ((self = [super init])) {
    _service_tracker = std::make_unique<LuxxleSyncServiceTracker>(
        syncServiceImpl, base::BindRepeating(onSyncServiceStateChanged),
        base::BindRepeating(onSyncServiceShutdown));
  }
  return self;
}
@end

// MARK: - LuxxleSyncAPI

@interface LuxxleSyncAPI () {
  std::unique_ptr<LuxxleSyncWorker> _worker;
  raw_ptr<ProfileIOS> _profile;
}
@end

@implementation LuxxleSyncAPI

- (instancetype)initWithBrowserState:(ProfileIOS*)mainBrowserState {
  if ((self = [super init])) {
    _profile = mainBrowserState;
    _worker.reset(new LuxxleSyncWorker(_profile));
  }
  return self;
}

- (void)dealloc {
  _worker.reset();
  _profile = nullptr;
}

- (bool)canSyncFeatureStart {
  return _worker->CanSyncFeatureStart();
}

- (bool)isSyncFeatureActive {
  return _worker->IsSyncFeatureActive();
}

- (bool)isInitialSyncFeatureSetupComplete {
  return _worker->IsInitialSyncFeatureSetupComplete();
}

- (void)setSetupComplete {
  _worker->SetSetupComplete();
}

- (void)requestSync {
  _worker->RequestSync();
}

- (bool)isValidSyncCode:(NSString*)syncCode {
  return _worker->IsValidSyncCode(base::SysNSStringToUTF8(syncCode));
}

- (NSString*)getSyncCode {
  std::string syncCode = _worker->GetOrCreateSyncCode();
  if (syncCode.empty()) {
    return nil;
  }

  return base::SysUTF8ToNSString(syncCode);
}

- (bool)setSyncCode:(NSString*)syncCode {
  return _worker->SetSyncCode(base::SysNSStringToUTF8(syncCode));
}

- (NSString*)syncCodeFromHexSeed:(NSString*)hexSeed {
  return base::SysUTF8ToNSString(
      _worker->GetSyncCodeFromHexSeed(base::SysNSStringToUTF8(hexSeed)));
}

- (NSString*)hexSeedFromSyncCode:(NSString*)syncCode {
  return base::SysUTF8ToNSString(
      _worker->GetHexSeedFromSyncCode(base::SysNSStringToUTF8(syncCode)));
}

- (NSString*)qrCodeJsonFromHexSeed:(NSString*)hexSeed {
  return base::SysUTF8ToNSString(
      _worker->GetQrCodeJsonFromHexSeed(base::SysNSStringToUTF8(hexSeed)));
}

- (LuxxleSyncAPIQrCodeDataValidationResult)getQRCodeValidationResult:
    (NSString*)json {
  return static_cast<LuxxleSyncAPIQrCodeDataValidationResult>(
      _worker->GetQrCodeValidationResult(base::SysNSStringToUTF8(json)));
}

- (LuxxleSyncAPIWordsValidationStatus)getWordsValidationResult:
    (NSString*)timeLimitedWords {
  return static_cast<LuxxleSyncAPIWordsValidationStatus>(
      _worker->GetWordsValidationResult(
          base::SysNSStringToUTF8(timeLimitedWords)));
}

- (NSString*)getWordsFromTimeLimitedWords:(NSString*)timeLimitedWords {
  return base::SysUTF8ToNSString(_worker->GetWordsFromTimeLimitedWords(
      base::SysNSStringToUTF8(timeLimitedWords)));
}

- (NSString*)getTimeLimitedWordsFromWords:(NSString*)words {
  return base::SysUTF8ToNSString(
      _worker->GetTimeLimitedWordsFromWords(base::SysNSStringToUTF8(words)));
}

- (NSDate*)getExpirationFromTimeLimitedWords:(NSString*)timeLimitedWords {
  base::Time not_after = luxxle_sync::TimeLimitedWords::GetNotAfter(
      base::SysNSStringToUTF8(timeLimitedWords));

  return not_after.ToNSDate();
}

- (NSString*)getHexSeedFromQrCodeJson:(NSString*)json {
  return base::SysUTF8ToNSString(
      _worker->GetHexSeedFromQrCodeJson(base::SysNSStringToUTF8(json)));
}

- (UIImage*)getQRCodeImage:(CGSize)size {
  std::vector<uint8_t> seed;
  std::string sync_code = _worker->GetOrCreateSyncCode();
  if (!luxxle_sync::crypto::PassphraseToBytes32(sync_code, &seed)) {
    return nil;
  }

  // QR code version 3 can only carry 84 bytes so we hex encode 32 bytes
  // seed then we will have 64 bytes input data
  const std::string sync_code_hex = base::HexEncode(seed.data(), seed.size());

  NSData* sync_code_data = [base::SysUTF8ToNSString(sync_code_hex.c_str())
      dataUsingEncoding:NSUTF8StringEncoding];  // NSISOLatin1StringEncoding

  if (!sync_code_data) {
    return nil;
  }

  CIFilter* filter = [CIFilter filterWithName:@"CIQRCodeGenerator"];
  [filter setValue:sync_code_data forKey:@"inputMessage"];
  [filter setValue:@"H" forKey:@"inputCorrectionLevel"];

  CIImage* ciImage = [filter outputImage];
  if (ciImage) {
    CGFloat scaleX = size.width / ciImage.extent.size.width;
    CGFloat scaleY = size.height / ciImage.extent.size.height;
    CGAffineTransform transform = CGAffineTransformMakeScale(scaleX, scaleY);
    ciImage = [ciImage imageByApplyingTransform:transform];

    return [UIImage imageWithCIImage:ciImage
                               scale:[[UIScreen mainScreen] scale]
                         orientation:UIImageOrientationUp];
  }
  return nil;
}

- (NSString*)getDeviceListJSON {
  auto device_list = _worker->GetDeviceList();
  auto* local_device_info = _worker->GetLocalDeviceInfo();

  base::Value::List device_list_value;

  for (const auto& device : device_list) {
    auto device_value = device->ToValue();
    bool is_current_device =
        local_device_info ? local_device_info->guid() == device->guid() : false;
    device_value.Set("isCurrentDevice", is_current_device);
    device_value.Set("guid", device->guid());
    device_value.Set("supportsSelfDelete", device->is_self_delete_supported());
    device_list_value.Append(base::Value(std::move(device_value)));
  }

  std::string json_string;
  if (!base::JSONWriter::Write(device_list_value, &json_string)) {
    return nil;
  }

  return base::SysUTF8ToNSString(json_string);
}

- (void)resetSync {
  _worker->ResetSync();
}

- (void)setDidJoinSyncChain:(void (^)(bool success))completion {
  _worker->SetJoinSyncChainCallback(base::BindOnce(completion));
}

- (void)permanentlyDeleteAccount:
    (void (^)(LuxxleSyncAPISyncProtocolErrorResult))completion {
  _worker->PermanentlyDeleteAccount(base::BindOnce(
      [](void (^completion)(LuxxleSyncAPISyncProtocolErrorResult),
         const syncer::SyncProtocolError& error) {
        completion(
            static_cast<LuxxleSyncAPISyncProtocolErrorResult>(error.error_type));
      },
      completion));
}

- (bool)isSyncAccountDeletedNoticePending {
  luxxle_sync::Prefs luxxle_sync_prefs(_profile->GetPrefs());
  return luxxle_sync_prefs.IsSyncAccountDeletedNoticePending();
}

- (void)setIsSyncAccountDeletedNoticePending:
    (bool)isSyncAccountDeletedNoticePending {
  luxxle_sync::Prefs luxxle_sync_prefs(_profile->GetPrefs());
  luxxle_sync_prefs.SetSyncAccountDeletedNoticePending(false);
}

- (bool)isFailedDecryptSeedNoticeDismissed {
  luxxle_sync::Prefs luxxle_sync_prefs(_profile->GetPrefs());
  return luxxle_sync_prefs.IsFailedDecryptSeedNoticeDismissed();
}

- (void)dismissFailedDecryptSeedNotice {
  luxxle_sync::Prefs luxxle_sync_prefs(_profile->GetPrefs());
  luxxle_sync_prefs.DismissFailedDecryptSeedNotice();
}

- (void)deleteDevice:(NSString*)guid {
  _worker->DeleteDevice(base::SysNSStringToUTF8(guid));
}

- (id)createSyncDeviceObserver:(void (^)())onDeviceInfoChanged {
  auto* tracker = DeviceInfoSyncServiceFactory::GetForProfile(_profile)
                      ->GetDeviceInfoTracker();
  return [[LuxxleSyncDeviceObserver alloc]
      initWithDeviceInfoTracker:tracker
                       callback:onDeviceInfoChanged];
}

- (id)createSyncServiceObserver:(void (^)())onSyncServiceStateChanged
          onSyncServiceShutdown:(void (^)())onSyncServiceShutdown {
  auto* service = static_cast<syncer::SyncServiceImpl*>(
      SyncServiceFactory::GetForProfile(_profile));
  return [[LuxxleSyncServiceObserver alloc]
      initWithSyncServiceImpl:service
         stateChangedCallback:onSyncServiceStateChanged
         syncShutdownCallback:onSyncServiceShutdown];
}
@end
