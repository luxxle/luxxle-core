/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_WIN_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_WIN_H_

#define client_view_bounds_ \
  client_view_bounds_;      \
  friend class LuxxleBrowserFrameViewWin
#define ShouldShowWindowTitle virtual ShouldShowWindowTitle
#define LayoutCaptionButtons virtual LayoutCaptionButtons

#include "src/chrome/browser/ui/views/frame/browser_frame_view_win.h"  // IWYU pragma: export

#undef LayoutCaptionButtons
#undef ShouldShowWindowTitle
#undef client_view_bounds_

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_FRAME_VIEW_WIN_H_
