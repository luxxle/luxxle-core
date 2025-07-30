/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_LINUX_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_LINUX_H_

#include "chrome/browser/ui/views/frame/opaque_browser_frame_view.h"

// Make BrowserFrameViewLinux extends our LuxxleOpaqueBrowserFrameView
#include "luxxle/browser/ui/views/frame/luxxle_opaque_browser_frame_view.h"
#define OpaqueBrowserFrameView LuxxleOpaqueBrowserFrameView

#include "src/chrome/browser/ui/views/frame/browser_frame_view_linux.h"  // IWYU pragma: export

#undef OpaqueBrowserFrameView

// Sanity check at compile time.
static_assert(
    std::is_base_of_v<LuxxleOpaqueBrowserFrameView, BrowserFrameViewLinux>,
    "BrowserFrameViewLinux should be a child of LuxxleOpaqueBrowserFrameView");

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_LINUX_H_
