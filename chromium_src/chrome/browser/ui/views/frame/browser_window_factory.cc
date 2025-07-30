/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/frame/luxxle_browser_frame.h"
#include "luxxle/browser/ui/views/frame/luxxle_browser_view.h"
#include "chrome/browser/ui/views/frame/browser_view.h"

#define BrowserFrame LuxxleBrowserFrame
#define BrowserView LuxxleBrowserView
#include "src/chrome/browser/ui/views/frame/browser_window_factory.cc"
#undef BrowserView
#undef BrowserFrame
