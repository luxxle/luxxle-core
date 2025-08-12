/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_COMPONENTS_SEARCH_ENGINES_SEARCH_ENGINES_PREF_NAMES_H_
#define LUXXLE_CHROMIUM_SRC_COMPONENTS_SEARCH_ENGINES_SEARCH_ENGINES_PREF_NAMES_H_

#include <components/search_engines/search_engines_pref_names.h>  // IWYU pragma: export

#include "luxxle/components/search_engines/luxxle_search_engines_pref_names.h"

namespace prefs {

inline constexpr char kDefaultSearchProviderByExtension[] =
    "luxxle.default_search_provider_by_extension";
inline constexpr char kLuxxleDefaultSearchVersion[] =
    "luxxle.search.default_version";
inline constexpr char kSyncedDefaultPrivateSearchProviderGUID[] =
    "luxxle.default_private_search_provider_guid";
inline constexpr char kSyncedDefaultPrivateSearchProviderData[] =
    "luxxle.default_private_search_provider_data";

}  // namespace prefs

#endif  // LUXXLE_CHROMIUM_SRC_COMPONENTS_SEARCH_ENGINES_SEARCH_ENGINES_PREF_NAMES_H_