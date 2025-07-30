/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/gcm_driver/luxxle_gcm_channel_status.h"

#include <memory>

#include "luxxle/components/constants/pref_names.h"
#include "chrome/browser/gcm/gcm_profile_service_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "components/gcm_driver/gcm_driver_desktop.h"
#include "components/gcm_driver/gcm_profile_service.h"
#include "components/prefs/pref_service.h"

namespace gcm {

constexpr char kLuxxleGCMStatusKey[] = "luxxle_gcm_channel_status";

LuxxleGCMChannelStatus::LuxxleGCMChannelStatus(Profile* profile, bool enabled)
    : profile_(profile), gcm_enabled_(enabled) {}

// static
LuxxleGCMChannelStatus* LuxxleGCMChannelStatus::GetForProfile(
    Profile* profile) {
  LuxxleGCMChannelStatus* status = static_cast<LuxxleGCMChannelStatus*>(
      profile->GetUserData(kLuxxleGCMStatusKey));

  if (!status) {
    bool enabled = profile->GetPrefs()->GetBoolean(kLuxxleGCMChannelStatus);
    // Object cleanup is handled by SupportsUserData
    profile->SetUserData(
        kLuxxleGCMStatusKey,
        std::make_unique<LuxxleGCMChannelStatus>(profile, enabled));
    status = static_cast<LuxxleGCMChannelStatus*>(
        profile->GetUserData(kLuxxleGCMStatusKey));
  }
  return status;
}

bool LuxxleGCMChannelStatus::IsGCMEnabled() const {
  return gcm_enabled_;
}

void LuxxleGCMChannelStatus::UpdateGCMDriverStatus() {
  if (!profile_)
    return;
  gcm::GCMProfileService* gcm_profile_service =
      gcm::GCMProfileServiceFactory::GetForProfile(profile_);
  if (!gcm_profile_service)
    return;
  gcm::GCMDriver* gcm_driver = gcm_profile_service->driver();
  if (!gcm_driver)
    return;
  gcm_driver->SetEnabled(IsGCMEnabled());
}

}  // namespace gcm
