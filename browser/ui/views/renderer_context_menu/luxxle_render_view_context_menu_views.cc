/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/renderer_context_menu/luxxle_render_view_context_menu_views.h"

LuxxleRenderViewContextMenuViews::LuxxleRenderViewContextMenuViews(
    content::RenderFrameHost& render_frame_host,
    const content::ContextMenuParams& params)
    : RenderViewContextMenuViews(render_frame_host, params) {}

LuxxleRenderViewContextMenuViews::~LuxxleRenderViewContextMenuViews() = default;

// static
RenderViewContextMenuViews* LuxxleRenderViewContextMenuViews::Create(
    content::RenderFrameHost& render_frame_host,
    const content::ContextMenuParams& params) {
  return new LuxxleRenderViewContextMenuViews(render_frame_host, params);
}

void LuxxleRenderViewContextMenuViews::Show() {
  // Removes duplicated separator if any. The duplicated separator may appear
  // in |LuxxleRenderViewContextMenu::InitMenu| after remove the translate menu
  // item.
  RemoveAdjacentSeparators();
  RenderViewContextMenuViews::Show();
}
