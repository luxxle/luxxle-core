/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_WORKER_H_
#define LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_WORKER_H_

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "base/scoped_multi_source_observation.h"
#include "base/scoped_observation.h"
#include "luxxle/components/luxxle_sync/time_limited_words.h"
#include "components/sync/engine/sync_protocol_error.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_service_observer.h"
#include "components/sync_device_info/device_info_sync_service.h"
#include "components/sync_device_info/device_info_tracker.h"

class ProfileIOS;
struct SyncProtocolError;

namespace syncer {
class LuxxleSyncServiceImpl;
class DeviceInfo;
class LuxxleDeviceInfo;
class SyncServiceImpl;
}  // namespace syncer

namespace luxxle_sync {
enum class QrCodeDataValidationResult;
}  // namespace luxxle_sync

class LuxxleSyncDeviceTracker : public syncer::DeviceInfoTracker::Observer {
 public:
  LuxxleSyncDeviceTracker(
      syncer::DeviceInfoTracker* device_info_tracker,
      const base::RepeatingCallback<void()>& on_device_info_changed_callback);
  ~LuxxleSyncDeviceTracker() override;

 private:
  void OnDeviceInfoChange() override;

  base::RepeatingCallback<void()> on_device_info_changed_callback_;

  base::ScopedObservation<syncer::DeviceInfoTracker,
                          syncer::DeviceInfoTracker::Observer>
      device_info_tracker_observer_{this};
};

class LuxxleSyncServiceTracker : public syncer::SyncServiceObserver {
 public:
  LuxxleSyncServiceTracker(
      syncer::SyncServiceImpl* sync_service_impl,
      const base::RepeatingCallback<void()>& on_state_changed_callback,
      const base::RepeatingCallback<void()>& on_sync_shutdown_callback);
  ~LuxxleSyncServiceTracker() override;

 private:
  void OnStateChanged(syncer::SyncService* sync) override;
  void OnSyncShutdown(syncer::SyncService* sync) override;

  base::RepeatingCallback<void()> on_state_changed_callback_;
  base::RepeatingCallback<void()> on_sync_shutdown_callback_;

  base::ScopedObservation<syncer::SyncService, syncer::SyncServiceObserver>
      sync_service_observer_{this};
};

class LuxxleSyncWorker : public syncer::SyncServiceObserver {
 public:
  explicit LuxxleSyncWorker(ProfileIOS* profile_);
  LuxxleSyncWorker(const LuxxleSyncWorker&) = delete;
  LuxxleSyncWorker& operator=(const LuxxleSyncWorker&) = delete;
  ~LuxxleSyncWorker() override;

  bool RequestSync();
  std::string GetOrCreateSyncCode();
  bool IsValidSyncCode(const std::string& sync_code);
  bool SetSyncCode(const std::string& sync_code);
  std::string GetSyncCodeFromHexSeed(const std::string& hex_seed);
  std::string GetHexSeedFromSyncCode(const std::string& code_words);
  std::string GetQrCodeJsonFromHexSeed(const std::string& hex_seed);
  luxxle_sync::QrCodeDataValidationResult GetQrCodeValidationResult(
      const std::string& json);
  luxxle_sync::TimeLimitedWords::ValidationStatus GetWordsValidationResult(
      const std::string& time_limited_words);
  std::string GetWordsFromTimeLimitedWords(
      const std::string& time_limited_words);
  std::string GetTimeLimitedWordsFromWords(const std::string& words);
  std::string GetHexSeedFromQrCodeJson(const std::string& json);
  const syncer::DeviceInfo* GetLocalDeviceInfo();
  std::vector<std::unique_ptr<syncer::LuxxleDeviceInfo>> GetDeviceList();
  bool CanSyncFeatureStart();
  bool IsSyncFeatureActive();
  bool IsInitialSyncFeatureSetupComplete();
  bool SetSetupComplete();
  void ResetSync();
  void DeleteDevice(const std::string& device_guid);
  void SetJoinSyncChainCallback(base::OnceCallback<void(bool)> callback);
  void PermanentlyDeleteAccount(
      base::OnceCallback<void(const syncer::SyncProtocolError&)> callback);

 private:
  // syncer::SyncServiceObserver implementation.

  syncer::LuxxleSyncServiceImpl* GetSyncService() const;
  void OnStateChanged(syncer::SyncService* service) override;
  void OnSyncShutdown(syncer::SyncService* service) override;

  void OnResetDone();

  void SetEncryptionPassphrase(syncer::SyncService* service);
  void SetDecryptionPassphrase(syncer::SyncService* service);

  std::string passphrase_;

  raw_ptr<ProfileIOS> profile_;  // NOT OWNED
  base::ScopedMultiSourceObservation<syncer::SyncService,
                                     syncer::SyncServiceObserver>
      sync_service_observer_{this};
  base::WeakPtrFactory<LuxxleSyncWorker> weak_ptr_factory_{this};
};

#endif  // LUXXLE_IOS_BROWSER_API_SYNC_LUXXLE_SYNC_WORKER_H_
