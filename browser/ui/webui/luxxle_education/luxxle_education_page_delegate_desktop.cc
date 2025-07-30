/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_education/luxxle_education_page_delegate_desktop.h"

#include "luxxle/browser/ui/luxxle_rewards/rewards_panel_coordinator.h"
#include "luxxle/browser/ui/luxxle_vpn/luxxle_vpn_controller.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "chrome/browser/ui/browser_window/public/browser_window_features.h"
#include "chrome/browser/ui/browser_window/public/browser_window_interface.h"
#include "chrome/browser/ui/views/side_panel/side_panel_entry.h"
#include "chrome/browser/ui/views/side_panel/side_panel_ui.h"

namespace luxxle_education {

LuxxleEducationPageDelegateDesktop::LuxxleEducationPageDelegateDesktop(
    BrowserWindowInterface& window_interface)
    : window_interface_(window_interface) {}

LuxxleEducationPageDelegateDesktop::~LuxxleEducationPageDelegateDesktop() =
    default;

void LuxxleEducationPageDelegateDesktop::OpenURL(
    const GURL& url,
    WindowOpenDisposition disposition) {
  window_interface_->OpenGURL(url, disposition);
}

void LuxxleEducationPageDelegateDesktop::OpenRewardsPanel() {
  // TODO(zenparsing): Instead of using a `Browser` pointer,
  // expose Rewards panel functionality via `BrowserWindowFeatures`.
  // See https://github.com/luxxle/luxxle-browser/issues/42179.
  auto* browser = window_interface_->GetBrowserForMigrationOnly();
  CHECK(browser);
  if (auto* panel_coordinator =
          luxxle_rewards::RewardsPanelCoordinator::FromBrowser(browser)) {
    panel_coordinator->OpenRewardsPanel();
  }
}

void LuxxleEducationPageDelegateDesktop::OpenVPNPanel() {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  window_interface_->GetFeatures().luxxle_vpn_controller()->ShowLuxxleVPNBubble(
      /* show_select */ false);
#endif
}

void LuxxleEducationPageDelegateDesktop::OpenAIChat() {
  window_interface_->GetFeatures().side_panel_ui()->Show(
      SidePanelEntry::Key(SidePanelEntryId::kChatUI));
}

}  // namespace luxxle_education
