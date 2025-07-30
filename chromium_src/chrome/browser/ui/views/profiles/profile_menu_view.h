/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_

#define LUXXLE_PROFILE_MENU_VIEW_H friend class LuxxleProfileMenuView;

#define OnExitProfileButtonClicked virtual OnExitProfileButtonClicked
#define BuildFeatureButtons virtual BuildFeatureButtons

#include "src/chrome/browser/ui/views/profiles/profile_menu_view.h"  // IWYU pragma: export
#undef BuildFeatureButtons
#undef OnExitProfileButtonClicked
#undef LUXXLE_PROFILE_MENU_VIEW_H

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_PROFILES_PROFILE_MENU_VIEW_H_
