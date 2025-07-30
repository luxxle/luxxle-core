/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_PAGE_DELEGATE_DESKTOP_H_
#define LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_PAGE_DELEGATE_DESKTOP_H_

#include "base/memory/raw_ref.h"
#include "luxxle/browser/ui/webui/luxxle_browser_command/luxxle_browser_command_handler.h"
#include "ui/base/window_open_disposition.h"
#include "url/gurl.h"

class BrowserWindowInterface;

namespace luxxle_education {

// Handles browser-level requests from the Luxxle Education WebUI.
class LuxxleEducationPageDelegateDesktop
    : public LuxxleBrowserCommandHandler::Delegate {
 public:
  explicit LuxxleEducationPageDelegateDesktop(
      BrowserWindowInterface& window_interface);
  ~LuxxleEducationPageDelegateDesktop() override;

  void OpenURL(const GURL& url, WindowOpenDisposition disposition) override;
  void OpenRewardsPanel() override;
  void OpenVPNPanel() override;
  void OpenAIChat() override;

 private:
  // The browser window interface for tab w/ luxxle_education's WebUI.
  raw_ref<BrowserWindowInterface> window_interface_;
};

}  // namespace luxxle_education

#endif  // LUXXLE_BROWSER_UI_WEBUI_LUXXLE_EDUCATION_LUXXLE_EDUCATION_PAGE_DELEGATE_DESKTOP_H_
