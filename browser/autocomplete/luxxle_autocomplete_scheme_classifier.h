/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_AUTOCOMPLETE_LUXXLE_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_
#define LUXXLE_BROWSER_AUTOCOMPLETE_LUXXLE_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_

#include <string>

#include "chrome/browser/autocomplete/chrome_autocomplete_scheme_classifier.h"

class LuxxleAutocompleteSchemeClassifier
    : public ChromeAutocompleteSchemeClassifier {
 public:
  explicit LuxxleAutocompleteSchemeClassifier(Profile* profile);
  LuxxleAutocompleteSchemeClassifier(const LuxxleAutocompleteSchemeClassifier&) =
      delete;
  LuxxleAutocompleteSchemeClassifier& operator=(
      const LuxxleAutocompleteSchemeClassifier&) = delete;
  ~LuxxleAutocompleteSchemeClassifier() override;

  metrics::OmniboxInputType GetInputTypeForScheme(
      const std::string& scheme) const override;
};

#endif  // LUXXLE_BROWSER_AUTOCOMPLETE_LUXXLE_AUTOCOMPLETE_SCHEME_CLASSIFIER_H_

