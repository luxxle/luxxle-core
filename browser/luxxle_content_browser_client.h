/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_LUXXLE_CONTENT_BROWSER_CLIENT_H_
#define LUXXLE_BROWSER_LUXXLE_CONTENT_BROWSER_CLIENT_H_

#include "chrome/browser/chrome_content_browser_client.h"
#include "chrome/browser/startup_data.h"

class LuxxleContentBrowserClient : public ChromeContentBrowserClient {
 public:
  LuxxleContentBrowserClient();
  ~LuxxleContentBrowserClient() override;

  LuxxleContentBrowserClient(const LuxxleContentBrowserClient&) = delete;
  LuxxleContentBrowserClient& operator=(const LuxxleContentBrowserClient&) = delete;

  // ChromeContentBrowserClient overrides:
  std::unique_ptr<content::BrowserMainParts> CreateBrowserMainParts(
      bool is_integration_test) override;

 private:
  std::unique_ptr<StartupData> startup_data_;
};

#endif  // LUXXLE_BROWSER_LUXXLE_CONTENT_BROWSER_CLIENT_H_