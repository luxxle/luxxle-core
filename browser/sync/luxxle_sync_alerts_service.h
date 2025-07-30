/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_ALERTS_SERVICE_H_
#define LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_ALERTS_SERVICE_H_

#include "base/memory/raw_ptr.h"
#include "base/scoped_multi_source_observation.h"
#include "components/keyed_service/core/keyed_service.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_service_observer.h"

class Profile;

namespace syncer {
class SyncService;
class SyncServiceObserver;
}  // namespace syncer

class LuxxleSyncAlertsService : public KeyedService,
                               public syncer::SyncServiceObserver {
 public:
  explicit LuxxleSyncAlertsService(Profile* profile);
  LuxxleSyncAlertsService(const LuxxleSyncAlertsService&) = delete;
  LuxxleSyncAlertsService& operator=(const LuxxleSyncAlertsService&) = delete;
  ~LuxxleSyncAlertsService() override;

 private:
  // syncer::SyncServiceObserver implementation.
  void OnStateChanged(syncer::SyncService* service) override;
  void OnSyncShutdown(syncer::SyncService* sync) override;

  void ShowInfobar();

  raw_ptr<Profile> profile_ = nullptr;
  base::ScopedMultiSourceObservation<syncer::SyncService,
                                     syncer::SyncServiceObserver>
      sync_service_observer_{this};
  base::WeakPtrFactory<LuxxleSyncAlertsService> weak_ptr_factory_{this};
};

#endif  // LUXXLE_BROWSER_SYNC_LUXXLE_SYNC_ALERTS_SERVICE_H_
