/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/browser_window/public/browser_window_features.h"

#include "base/memory/ptr_util.h"
#include "base/notreached.h"
#include "luxxle/browser/ui/luxxle_browser_window.h"
#include "luxxle/browser/ui/sidebar/sidebar_controller.h"
#include "luxxle/browser/ui/sidebar/sidebar_utils.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/tabs/split_view_browser_data.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
#include "luxxle/browser/ui/luxxle_vpn/luxxle_vpn_controller.h"
#endif

#if !BUILDFLAG(ENABLE_LUXXLE_VPN)
// Use stub class to avoid incomplete type build error.
class LuxxleVPNController {};
#endif

// static
std::unique_ptr<BrowserWindowFeatures>
BrowserWindowFeatures::CreateBrowserWindowFeatures() {
  return base::WrapUnique(new BrowserWindowFeatures());
}

// static
void BrowserWindowFeatures::ReplaceBrowserWindowFeaturesForTesting(
    BrowserWindowFeaturesFactory factory) {
  // Upstream doesn't use this static method.
  // Revisit if used. Need to handle GetFactory() overriding.
  NOTREACHED();
}

BrowserWindowFeatures::BrowserWindowFeatures() = default;
BrowserWindowFeatures::~BrowserWindowFeatures() = default;

LuxxleVPNController* BrowserWindowFeatures::luxxle_vpn_controller() {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  return luxxle_vpn_controller_.get();
#else
  NOTREACHED();
#endif
}

void BrowserWindowFeatures::Init(BrowserWindowInterface* browser) {
  BrowserWindowFeatures_ChromiumImpl::Init(browser);

  if (tabs::features::IsLuxxleSplitViewEnabled()) {
    split_view_browser_data_ = std::make_unique<SplitViewBrowserData>(browser);
  }
}

void BrowserWindowFeatures::InitPostBrowserViewConstruction(
    BrowserView* browser_view) {
  BrowserWindowFeatures_ChromiumImpl::InitPostBrowserViewConstruction(
      browser_view);

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  luxxle_vpn_controller_ = std::make_unique<LuxxleVPNController>(browser_view);
#endif
}

void BrowserWindowFeatures::InitPostWindowConstruction(Browser* browser) {
  BrowserWindowFeatures_ChromiumImpl::InitPostWindowConstruction(browser);

  if (sidebar::CanUseSidebar(browser)) {
    sidebar_controller_ = std::make_unique<sidebar::SidebarController>(
        browser, browser->profile());
  }
}
