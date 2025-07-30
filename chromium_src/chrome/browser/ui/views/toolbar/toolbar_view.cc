/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/profiles/luxxle_avatar_toolbar_button.h"
#include "luxxle/browser/ui/views/toolbar/luxxle_browser_app_menu_button.h"
#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "luxxle/browser/ui/views/location_bar/luxxle_location_bar_view.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"

#define LocationBarView LuxxleLocationBarView
#endif

#define BrowserAppMenuButton LuxxleBrowserAppMenuButton
#define LUXXLE_TOOLBAR_VIEW_INIT                                   \
  avatar_ = container_view_->AddChildView(                        \
      std::make_unique<LuxxleAvatarToolbarButton>(browser_view_)); \
  if (false)

#include "src/chrome/browser/ui/views/toolbar/toolbar_view.cc"
#undef LUXXLE_TOOLBAR_VIEW_INIT
#undef BrowserAppMenuButton
#if BUILDFLAG(ENABLE_EXTENSIONS)
#undef LocationBarView
#endif
