/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/app/luxxle_main_delegate_simple.h"

#include <memory>

#include "base/command_line.h"
#include "base/path_service.h"
#include "chrome/browser/chrome_content_browser_client.h"
#include "luxxle/browser/luxxle_content_browser_client.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/renderer/chrome_content_renderer_client.h"
#include "chrome/utility/chrome_content_utility_client.h"
#include "content/public/app/content_main_delegate.h"
#include "luxxle/browser/luxxle_browser_main_parts.h"

LuxxleMainDelegate::LuxxleMainDelegate(const StartupTimestamps& timestamps)
    : ChromeMainDelegate(timestamps) {}

LuxxleMainDelegate::~LuxxleMainDelegate() = default;

content::ContentBrowserClient* LuxxleMainDelegate::CreateContentBrowserClient() {
#if defined(CHROME_MULTIPLE_DLL_CHILD)
  return nullptr;
#else
  if (chrome_content_browser_client_ == nullptr) {
    chrome_content_browser_client_ = std::make_unique<LuxxleContentBrowserClient>();
  }
  return chrome_content_browser_client_.get();
#endif
}

content::ContentRendererClient* LuxxleMainDelegate::CreateContentRendererClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return nullptr;
#else
  if (chrome_content_renderer_client_ == nullptr) {
    chrome_content_renderer_client_ = std::make_unique<ChromeContentRendererClient>();
  }
  return chrome_content_renderer_client_.get();
#endif
}

content::ContentUtilityClient* LuxxleMainDelegate::CreateContentUtilityClient() {
#if defined(CHROME_MULTIPLE_DLL_BROWSER)
  return nullptr;
#else
  if (chrome_content_utility_client_ == nullptr) {
    chrome_content_utility_client_ = std::make_unique<ChromeContentUtilityClient>();
  }
  return chrome_content_utility_client_.get();
#endif
}

std::optional<int> LuxxleMainDelegate::BasicStartupComplete() {
  // Set up Luxxle-specific paths and configuration
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  
  // Set the application name for Luxxle
  command_line->AppendSwitchASCII("product-name", "Luxxle");
  
  // Set default homepage to luxxle.com
  if (!command_line->HasSwitch("homepage")) {
    command_line->AppendSwitchASCII("homepage", "https://luxxle.com");
  }
  
  // Set the startup URLs to luxxle.com if no URLs are specified
  if (!command_line->HasSwitch("restore-last-session") && 
      command_line->GetArgs().empty()) {
    command_line->AppendArg("https://luxxle.com");
  }
  
  return ChromeMainDelegate::BasicStartupComplete();
}