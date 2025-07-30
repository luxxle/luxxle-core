/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/test/base/chrome_unit_test_suite.h"

#include "luxxle/common/luxxle_content_client.h"
#include "luxxle/common/resource_bundle_helper.h"
#include "luxxle/test/base/testing_luxxle_browser_process.h"
#include "ui/base/resource/resource_bundle.h"

#define InitSharedInstanceWithLocale(...)    \
  InitSharedInstanceWithLocale(__VA_ARGS__); \
  luxxle::InitializeResourceBundle();

#define ChromeContentClient LuxxleContentClient
#include "src/chrome/test/base/chrome_unit_test_suite.cc"
#undef ChromeContentClient
#undef InitSharedInstanceWithLocale
