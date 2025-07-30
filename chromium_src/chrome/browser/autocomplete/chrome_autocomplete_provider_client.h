// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_AUTOCOMPLETE_CHROME_AUTOCOMPLETE_PROVIDER_CLIENT_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_AUTOCOMPLETE_CHROME_AUTOCOMPLETE_PROVIDER_CLIENT_H_

#include "luxxle/components/commander/common/buildflags/buildflags.h"
#include "components/omnibox/browser/autocomplete_provider_client.h"

#if BUILDFLAG(ENABLE_COMMANDER)
#include "luxxle/components/commander/browser/commander_frontend_delegate.h"
#define GetAutocompleteClassifier       \
  GetAutocompleteClassifier() override; \
  commander::CommanderFrontendDelegate* GetCommanderDelegate
#endif  // BUILDFLAG(ENABLE_COMMANDER)

#define GetInMemoryDatabase                           \
  GetInMemoryDatabase() override;                     \
  void OpenLeo(const std::u16string& query) override; \
  bool IsLeoProviderEnabled

#define GetAcceptLanguages             \
  GetAcceptLanguages() const override; \
  std::u16string GetClipboardText

#include "src/chrome/browser/autocomplete/chrome_autocomplete_provider_client.h"  // IWYU pragma: export

#undef GetAcceptLanguages
#undef GetInMemoryDatabase
#undef GetAutocompleteClassifier

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_AUTOCOMPLETE_CHROME_AUTOCOMPLETE_PROVIDER_CLIENT_H_
