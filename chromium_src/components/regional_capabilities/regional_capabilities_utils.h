/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_COMPONENTS_REGIONAL_CAPABILITIES_REGIONAL_CAPABILITIES_UTILS_H_
#define LUXXLE_CHROMIUM_SRC_COMPONENTS_REGIONAL_CAPABILITIES_REGIONAL_CAPABILITIES_UTILS_H_

#include <vector>
#include "components/country_codes/country_codes.h"
#include "third_party/search_engines_data/resources/definitions/prepopulated_engines.h"

namespace regional_capabilities {

// Get Luxxle-specific prepopulated engines for a given country
std::vector<const TemplateURLPrepopulateData::PrepopulatedEngine*> 
GetLuxxlePrepopulatedEnginesForCountryID(country_codes::CountryId country_id);

}  // namespace regional_capabilities

// Include the original header
#include <components/regional_capabilities/regional_capabilities_utils.h>  // IWYU pragma: export

#endif  // LUXXLE_CHROMIUM_SRC_COMPONENTS_REGIONAL_CAPABILITIES_REGIONAL_CAPABILITIES_UTILS_H_