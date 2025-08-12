/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_LUXXLE_PROFILE_PREFS_H_
#define LUXXLE_BROWSER_LUXXLE_PROFILE_PREFS_H_

namespace user_prefs {
class PrefRegistrySyncable;
}

namespace luxxle {

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_LUXXLE_PROFILE_PREFS_H_