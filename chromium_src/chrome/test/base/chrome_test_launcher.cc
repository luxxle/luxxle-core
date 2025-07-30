/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/test/base/chrome_test_launcher.h"

#include "luxxle/app/luxxle_main_delegate.h"
#include "luxxle/utility/luxxle_content_utility_client.h"

#define ChromeContentUtilityClient LuxxleContentUtilityClient
#define ChromeMainDelegate LuxxleMainDelegate
#include "src/chrome/test/base/chrome_test_launcher.cc"
#undef ChromeMainDelegate
#undef ChromeContentUtilityClient
