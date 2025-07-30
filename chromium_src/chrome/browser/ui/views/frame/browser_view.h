/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_

#include "luxxle/browser/ui/luxxle_browser_window.h"
#include "luxxle/browser/ui/views/bookmarks/luxxle_bookmark_bar_view.h"
#include "luxxle/browser/ui/views/frame/luxxle_browser_view_layout.h"
#include "luxxle/browser/ui/views/side_panel/luxxle_side_panel.h"
#include "build/build_config.h"
#include "chrome/browser/ui/exclusive_access/exclusive_access_context.h"
#include "chrome/browser/ui/views/side_panel/side_panel.h"

#define BrowserViewLayoutDelegateImpl \
  BrowserViewLayoutDelegateImpl;      \
  friend class LuxxleBrowserView;      \
  void SetNativeWindowPropertyForWidget(views::Widget* widget)
#define BrowserWindow LuxxleBrowserWindow
#define BrowserViewLayout LuxxleBrowserViewLayout
#define SidePanel LuxxleSidePanel
#define BookmarkBarView LuxxleBookmarkBarView

#define MaybeShowReadingListInSidePanelIPH \
  virtual MaybeShowReadingListInSidePanelIPH

#define UpdateDevToolsForContents virtual UpdateDevToolsForContents
#define GetTabStripVisible virtual GetTabStripVisible

#define GetTabSearchBubbleHost     \
  GetTabSearchBubbleHost_Unused(); \
  virtual TabSearchBubbleHost* GetTabSearchBubbleHost

#define UpdateExclusiveAccessBubble                            \
  UpdateExclusiveAccessBubble_ChromiumImpl(                    \
      const ExclusiveAccessBubbleParams& params,               \
      ExclusiveAccessBubbleHideCallback first_hide_callback);  \
  virtual views::View* GetContentsContainerForLayoutManager(); \
  void UpdateExclusiveAccessBubble

#if BUILDFLAG(IS_WIN)
#define GetSupportsTitle virtual GetSupportsTitle

// On Windows <winuser.h> defines LoadAccelerators
// Using push_macro seems to be causing #undef not to work in Chromium 125.
// Unclear what causes this.
// #pragma push_macro("LoadAccelerators")
#undef LoadAccelerators
#endif
#define LoadAccelerators virtual LoadAccelerators
#define ShowSplitView virtual ShowSplitView
#define HideSplitView virtual HideSplitView

#include "src/chrome/browser/ui/views/frame/browser_view.h"  // IWYU pragma: export

#undef HideSplitView
#undef ShowSplitView
#undef LoadAccelerators
#if BUILDFLAG(IS_WIN)
// #pragma pop_macro("LoadAccelerators")
#undef GetSupportsTitle
#endif

#undef UpdateExclusiveAccessBubble
#undef GetTabSearchBubbleHost
#undef GetTabStripVisible
#undef UpdateDevToolsForContents
#undef MaybeShowReadingListInSidePanelIPH
#undef BookmarkBarView
#undef SidePanel
#undef BrowserViewLayout
#undef BrowserWindow
#undef BrowserViewLayoutDelegateImpl

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_BROWSER_VIEW_H_
