/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/side_panel/luxxle_side_panel_view_base.h"

#include "luxxle/browser/ui/color/luxxle_color_id.h"
#include "chrome/browser/ui/views/side_panel/side_panel_content_proxy.h"
#include "chrome/browser/ui/views/side_panel/side_panel_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/views/background.h"

LuxxleSidePanelViewBase::LuxxleSidePanelViewBase() {
  // Originally SidePanelEntry's Content was
  // [ReadLater|Bookmarks]SidePanelWebView and it's availability is set to true
  // when SidePanelWebUIView::ShowUI() and then proxy's availability callback is
  // executed. However, we use parent view(LuxxleReadLaterSidePanelView) to have
  // panel specific header view and this class becomes SidePanelEntry's Content.
  // To make this content available when SidePanelWebUIVew::ShowUI() is called,
  // this observes WebView's visibility because it's set as visible when
  // ShowUI() is called.
  // NOTE: If we use our own reading list page and it has loading spinner, maybe
  // we can set `true` here.
  SidePanelUtil::GetSidePanelContentProxy(this)->SetAvailable(false);
  SetBackground(
      views::CreateSolidBackground(kColorSidebarPanelHeaderBackground));
}

LuxxleSidePanelViewBase::~LuxxleSidePanelViewBase() = default;

void LuxxleSidePanelViewBase::StartObservingWebWebViewVisibilityChange(
    views::View* web_view) {
  view_observation_.Observe(web_view);
}

void LuxxleSidePanelViewBase::OnViewVisibilityChanged(
    views::View* observed_view,
    views::View* starting_view) {
  // Once it becomes available, stop observing becuase its availablity is
  // not changed from now on.
  if (observed_view->GetVisible()) {
    SidePanelUtil::GetSidePanelContentProxy(this)->SetAvailable(true);
    view_observation_.Reset();
  }
}

BEGIN_METADATA(LuxxleSidePanelViewBase)
END_METADATA
