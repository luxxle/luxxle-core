/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/components/sync/service/luxxle_sync_service_impl.h"
#include "components/os_crypt/sync/os_crypt.h"

// IsSetupInProgress isn't accurate in luxxle sync flow especially for the first
// time setup, we rely on it to display setup dialog
#define LUXXLE_GET_SYNC_STATUS_DICTIONARY                                     \
  sync_status.Set(                                                           \
      "firstSetupInProgress",                                                \
      service && !disallowed_by_policy &&                                    \
          !service->GetUserSettings()->IsInitialSyncFeatureSetupComplete()); \
  {                                                                          \
    syncer::LuxxleSyncServiceImpl* luxxle_sync_service =                       \
        static_cast<syncer::LuxxleSyncServiceImpl*>(service);                 \
    if (luxxle_sync_service) {                                                \
      bool failed_to_decrypt = false;                                        \
      luxxle_sync_service->prefs().GetSeed(&failed_to_decrypt);               \
      sync_status.Set("hasSyncWordsDecryptionError", failed_to_decrypt);     \
      sync_status.Set("isOsEncryptionAvailable",                             \
                      OSCrypt::IsEncryptionAvailable());                     \
    }                                                                        \
  }

#include "src/chrome/browser/ui/webui/settings/people_handler.cc"
#undef LUXXLE_GET_SYNC_STATUS_DICTIONARY
