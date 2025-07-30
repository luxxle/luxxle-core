/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/browser_commands.h"

#include "luxxle/components/commander/common/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/common/webui_url_constants.h"

#define ReloadBypassingCache ReloadBypassingCache_ChromiumImpl
#define GetReadingListModel GetReadingListModel_ChromiumImpl
#define kChromeUISplitViewNewTabPageURL kChromeUINewTabURL
#include "src/chrome/browser/ui/browser_commands.cc"
#undef kChromeUISplitViewNewTabPageURL
#undef ReloadBypassingCache
#undef GetReadingListModel

namespace chrome {

void ReloadBypassingCache(Browser* browser, WindowOpenDisposition disposition) {
  Profile* profile = browser->profile();
  DCHECK(profile);
  // NewTorConnectionForSite will do hard reload after obtaining new identity
  if (profile->IsTor()) {
    luxxle::NewTorConnectionForSite(browser);
  } else {
    ReloadBypassingCache_ChromiumImpl(browser, disposition);
  }
}

ReadingListModel* GetReadingListModel(Browser* browser) {
  return nullptr;
}

}  // namespace chrome
