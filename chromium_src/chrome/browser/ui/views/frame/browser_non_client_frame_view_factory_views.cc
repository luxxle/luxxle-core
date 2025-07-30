/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <functional>
#include <type_traits>

#include "build/build_config.h"

#if BUILDFLAG(IS_WIN)
#include "luxxle/browser/ui/views/frame/luxxle_browser_frame_view_win.h"
#define BrowserFrameViewWin LuxxleBrowserFrameViewWin
#endif

// This file is included for all platform by upstream source code and we need
// to include this first to avoid #define conflicts.
#include "chrome/browser/ui/views/frame/browser_frame_view_linux.h"

#include "luxxle/browser/ui/views/frame/luxxle_opaque_browser_frame_view.h"
#define OpaqueBrowserFrameView LuxxleOpaqueBrowserFrameView

#if BUILDFLAG(IS_LINUX)
#include "luxxle/browser/ui/views/frame/luxxle_browser_frame_view_linux_native.h"
#define BrowserFrameViewLinuxNative LuxxleBrowserFrameViewLinuxNative
#endif  // BUILDFLAG(IS_LINUX)

#include "src/chrome/browser/ui/views/frame/browser_non_client_frame_view_factory_views.cc"

#if BUILDFLAG(IS_LINUX)
#undef BrowserFrameViewLinuxNative
#endif  // BUILDFLAG(IS_LINUX)

#undef OpaqueBrowserFrameView

// A sanity check for our macro
static_assert(
    std::is_same_v<std::unique_ptr<LuxxleOpaqueBrowserFrameView>,
                   decltype(std::function{
                       chrome::CreateOpaqueBrowserFrameView})::result_type>,
    "CreateOpaqueBrowserFrameView is not returning "
    "LuxxleOpaqueBrowserFrameView");

#if BUILDFLAG(IS_WIN)
#undef BrowserFrameViewWin
#endif
