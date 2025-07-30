/* Copyright (c) 2018 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/luxxle_browser_content_setting_bubble_model_delegate.h"

#include "luxxle/components/constants/url_constants.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_tabstrip.h"

constexpr char kLuxxleCommunitySupportUrl[] = "https://community.luxxle.com/";

LuxxleBrowserContentSettingBubbleModelDelegate::
LuxxleBrowserContentSettingBubbleModelDelegate(Browser* browser) :
    BrowserContentSettingBubbleModelDelegate(browser),
    browser_(browser) {
}

LuxxleBrowserContentSettingBubbleModelDelegate::
    ~LuxxleBrowserContentSettingBubbleModelDelegate() = default;

void
LuxxleBrowserContentSettingBubbleModelDelegate::ShowWidevineLearnMorePage() {
  GURL learn_more_url = GURL(kWidevineTOS);
  chrome::AddSelectedTabWithURL(browser_, learn_more_url,
                                ui::PAGE_TRANSITION_LINK);
}

void LuxxleBrowserContentSettingBubbleModelDelegate::ShowLearnMorePage(
    ContentSettingsType type) {
  // TODO(yrliou): Use specific support pages for each content setting type
  GURL learn_more_url(kLuxxleCommunitySupportUrl);
  chrome::AddSelectedTabWithURL(browser_, learn_more_url,
                                ui::PAGE_TRANSITION_LINK);
}
