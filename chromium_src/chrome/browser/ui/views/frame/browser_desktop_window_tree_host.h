/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_DESKTOP_WINDOW_TREE_HOST_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_DESKTOP_WINDOW_TREE_HOST_H_

#define CreateBrowserDesktopWindowTreeHost(...)    \
  CreateBrowserDesktopWindowTreeHost(__VA_ARGS__); \
  static BrowserDesktopWindowTreeHost*             \
  CreateBrowserDesktopWindowTreeHost_Unused(__VA_ARGS__)

#include "src/chrome/browser/ui/views/frame/browser_desktop_window_tree_host.h"  // IWYU pragma: export

#undef CreateBrowserDesktopWindowTreeHost

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_DESKTOP_WINDOW_TREE_HOST_H_
