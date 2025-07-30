/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "luxxle/ios/browser/api/sync/driver/luxxle_sync_profile_service.h"

#include <unordered_map>

#include "base/memory/raw_ptr.h"
#include "components/sync/base/data_type.h"
#include "components/sync/base/user_selectable_type.h"
#include "components/sync/service/sync_service.h"
#include "components/sync/service/sync_user_settings.h"
#include "ios/web/public/thread/web_thread.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

static_assert(static_cast<NSInteger>(syncer::UserSelectableType::kCookies) ==
                  static_cast<NSInteger>(syncer::UserSelectableType::kLastType),
              "syncer::UserSelectableType has changed in a Chromium update");

namespace luxxle {
namespace ios {
std::unordered_map<syncer::UserSelectableType, LuxxleSyncUserSelectableTypes>
    mapping = {
        {syncer::UserSelectableType::kBookmarks,
         LuxxleSyncUserSelectableTypes_BOOKMARKS},
        {syncer::UserSelectableType::kPreferences,
         LuxxleSyncUserSelectableTypes_PREFERENCES},
        {syncer::UserSelectableType::kPasswords,
         LuxxleSyncUserSelectableTypes_PASSWORDS},
        {syncer::UserSelectableType::kAutofill,
         LuxxleSyncUserSelectableTypes_AUTOFILL},
        {syncer::UserSelectableType::kThemes,
         LuxxleSyncUserSelectableTypes_THEMES},
        {syncer::UserSelectableType::kHistory,
         LuxxleSyncUserSelectableTypes_HISTORY},
        {syncer::UserSelectableType::kExtensions,
         LuxxleSyncUserSelectableTypes_EXTENSIONS},
        {syncer::UserSelectableType::kApps, LuxxleSyncUserSelectableTypes_APPS},
        {syncer::UserSelectableType::kReadingList,
         LuxxleSyncUserSelectableTypes_READING_LIST},
        {syncer::UserSelectableType::kTabs, LuxxleSyncUserSelectableTypes_TABS},
        {syncer::UserSelectableType::kSavedTabGroups,
         LuxxleSyncUserSelectableTypes_SAVED_TAB_GROUPS},
        {syncer::UserSelectableType::kPayments,
         LuxxleSyncUserSelectableTypes_PAYMENTS},
        {syncer::UserSelectableType::kProductComparison,
         LuxxleSyncUserSelectableTypes_PRODUCT_COMPARISON},
        {syncer::UserSelectableType::kCookies,
         LuxxleSyncUserSelectableTypes_COOKIES}};

syncer::UserSelectableTypeSet user_types_from_options(
    LuxxleSyncUserSelectableTypes options) {
  syncer::UserSelectableTypeSet results;
  for (auto it = mapping.begin(); it != mapping.end(); ++it) {
    if (options & it->second) {
      results.Put(it->first);
    }
  }
  return results;
}

LuxxleSyncUserSelectableTypes options_from_user_types(
    const syncer::UserSelectableTypeSet& types) {
  LuxxleSyncUserSelectableTypes results = LuxxleSyncUserSelectableTypes_NONE;
  for (auto it = mapping.begin(); it != mapping.end(); ++it) {
    if (types.Has(it->first)) {
      results |= it->second;
    }
  }
  return results;
}
}  // namespace ios
}  // namespace luxxle

@interface LuxxleSyncProfileServiceIOS () {
  raw_ptr<syncer::SyncService> sync_service_;
  std::unordered_map<syncer::UserSelectableType, LuxxleSyncUserSelectableTypes>
      type_mapping;
}
@end

@implementation LuxxleSyncProfileServiceIOS

- (instancetype)initWithProfileSyncService:(syncer::SyncService*)syncService {
  if ((self = [super init])) {
    DCHECK_CURRENTLY_ON(web::WebThread::UI);
    sync_service_ = syncService;
  }
  return self;
}

- (bool)isSyncFeatureActive {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  return sync_service_->IsSyncFeatureActive();
}

- (LuxxleSyncUserSelectableTypes)activeSelectableTypes {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::DataTypeSet active_types = sync_service_->GetActiveDataTypes();

  syncer::UserSelectableTypeSet user_types;
  for (syncer::UserSelectableType type : syncer::UserSelectableTypeSet::All()) {
    if (active_types.Has(syncer::UserSelectableTypeToCanonicalDataType(type))) {
      user_types.Put(type);
    }
  }
  return luxxle::ios::options_from_user_types(user_types);
}

- (LuxxleSyncUserSelectableTypes)userSelectedTypes {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  syncer::UserSelectableTypeSet types =
      sync_service_->GetUserSettings()->GetSelectedTypes();
  return luxxle::ios::options_from_user_types(types);
}

- (void)setUserSelectedTypes:(LuxxleSyncUserSelectableTypes)options {
  DCHECK_CURRENTLY_ON(web::WebThread::UI);
  bool sync_everything = false;
  syncer::UserSelectableTypeSet selected_types =
      luxxle::ios::user_types_from_options(options);
  sync_service_->GetUserSettings()->SetSelectedTypes(sync_everything,
                                                     selected_types);
}

@end
