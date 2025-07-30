/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/sync/luxxle_sync_worker.h"

#include <string>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "base/json/json_writer.h"
#include "base/memory/weak_ptr.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/sys_string_conversions.h"
#include "luxxle/components/luxxle_sync/luxxle_sync_prefs.h"
#include "luxxle/components/luxxle_sync/crypto/crypto.h"
#include "luxxle/components/luxxle_sync/qr_code_data.h"
#include "luxxle/components/luxxle_sync/qr_code_validator.h"
#include "luxxle/components/luxxle_sync/sync_service_impl_helper.h"
#include "luxxle/components/luxxle_sync/time_limited_words.h"
#include "luxxle/components/sync/service/luxxle_sync_service_impl.h"
#include "luxxle/components/sync_device_info/luxxle_device_info.h"
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
#include "ios/web/public/thread/web_thread.h"

namespace {
static const size_t SEED_BYTES_COUNT = 32u;
}  // namespace

LuxxleSyncDeviceTracker::LuxxleSyncDeviceTracker(
    syncer::DeviceInfoTracker* device_info_tracker,
    const base::RepeatingCallback<void()>& on_device_info_changed_callback)
    : on_device_info_changed_callback_(on_device_info_changed_callback) {
  DCHECK(device_info_tracker);
  device_info_tracker_observer_.Observe(device_info_tracker);
}

LuxxleSyncDeviceTracker::~LuxxleSyncDeviceTracker() {
  // Observer will be removed by ScopedObservation
}

void LuxxleSyncDeviceTracker::OnDeviceInfoChange() {
  if (on_device_info_changed_callback_) {
    on_device_info_changed_callback_.Run();
  }
}

LuxxleSyncServiceTracker::LuxxleSyncServiceTracker(
    syncer::SyncServiceImpl* sync_service_impl,
    const base::RepeatingCallback<void()>& on_state_changed_callback,
    const base::RepeatingCallback<void()>& on_sync_shutdown_callback)
    : on_state_changed_callback_(on_state_changed_callback),
      on_sync_shutdown_callback_(on_sync_shutdown_callback) {
  DCHECK(sync_service_impl);
  sync_service_observer_.Observe(sync_service_impl);
}

LuxxleSyncServiceTracker::~LuxxleSyncServiceTracker() {
  // Observer will be removed by ScopedObservation
}

void LuxxleSyncServiceTracker::OnStateChanged(syncer::SyncService* sync) {
  if (on_state_changed_callback_) {
    on_state_changed_callback_.Run();
  }
}

void LuxxleSyncServiceTracker::OnSyncShutdown(syncer::SyncService* sync) {
  if (on_sync_shutdown_callback_) {
    on_sync_shutdown_callback_.Run();
  }
}

LuxxleSyncWorker::LuxxleSyncWorker(ProfileIOS* profile) : profile_(profile) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
}

LuxxleSyncWorker::~LuxxleSyncWorker() {
  // Observer will be removed by ScopedObservation
}

bool LuxxleSyncWorker::RequestSync() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::SyncService* sync_service = GetSyncService();

  if (!sync_service) {
    return false;
  }

  if (!sync_service_observer_.IsObservingSource(sync_service)) {
    sync_service_observer_.AddObservation(sync_service);
  }

  sync_service->SetSyncFeatureRequested();

  return true;
}

const syncer::DeviceInfo* LuxxleSyncWorker::GetLocalDeviceInfo() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);

  if (!device_info_service) {
    return nullptr;
  }

  return device_info_service->GetLocalDeviceInfoProvider()
      ->GetLocalDeviceInfo();
}

std::vector<std::unique_ptr<syncer::LuxxleDeviceInfo>>
LuxxleSyncWorker::GetDeviceList() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);

  if (!device_info_service) {
    return std::vector<std::unique_ptr<syncer::LuxxleDeviceInfo>>();
  }

  syncer::DeviceInfoTracker* tracker =
      device_info_service->GetDeviceInfoTracker();
  return tracker->GetAllLuxxleDeviceInfo();
}

std::string LuxxleSyncWorker::GetOrCreateSyncCode() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::LuxxleSyncServiceImpl* sync_service = GetSyncService();
  std::string sync_code;
  if (sync_service) {
    sync_code = sync_service->GetOrCreateSyncCode();
  }

  CHECK(luxxle_sync::crypto::IsPassphraseValid(sync_code));
  return sync_code;
}

bool LuxxleSyncWorker::IsValidSyncCode(const std::string& sync_code) {
  std::vector<uint8_t> seed;
  if (!luxxle_sync::crypto::PassphraseToBytes32(sync_code, &seed)) {
    return false;
  }
  return seed.size() == SEED_BYTES_COUNT;
}

bool LuxxleSyncWorker::SetSyncCode(const std::string& sync_code) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  if (sync_code.empty()) {
    return false;
  }

  syncer::LuxxleSyncServiceImpl* sync_service = GetSyncService();
  if (!sync_service || !sync_service->SetSyncCode(sync_code)) {
    const std::string error_msg = sync_service
                                      ? "invalid sync code:" + sync_code
                                      : "sync service is not available";
    LOG(ERROR) << error_msg;
    return false;
  }

  passphrase_ = sync_code;
  return true;
}

std::string LuxxleSyncWorker::GetSyncCodeFromHexSeed(
    const std::string& hex_code_seed) {
  DCHECK(!hex_code_seed.empty());

  std::vector<uint8_t> bytes;
  std::string sync_code_words;
  if (base::HexStringToBytes(hex_code_seed, &bytes)) {
    DCHECK_EQ(bytes.size(), SEED_BYTES_COUNT);
    if (bytes.size() == SEED_BYTES_COUNT) {
      sync_code_words = luxxle_sync::crypto::PassphraseFromBytes32(bytes);
      if (sync_code_words.empty()) {
        VLOG(1) << __func__ << " PassphraseFromBytes32 failed for "
                << hex_code_seed;
      }
    } else {
      LOG(ERROR) << "wrong seed bytes " << bytes.size();
    }

    DCHECK_NE(sync_code_words, "");
  } else {
    VLOG(1) << __func__ << " HexStringToBytes failed for hex_code_seed";
  }
  return sync_code_words;
}

std::string LuxxleSyncWorker::GetHexSeedFromSyncCode(
    const std::string& code_words) {
  DCHECK(!code_words.empty());

  std::string sync_code_hex;
  std::vector<uint8_t> bytes;
  if (luxxle_sync::crypto::PassphraseToBytes32(code_words, &bytes)) {
    DCHECK_EQ(bytes.size(), SEED_BYTES_COUNT);
    if (bytes.size() == SEED_BYTES_COUNT) {
      sync_code_hex = base::HexEncode(&bytes.at(0), bytes.size());
    } else {
      LOG(ERROR) << "wrong seed bytes " << bytes.size();
    }
  } else {
    VLOG(1) << __func__ << " PassphraseToBytes32 failed for " << code_words;
  }
  return sync_code_hex;
}

std::string LuxxleSyncWorker::GetQrCodeJsonFromHexSeed(
    const std::string& hex_seed) {
  DCHECK(!hex_seed.empty());
  return luxxle_sync::QrCodeData::CreateWithActualDate(hex_seed)->ToJson();
}

luxxle_sync::QrCodeDataValidationResult
LuxxleSyncWorker::GetQrCodeValidationResult(const std::string& json) {
  DCHECK(!json.empty());
  return luxxle_sync::QrCodeDataValidator::ValidateQrDataJson(json);
}

luxxle_sync::TimeLimitedWords::ValidationStatus
LuxxleSyncWorker::GetWordsValidationResult(
    const std::string& time_limited_words) {
  DCHECK(!time_limited_words.empty());
  auto words_with_status =
      luxxle_sync::TimeLimitedWords::Parse(time_limited_words);
  if (words_with_status.has_value()) {
    return luxxle_sync::TimeLimitedWords::ValidationStatus::kValid;
  } else {
    return words_with_status.error();
  }
}

std::string LuxxleSyncWorker::GetWordsFromTimeLimitedWords(
    const std::string& time_limited_words) {
  DCHECK(!time_limited_words.empty());
  auto words_with_status =
      luxxle_sync::TimeLimitedWords::Parse(time_limited_words);
  DCHECK(words_with_status.has_value());
  return words_with_status.value();
}

std::string LuxxleSyncWorker::GetTimeLimitedWordsFromWords(
    const std::string& words) {
  DCHECK(!words.empty());
  auto generate_result = luxxle_sync::TimeLimitedWords::GenerateForNow(words);
  CHECK(generate_result.has_value());
  return generate_result.value();
}

std::string LuxxleSyncWorker::GetHexSeedFromQrCodeJson(const std::string& json) {
  DCHECK(!json.empty());
  std::unique_ptr<luxxle_sync::QrCodeData> qr_data =
      luxxle_sync::QrCodeData::FromJson(json);
  if (qr_data) {
    DCHECK(!GetSyncCodeFromHexSeed(qr_data->sync_code_hex).empty());
    return qr_data->sync_code_hex;
  }

  DCHECK(!GetSyncCodeFromHexSeed(json).empty());
  return json;
}

bool LuxxleSyncWorker::IsInitialSyncFeatureSetupComplete() {
  syncer::SyncService* sync_service = GetSyncService();
  return sync_service &&
         sync_service->GetUserSettings()->IsInitialSyncFeatureSetupComplete();
}

bool LuxxleSyncWorker::SetSetupComplete() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::SyncService* sync_service = GetSyncService();

  if (!sync_service) {
    return false;
  }

  sync_service->SetSyncFeatureRequested();

  if (!sync_service->GetUserSettings()->IsInitialSyncFeatureSetupComplete()) {
    sync_service->GetUserSettings()->SetInitialSyncFeatureSetupComplete(
        syncer::SyncFirstSetupCompleteSource::ADVANCED_FLOW_CONFIRM);
  }

  return true;
}

void LuxxleSyncWorker::ResetSync() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::LuxxleSyncServiceImpl* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  sync_service->prefs().AddLeaveChainDetail(__FILE__, __LINE__, __func__);

  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  DCHECK(device_info_service);

  luxxle_sync::ResetSync(sync_service, device_info_service,
                        base::BindOnce(&LuxxleSyncWorker::OnResetDone,
                                        weak_ptr_factory_.GetWeakPtr()));
}

void LuxxleSyncWorker::DeleteDevice(const std::string& device_guid) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::LuxxleSyncServiceImpl* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  auto* device_info_service =
      DeviceInfoSyncServiceFactory::GetForProfile(profile_);
  DCHECK(device_info_service);

  luxxle_sync::DeleteDevice(sync_service, device_info_service, device_guid);
}

void LuxxleSyncWorker::SetJoinSyncChainCallback(
    base::OnceCallback<void(bool)> callback) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::LuxxleSyncServiceImpl* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  sync_service->SetJoinChainResultCallback(std::move(callback));
}

void LuxxleSyncWorker::PermanentlyDeleteAccount(
    base::OnceCallback<void(const syncer::SyncProtocolError&)> callback) {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::LuxxleSyncServiceImpl* sync_service = GetSyncService();

  if (!sync_service) {
    return;
  }

  sync_service->prefs().AddLeaveChainDetail(__FILE__, __LINE__, __func__);

  sync_service->PermanentlyDeleteAccount(std::move(callback));
}

syncer::LuxxleSyncServiceImpl* LuxxleSyncWorker::GetSyncService() const {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  return static_cast<syncer::LuxxleSyncServiceImpl*>(
      SyncServiceFactory::GetForProfile(profile_));
}

void LuxxleSyncWorker::SetEncryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());

  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(!sync_user_settings->IsPassphraseRequired());

  if (sync_user_settings->IsCustomPassphraseAllowed() &&
      !sync_user_settings->IsUsingExplicitPassphrase() &&
      !sync_user_settings->IsTrustedVaultKeyRequired()) {
    sync_user_settings->SetEncryptionPassphrase(this->passphrase_);

    VLOG(3) << "[LuxxleSync] " << __func__ << " SYNC_CREATED_NEW_PASSPHRASE";
  }
}

void LuxxleSyncWorker::SetDecryptionPassphrase(syncer::SyncService* service) {
  DCHECK(service);
  DCHECK(service->IsEngineInitialized());
  DCHECK(!this->passphrase_.empty());

  syncer::SyncUserSettings* sync_user_settings = service->GetUserSettings();
  DCHECK(sync_user_settings->IsPassphraseRequired());

  if (sync_user_settings->SetDecryptionPassphrase(this->passphrase_)) {
    VLOG(3) << "[LuxxleSync] " << __func__
            << " SYNC_ENTERED_EXISTING_PASSPHRASE";
  }
}

void LuxxleSyncWorker::OnStateChanged(syncer::SyncService* service) {
  // If the sync engine has shutdown for some reason, just give up
  if (!service || !service->IsEngineInitialized()) {
    VLOG(3) << "[LuxxleSync] " << __func__ << " sync engine is not initialized";
    return;
  }

  if (this->passphrase_.empty()) {
    VLOG(3) << "[LuxxleSync] " << __func__ << " empty passphrase";
    return;
  }

  if (service->GetUserSettings()->IsPassphraseRequired()) {
    SetDecryptionPassphrase(service);
  } else {
    SetEncryptionPassphrase(service);
  }
}

void LuxxleSyncWorker::OnSyncShutdown(syncer::SyncService* service) {
  if (sync_service_observer_.IsObservingSource(service)) {
    sync_service_observer_.RemoveObservation(service);
  }
}

void LuxxleSyncWorker::OnResetDone() {
  syncer::SyncService* sync_service = GetSyncService();
  if (sync_service && sync_service_observer_.IsObservingSource(sync_service)) {
    sync_service_observer_.RemoveObservation(sync_service);
  }
}

bool LuxxleSyncWorker::CanSyncFeatureStart() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::SyncService* sync_service = GetSyncService();

  if (!sync_service) {
    return false;
  }

  return sync_service->IsSyncFeatureEnabled();
}

bool LuxxleSyncWorker::IsSyncFeatureActive() {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::SyncService* sync_service = GetSyncService();

  if (!sync_service) {
    return false;
  }

  return sync_service->IsSyncFeatureActive();
}
