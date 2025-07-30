/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/luxxle_browser.h"
#include "luxxle/browser/ui/luxxle_browser_actions.h"
#include "luxxle/browser/ui/luxxle_browser_command_controller.h"
#include "luxxle/browser/ui/luxxle_browser_content_setting_bubble_model_delegate.h"
#include "luxxle/browser/ui/luxxle_tab_strip_model_delegate.h"
#include "luxxle/browser/ui/tabs/luxxle_tab_strip_model.h"
#include "luxxle/browser/ui/toolbar/luxxle_location_bar_model_delegate.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"

#if !BUILDFLAG(IS_ANDROID)
#include "luxxle/browser/ui/bookmark/luxxle_bookmark_tab_helper.h"
#endif

#define LUXXLE_BROWSER_CREATE return new LuxxleBrowser(params);
#define BrowserContentSettingBubbleModelDelegate \
  LuxxleBrowserContentSettingBubbleModelDelegate
#define BrowserCommandController LuxxleBrowserCommandController
#define BrowserLocationBarModelDelegate LuxxleLocationBarModelDelegate
#if !BUILDFLAG(IS_ANDROID)
#define BookmarkTabHelper LuxxleBookmarkTabHelper
#endif
#define BrowserTabStripModelDelegate LuxxleTabStripModelDelegate
#define BrowserActions(...) LuxxleBrowserActions(__VA_ARGS__)

#include "src/chrome/browser/ui/browser.cc"

#undef BrowserActions
#undef BrowserTabStripModelDelegate
#undef BrowserLocationBarModelDelegate
#undef BrowserContentSettingBubbleModelDelegate
#undef BrowserCommandController
#undef LUXXLE_BROWSER_CREATE

#if !BUILDFLAG(IS_ANDROID)
#undef BookmarkTabHelper
#endif
