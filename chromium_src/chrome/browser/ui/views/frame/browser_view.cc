/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Include the corresponding header first since it defines the same macros and
// therefore avoid undef before use.
#include "chrome/browser/ui/views/frame/browser_view.h"

#include "luxxle/browser/ui/views/luxxle_tab_search_bubble_host.h"
#include "luxxle/browser/ui/views/frame/luxxle_browser_view_layout.h"
#include "luxxle/browser/ui/views/frame/luxxle_tab_strip_region_view.h"
#include "luxxle/browser/ui/views/frame/split_view/luxxle_multi_contents_view.h"
#include "luxxle/browser/ui/views/infobars/luxxle_infobar_container_view.h"
#include "luxxle/browser/ui/views/side_panel/luxxle_side_panel.h"
#include "luxxle/browser/ui/views/side_panel/luxxle_side_panel_coordinator.h"
#include "luxxle/browser/ui/views/tabs/luxxle_browser_tab_strip_controller.h"
#include "luxxle/browser/ui/views/tabs/luxxle_tab_strip.h"
#include "luxxle/browser/ui/views/tabs/vertical_tab_utils.h"
#include "luxxle/browser/ui/views/toolbar/luxxle_toolbar_view.h"
#include "luxxle/components/constants/pref_names.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/views/frame/browser_view_layout.h"
#include "chrome/browser/ui/views/side_panel/side_panel.h"
#include "chrome/browser/ui/web_applications/app_browser_controller.h"

#define InfoBarContainerView LuxxleInfoBarContainerView
#define BrowserViewLayout LuxxleBrowserViewLayout
#define ToolbarView LuxxleToolbarView
#define BrowserTabStripController LuxxleBrowserTabStripController
#define TabStrip LuxxleTabStrip
#define TabStripRegionView LuxxleTabStripRegionView
#define BookmarkBarView LuxxleBookmarkBarView
#define UpdateExclusiveAccessBubble UpdateExclusiveAccessBubble_ChromiumImpl
#define MultiContentsView LuxxleMultiContentsView

#define LUXXLE_BROWSER_VIEW_LAYOUT_CONVERTED_HIT_TEST \
  if (dst->GetWidget() != src->GetWidget()) {        \
    return false;                                    \
  }

#include "src/chrome/browser/ui/views/frame/browser_view.cc"

#undef MultiContentsView
#undef UpdateExclusiveAccessBubble
#undef BookmarkBarView
#undef TabStripRegionView
#undef TabStrip
#undef BrowserTabStripController
#undef ToolbarView
#undef BrowserViewLayout
#undef InfoBarContainerView
#undef LUXXLE_BROWSER_VIEW_LAYOUT_CONVERTED_HIT_TEST

views::View* BrowserView::GetContentsContainerForLayoutManager() {
  return contents_container();
}

void BrowserView::SetNativeWindowPropertyForWidget(views::Widget* widget) {
  // Sets a kBrowserWindowKey to given child |widget| so that we can get
  // BrowserView from the |widget|.
  DCHECK(GetWidget());
  DCHECK_EQ(GetWidget(), widget->GetTopLevelWidget())
      << "The |widget| should be child of BrowserView's widget.";

  widget->SetNativeWindowProperty(kBrowserViewKey, this);
}

void BrowserView::UpdateExclusiveAccessBubble(
    const ExclusiveAccessBubbleParams& params,
    ExclusiveAccessBubbleHideCallback first_hide_callback) {
  // Show/Hide full screen reminder bubble based on our settings preference
  // for tab-initiated ones.
  if (!GetProfile()->GetPrefs()->GetBoolean(kShowFullscreenReminder) &&
      params.type == EXCLUSIVE_ACCESS_BUBBLE_TYPE_FULLSCREEN_EXIT_INSTRUCTION) {
    return;
  }

  UpdateExclusiveAccessBubble_ChromiumImpl(params,
                                           std::move(first_hide_callback));
}
