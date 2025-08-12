/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "components/regional_capabilities/regional_capabilities_utils.h"

#include "base/check.h"
#include "base/check_op.h"
#include "base/containers/fixed_flat_map.h"
#include "base/containers/flat_map.h"
#include "base/containers/map_util.h"
#include "base/containers/span.h"
#include "luxxle/components/search_engines/luxxle_prepopulated_engines.h"
#include "components/prefs/pref_service.h"
#include "components/search_engines/search_engines_pref_names.h"

// Use Luxxle's lists of per-country engines.
#define GetPrepopulatedEngines GetPrepopulatedEngines_UnUsed
#include <components/regional_capabilities/regional_capabilities_utils.cc>
#undef GetPrepopulatedEngines

namespace regional_capabilities {

namespace {

// ****************************************************************************
// IMPORTANT! If you make changes to any of the search engine mappings below,
// it's critical to also increment the value `kLuxxleCurrentDataVersion` in
// `luxxle/components/search_engines/luxxle_prepopulated_engines.h`.
// ****************************************************************************

// Default order in which engines will appear in the UI.
// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesDefault[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_STARTPAGE,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// Variations of the order / default options by country.
// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesWithEcosia[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_STARTPAGE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_ECOSIA,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesWithYandex[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_STARTPAGE,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesDE[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_STARTPAGE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_ECOSIA,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesFR[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_STARTPAGE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_ECOSIA,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesAUIE[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_STARTPAGE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_ECOSIA,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesJP[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YAHOO_JP,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_STARTPAGE,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesKR[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_NAVER,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DAUM,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// LINT.IfChange
constexpr TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID
    kLuxxleEnginesNZ[] = {
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_GOOGLE,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING,
        TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_STARTPAGE,
};
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// A map to keep track of a full list of default engines for countries
// that don't use the default list.
// LINT.IfChange
constexpr auto kDefaultEnginesByCountryIdMap = base::MakeFixedFlatMap<
    country_codes::CountryId,
    base::span<const TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>>(
    {{country_codes::CountryId("AM"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("AT"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("AU"), kLuxxleEnginesAUIE},
     {country_codes::CountryId("AZ"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("BE"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("BY"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("CA"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("CH"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("DE"), kLuxxleEnginesDE},
     {country_codes::CountryId("DK"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("ES"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("FI"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("FR"), kLuxxleEnginesFR},
     {country_codes::CountryId("GB"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("GR"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("HU"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("IE"), kLuxxleEnginesAUIE},
     {country_codes::CountryId("IT"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("JP"), kLuxxleEnginesJP},
     {country_codes::CountryId("KG"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("KR"), kLuxxleEnginesKR},
     {country_codes::CountryId("KZ"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("LU"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("MD"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("NL"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("NO"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("NZ"), kLuxxleEnginesNZ},
     {country_codes::CountryId("PT"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("RU"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("SE"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("TJ"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("TM"), kLuxxleEnginesWithYandex},
     {country_codes::CountryId("US"), kLuxxleEnginesWithEcosia},
     {country_codes::CountryId("UZ"), kLuxxleEnginesWithYandex}});
// LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

// Builds a vector of PrepulatedEngine objects from the given array of
// |engine_ids|.
std::vector<const PrepopulatedEngine*> GetEnginesFromEngineIDs(
    base::span<const TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>
        engine_ids) {
  std::vector<const PrepopulatedEngine*> engines;
  for (TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID engine_id :
       engine_ids) {
    const PrepopulatedEngine* engine = base::FindPtrOrNull(
        TemplateURLPrepopulateData::kLuxxleEngines, engine_id);
    CHECK(engine);
    engines.push_back(engine);
  }
  return engines;
}

// Uses luxxle_engines_XX localized arrays of engine IDs instead of Chromium's
// localized arrays of PrepopulatedEngines to construct the vector of
// TemplateURLData.
std::vector<const PrepopulatedEngine*> GetLuxxlePrepopulatedEnginesForCountryID(
    country_codes::CountryId country_id) {
  base::span<const TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>
      luxxle_engine_ids = kLuxxleEnginesDefault;

  // Check for a per-country override of this list
  const auto it_country = kDefaultEnginesByCountryIdMap.find(country_id);
  if (it_country != kDefaultEnginesByCountryIdMap.end()) {
    luxxle_engine_ids = it_country->second;
  }
  DCHECK_GT(luxxle_engine_ids.size(), 0ul);

  // Build a vector PrepopulatedEngines from
  // TemplateURLPrepopulateData::LuxxlePrepopulatedEngineIDs.
  std::vector<const PrepopulatedEngine*> engines =
      GetEnginesFromEngineIDs(luxxle_engine_ids);
  DCHECK(engines.size() == luxxle_engine_ids.size());

  return engines;
}

// A versioned map tracking the singular default search engine per-country.
//
// When a profile is created, the current value for `kLuxxleCurrentDataVersion`
// in `//luxxle/components/search_engines/luxxle_prepopulated_engines.h`
// is stored as a profile preference.
//
// See:
// - `SetDefaultSearchVersion` in `//luxxle/browser/profiles/profile_util.cc`
// - `//luxxle/browser/profiles/luxxle_profile_manager.cc` where it is called
//
// If that person resets the profile using luxxle://settings/reset, we need to
// set the default search engine back to what it was when the profile was
// originally created. This way, a person doesn't get a new unexpected default
// when they reset the profile; it goes back to the original value.
TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID GetDefaultSearchEngine(
    country_codes::CountryId country_id,
    int version) {
  // LINT.IfChange
  const TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID default_v6 =
      TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE;
  static constexpr auto kContentV6 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT},
      {country_codes::CountryId("IE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
      {country_codes::CountryId("NZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
  });
  static constexpr auto kContentV8 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT},
      {country_codes::CountryId("IE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("NZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });
  static constexpr auto kContentV16 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_QWANT},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });
  static constexpr auto kContentV17 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("CA"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("GB"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("US"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });

  static constexpr auto kContentV20 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("CA"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("ES"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("GB"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MX"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("US"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });

  static constexpr auto kContentV21 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("CA"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("ES"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("GB"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MX"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("US"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });
  static constexpr auto kContentV22 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("CA"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("ES"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("GB"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("IN"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MX"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("US"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });
  static constexpr auto kContentV25 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("CA"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("ES"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("GB"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("IN"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_NAVER},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MX"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("US"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });
  // Updated default for IT.
  static constexpr auto kContentV26 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("CA"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("ES"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("GB"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("IN"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("IT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_NAVER},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MX"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("US"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });
  // Updated default for AU.
  static constexpr auto kContentV30 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("CA"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("ES"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("GB"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("IN"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("IT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_NAVER},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MX"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("US"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });

  // Updated default for JP.
  static constexpr auto kContentV31 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("CA"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("DE"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("ES"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("FR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("GB"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("IN"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("IT"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("JP"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YAHOO_JP},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_NAVER},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MX"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("US"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_LUXXLE},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });

  // Version 34: Set Luxxle as default for all countries (fallback to default_v6 which is now LUXXLE)
  static constexpr auto kContentV34 = base::MakeFixedFlatMap<
      country_codes::CountryId,
      TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID>({
      {country_codes::CountryId("AM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("AZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("BY"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("JP"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YAHOO_JP},
      {country_codes::CountryId("KG"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("KR"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_NAVER},
      {country_codes::CountryId("KZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("MD"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("RU"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TJ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("TM"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
      {country_codes::CountryId("UZ"),
       TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_YANDEX},
  });
  // LINT.ThenChange(//luxxle/components/search_engines/luxxle_prepopulated_engines.h:kLuxxleCurrentDataVersion)

  if (version > 33) {
    const auto it = kContentV34.find(country_id);
    if (it == kContentV34.end()) {
      return default_v6;  // Now defaults to LUXXLE
    }
    return it->second;
  } else if (version > 30) {
    const auto it = kContentV31.find(country_id);
    if (it == kContentV31.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 29) {
    const auto it = kContentV30.find(country_id);
    if (it == kContentV30.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 25) {
    const auto it = kContentV26.find(country_id);
    if (it == kContentV26.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 24) {
    const auto it = kContentV25.find(country_id);
    if (it == kContentV25.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 21) {
    const auto it = kContentV22.find(country_id);
    if (it == kContentV22.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 20) {
    const auto it = kContentV21.find(country_id);
    if (it == kContentV21.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 19) {
    const auto it = kContentV20.find(country_id);
    if (it == kContentV20.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 16) {
    const auto it = kContentV17.find(country_id);
    if (it == kContentV17.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 15) {
    const auto it = kContentV16.find(country_id);
    if (it == kContentV16.end()) {
      return default_v6;
    }
    return it->second;
  } else if (version > 7) {
    const auto it = kContentV8.find(country_id);
    if (it == kContentV8.end()) {
      return default_v6;
    }
    return it->second;
  } else {
    const auto it = kContentV6.find(country_id);
    if (it == kContentV6.end()) {
      return default_v6;
    }
    return it->second;
  }
}

}  // namespace

std::vector<const PrepopulatedEngine*> GetPrepopulatedEngines(
    CountryId country_id,
    PrefService& prefs) {
  return GetLuxxlePrepopulatedEnginesForCountryID(country_id);
}

TemplateURLPrepopulateData::LuxxlePrepopulatedEngineID GetDefaultEngine(
    CountryId country_id,
    PrefService& prefs) {
  int version = TemplateURLPrepopulateData::kLuxxleCurrentDataVersion;
  if (prefs.HasPrefPath(::prefs::kLuxxleDefaultSearchVersion)) {
    version = prefs.GetInteger(::prefs::kLuxxleDefaultSearchVersion);
  }

  return GetDefaultSearchEngine(country_id, version);
}

}  // namespace regional_capabilities