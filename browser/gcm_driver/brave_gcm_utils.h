/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_GCM_DRIVER_BRAVE_GCM_UTILS_H_
#define LUXXLE_BROWSER_GCM_DRIVER_BRAVE_GCM_UTILS_H_

class PrefService;

namespace user_prefs {
class PrefRegistrySyncable;
}  // namespace user_prefs

namespace gcm {

void RegisterGCMProfilePrefs(user_prefs::PrefRegistrySyncable* registry);
void MigrateGCMPrefs(PrefService* prefs);

}  // namespace gcm

#endif  // LUXXLE_BROWSER_GCM_DRIVER_BRAVE_GCM_UTILS_H_
