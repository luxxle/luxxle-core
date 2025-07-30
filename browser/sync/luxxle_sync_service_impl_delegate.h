/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_SERVICE_IMPL_DELEGATE_H_
#define LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_SERVICE_IMPL_DELEGATE_H_

#include <utility>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/scoped_observation.h"
#include "luxxle/components/sync/service/sync_service_impl_delegate.h"
#include "components/sync_device_info/device_info_tracker.h"

namespace history {
class HistoryService;
}  // namespace history

namespace syncer {

class LuxxleSyncServiceImpl;
class DeviceInfoSyncService;
class DeviceInfoTracker;
class LocalDeviceInfoProvider;

// Helper class to prevent pass of profile pointer into LuxxleSyncServiceImpl
// and to keep DeviceInfoSyncService
class LuxxleSyncServiceImplDelegate
    : public SyncServiceImplDelegate,
      public syncer::DeviceInfoTracker::Observer {
 public:
  explicit LuxxleSyncServiceImplDelegate(
      DeviceInfoSyncService* device_info_sync_service,
      history::HistoryService* history_service);
  ~LuxxleSyncServiceImplDelegate() override;

  void SuspendDeviceObserverForOwnReset() override;
  void ResumeDeviceObserver() override;

  void SetLocalDeviceAppearedCallback(
      base::OnceCallback<void()> local_device_appeared_callback) override;

  void GetKnownToSyncHistoryCount(
      base::OnceCallback<void(std::pair<bool, int>)> callback) override;

 private:
  // syncer::DeviceInfoTracker::Observer:
  void OnDeviceInfoChange() override;

  void OnSelfDeviceInfoDeleted(void);

  void RecordP3ASyncStatus();

  raw_ptr<syncer::DeviceInfoTracker> device_info_tracker_ = nullptr;
  raw_ptr<syncer::LocalDeviceInfoProvider> local_device_info_provider_ =
      nullptr;
  base::ScopedObservation<syncer::DeviceInfoTracker,
                          syncer::DeviceInfoTracker::Observer>
      device_info_observer_{this};

  raw_ptr<DeviceInfoSyncService> device_info_sync_service_ = nullptr;
  raw_ptr<history::HistoryService> history_service_ = nullptr;

  // This is triggered once after SetLocalDeviceAppearedCallback
  // when the local device first appears in the changed synced devices list
  base::OnceCallback<void()> local_device_appeared_callback_;

  base::WeakPtrFactory<LuxxleSyncServiceImplDelegate> weak_ptr_factory_;

  LuxxleSyncServiceImplDelegate(const LuxxleSyncServiceImplDelegate&) = delete;
  LuxxleSyncServiceImplDelegate& operator=(const LuxxleSyncServiceImplDelegate&) =
      delete;
};

}  // namespace syncer

#endif  // LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_SERVICE_IMPL_DELEGATE_H_
