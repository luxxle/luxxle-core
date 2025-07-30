/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/profiles/luxxle_incognito_menu_view.h"
#include "luxxle/browser/ui/views/profiles/luxxle_profile_menu_view.h"

#include "chrome/browser/ui/views/profiles/incognito_menu_view.h"
#include "chrome/browser/ui/views/profiles/profile_menu_view.h"

#define IncognitoMenuView LuxxleIncognitoMenuView
#define ProfileMenuView LuxxleProfileMenuView
#include "src/chrome/browser/ui/views/profiles/profile_menu_coordinator.cc"
#undef IncognitoMenuView
#undef ProfileMenuView
