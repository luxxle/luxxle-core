/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_TRANSLATE_LUXXLE_TRANSLATE_PREFS_MIGRATION_H_
#define LUXXLE_BROWSER_TRANSLATE_LUXXLE_TRANSLATE_PREFS_MIGRATION_H_

class PrefRegistrySimple;
class PrefService;

namespace translate {

namespace prefs {
inline constexpr char kMigratedToInternalTranslation[] =
    "luxxle.translate_migrated_from_extension";
}  // namespace prefs

void RegisterLuxxleProfilePrefsForMigration(PrefRegistrySimple* registry);

void ClearMigrationLuxxleProfilePrefs(PrefService* prefs);

}  // namespace translate

#endif  // LUXXLE_BROWSER_TRANSLATE_LUXXLE_TRANSLATE_PREFS_MIGRATION_H_
