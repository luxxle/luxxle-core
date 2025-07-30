/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/autocomplete/luxxle_autocomplete_scheme_classifier.h"

#include <string>

#include "base/strings/string_util.h"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/constants/url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"

LuxxleAutocompleteSchemeClassifier::LuxxleAutocompleteSchemeClassifier(
    Profile* profile)
    : ChromeAutocompleteSchemeClassifier(profile) {}

LuxxleAutocompleteSchemeClassifier::~LuxxleAutocompleteSchemeClassifier() =
    default;

// Without this override, typing in luxxle:// URLs will search Google
metrics::OmniboxInputType
LuxxleAutocompleteSchemeClassifier::GetInputTypeForScheme(
    const std::string& scheme) const {
  if (scheme.empty()) {
    return metrics::OmniboxInputType::EMPTY;
  }
  if (base::IsStringASCII(scheme) &&
      base::EqualsCaseInsensitiveASCII(scheme, kLuxxleUIScheme)) {
    return metrics::OmniboxInputType::URL;
  }

  return ChromeAutocompleteSchemeClassifier::GetInputTypeForScheme(scheme);
}
