/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_APP_LUXXLE_MAIN_DELEGATE_SIMPLE_H_
#define LUXXLE_APP_LUXXLE_MAIN_DELEGATE_SIMPLE_H_

#include <memory>
#include <optional>

#include "chrome/app/chrome_main_delegate.h"
#include "content/public/browser/content_browser_client.h"

class ChromeContentBrowserClient;
class ChromeContentRendererClient;
class ChromeContentUtilityClient;
class LuxxleContentBrowserClient;

// Luxxle implementation of ContentMainDelegate.
// This is a simplified version that uses Chrome's content clients
// but with Luxxle-specific branding and configuration.
class LuxxleMainDelegate : public ChromeMainDelegate {
 public:
  LuxxleMainDelegate(const LuxxleMainDelegate&) = delete;
  LuxxleMainDelegate& operator=(const LuxxleMainDelegate&) = delete;

  explicit LuxxleMainDelegate(const StartupTimestamps& timestamps);
  ~LuxxleMainDelegate() override;

  // ContentMainDelegate implementation:
  std::optional<int> BasicStartupComplete() override;

 protected:
  // ChromeMainDelegate implementation:
  content::ContentBrowserClient* CreateContentBrowserClient() override;
  content::ContentRendererClient* CreateContentRendererClient() override;
  content::ContentUtilityClient* CreateContentUtilityClient() override;

 private:
#if !defined(CHROME_MULTIPLE_DLL_CHILD)
  std::unique_ptr<content::ContentBrowserClient> chrome_content_browser_client_;
#endif
#if !defined(CHROME_MULTIPLE_DLL_BROWSER)
  std::unique_ptr<ChromeContentRendererClient> chrome_content_renderer_client_;
  std::unique_ptr<ChromeContentUtilityClient> chrome_content_utility_client_;
#endif
};

#endif  // LUXXLE_APP_LUXXLE_MAIN_DELEGATE_SIMPLE_H_