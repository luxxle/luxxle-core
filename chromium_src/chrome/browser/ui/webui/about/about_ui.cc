// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/components/webui/about_ui.h"  // IWYU pragma: export
#include "src/chrome/browser/ui/webui/about/about_ui.cc"

std::string AboutUIHTMLSource::ChromeURLs(
    content::BrowserContext* browser_context) const {
  return luxxle::ReplaceAboutUIChromeURLs(::ChromeURLs(browser_context));
}
