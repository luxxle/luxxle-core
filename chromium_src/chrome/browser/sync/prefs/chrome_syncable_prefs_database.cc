/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <optional>
#include <string_view>

#include "base/containers/fixed_flat_map.h"
#include "chrome/browser/sync/prefs/chrome_syncable_prefs_database.h"

namespace browser_sync {
namespace {
namespace luxxle_syncable_prefs_ids {
enum {
  // Starts at 300000 to avoid clashing with the Chromium's syncable_prefs_ids:
  // chrome_syncable_prefs_database.cc starts at 100000
  // ios_chrome_syncable_prefs_database.cc starts at 200000
  kProfileContentSettingsExceptionsShieldsAds = 300000,
  kProfileContentSettingsExceptionsTrackers = 300001,
  kProfileContentSettingsExceptionsHttpsUpgrades = 300002,
  kProfileContentSettingsExceptionsHttpUpgradableResources = 300003,
  kProfileContentSettingsExceptionsReferrers = 300004,
  kProfileContentSettingsExceptionsShieldsCookiesV3 = 300005,
  kProfileContentSettingsExceptionsCosmeticFiltering = 300006,
  kProfileContentSettingsExceptionsFingerprintingV2 = 300007,
  kProfileContentSettingsExceptionsLuxxleShields = 300008,
  kProfileContentSettingsExceptionsLuxxleSpeedreader = 300009,
  kProfileDefaultContentSettingValuesShieldsAds = 300010,
  kProfileDefaultContentSettingValuesTrackers = 300011,
  kProfileDefaultContentSettingValuesHttpsUpgrades = 300012,
  kProfileDefaultContentSettingValuesHttpUpgradableResources = 300013,
  kProfileDefaultContentSettingValuesReferrers = 300014,
  kProfileDefaultContentSettingValuesShieldsCookiesV3 = 300015,
  kProfileDefaultContentSettingValuesCosmeticFiltering = 300016,
  kProfileDefaultContentSettingValuesFingerprintingV2 = 300017,
  kProfileDefaultContentSettingValuesLuxxleShields = 300018,
  kProfileDefaultContentSettingValuesLuxxleSpeedreader = 300019,
  kProfileContentSettingsPartitionedExceptionsShieldsAds = 300020,
  kProfileContentSettingsPartitionedExceptionsTrackers = 300021,
  kProfileContentSettingsPartitionedExceptionsHttpsUpgrades = 300022,
  kProfileContentSettingsPartitionedExceptionsHttpUpgradableResources = 300023,
  kProfileContentSettingsPartitionedExceptionsReferrers = 300024,
  kProfileContentSettingsPartitionedExceptionsShieldsCookiesV3 = 300025,
  kProfileContentSettingsPartitionedExceptionsCosmeticFiltering = 300026,
  kProfileContentSettingsPartitionedExceptionsFingerprintingV2 = 300027,
  kProfileContentSettingsPartitionedExceptionsLuxxleShields = 300028,
  kProfileContentSettingsPartitionedExceptionsLuxxleSpeedreader = 300029,
};
}  // namespace luxxle_syncable_prefs_ids

const auto& LuxxleSyncablePreferences() {
  static constexpr auto kLuxxleSyncablePrefsAllowList = base::MakeFixedFlatMap<
      std::string_view, sync_preferences::SyncablePrefMetadata>({
      {"profile.content_settings.exceptions.luxxleShields",
       {luxxle_syncable_prefs_ids::kProfileContentSettingsExceptionsLuxxleShields,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.luxxleSpeedreader",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsExceptionsLuxxleSpeedreader,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.cosmeticFiltering",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsExceptionsCosmeticFiltering,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.fingerprintingV2",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsExceptionsFingerprintingV2,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.httpUpgradableResources",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsExceptionsHttpUpgradableResources,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.httpsUpgrades",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsExceptionsHttpsUpgrades,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.referrers",
       {luxxle_syncable_prefs_ids::kProfileContentSettingsExceptionsReferrers,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.shieldsAds",
       {luxxle_syncable_prefs_ids::kProfileContentSettingsExceptionsShieldsAds,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.shieldsCookiesV3",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsExceptionsShieldsCookiesV3,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.exceptions.trackers",
       {luxxle_syncable_prefs_ids::kProfileContentSettingsExceptionsTrackers,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.default_content_setting_values.luxxleShields",
       {luxxle_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesLuxxleShields,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.luxxleSpeedreader",
       {luxxle_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesLuxxleSpeedreader,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.cosmeticFiltering",
       {luxxle_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesCosmeticFiltering,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.fingerprintingV2",
       {luxxle_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesFingerprintingV2,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.httpUpgradableResources",
       {luxxle_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesHttpUpgradableResources,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.httpsUpgrades",
       {luxxle_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesHttpsUpgrades,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.referrers",
       {luxxle_syncable_prefs_ids::kProfileDefaultContentSettingValuesReferrers,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.shieldsAds",
       {luxxle_syncable_prefs_ids::kProfileDefaultContentSettingValuesShieldsAds,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.shieldsCookiesV3",
       {luxxle_syncable_prefs_ids::
            kProfileDefaultContentSettingValuesShieldsCookiesV3,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.default_content_setting_values.trackers",
       {luxxle_syncable_prefs_ids::kProfileDefaultContentSettingValuesTrackers,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kNone}},
      {"profile.content_settings.partitioned_exceptions.luxxleShields",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsLuxxleShields,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions.luxxleSpeedreader",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsLuxxleSpeedreader,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions.cosmeticFiltering",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsCosmeticFiltering,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions.fingerprintingV2",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsFingerprintingV2,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions."
       "httpUpgradableResources",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsHttpUpgradableResources,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions.httpsUpgrades",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsHttpsUpgrades,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions.referrers",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsReferrers,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions.shieldsAds",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsShieldsAds,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions.shieldsCookiesV3",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsShieldsCookiesV3,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
      {"profile.content_settings.partitioned_exceptions.trackers",
       {luxxle_syncable_prefs_ids::
            kProfileContentSettingsPartitionedExceptionsTrackers,
        syncer::PREFERENCES, sync_preferences::PrefSensitivity::kNone,
        sync_preferences::MergeBehavior::kMergeableDict}},
  });
  return kLuxxleSyncablePrefsAllowList;
}
}  // namespace
}  // namespace browser_sync

#define GetSyncablePrefMetadata GetSyncablePrefMetadata_ChromiumImpl
#include "src/chrome/browser/sync/prefs/chrome_syncable_prefs_database.cc"
#undef GetSyncablePrefMetadata

namespace browser_sync {

std::optional<sync_preferences::SyncablePrefMetadata>
ChromeSyncablePrefsDatabase::GetSyncablePrefMetadata(
    std::string_view pref_name) const {
  const auto it = LuxxleSyncablePreferences().find(pref_name);
  if (it != LuxxleSyncablePreferences().end()) {
    return it->second;
  }
  return GetSyncablePrefMetadata_ChromiumImpl(pref_name);
}

}  // namespace browser_sync
