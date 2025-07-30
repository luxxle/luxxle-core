/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/luxxle_content_browser_client.h"

#include <memory>

#include "luxxle/browser/luxxle_browser_main_parts.h"
#include "chrome/browser/startup_data.h"

LuxxleContentBrowserClient::LuxxleContentBrowserClient() = default;

LuxxleContentBrowserClient::~LuxxleContentBrowserClient() = default;

std::unique_ptr<content::BrowserMainParts>
LuxxleContentBrowserClient::CreateBrowserMainParts(bool is_integration_test) {
  startup_data_ = std::make_unique<StartupData>();
  std::unique_ptr<ChromeBrowserMainParts> main_parts =
      std::make_unique<ChromeBrowserMainParts>(
          is_integration_test,
          startup_data_.get());
  return main_parts;
}