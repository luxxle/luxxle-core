/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/themes/luxxle_theme_service.h"

#include <memory>

#include "luxxle/browser/extensions/luxxle_theme_event_router.h"

LuxxleThemeService::LuxxleThemeService(Profile* profile,
                                     const ThemeHelper& theme_helper)
    : ThemeService(profile, theme_helper) {
  luxxle_theme_event_router_ =
      std::make_unique<extensions::LuxxleThemeEventRouter>(profile);
}

LuxxleThemeService::~LuxxleThemeService() = default;

// We replace the baseline theme with the grayscale theme - the default theme is
// blue ish while ours is gray.
bool LuxxleThemeService::GetIsGrayscale() const {
  return ThemeService::GetIsGrayscale() || GetIsBaseline();
}

void LuxxleThemeService::SetLuxxleThemeEventRouterForTesting(
    extensions::LuxxleThemeEventRouter* mock_router) {
  luxxle_theme_event_router_.reset(mock_router);
}
