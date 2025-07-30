/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_RENDERER_CONTEXT_MENU_LUXXLE_RENDER_VIEW_CONTEXT_MENU_VIEWS_H_
#define LUXXLE_BROWSER_UI_VIEWS_RENDERER_CONTEXT_MENU_LUXXLE_RENDER_VIEW_CONTEXT_MENU_VIEWS_H_

#include "chrome/browser/ui/views/renderer_context_menu/render_view_context_menu_views.h"

class LuxxleRenderViewContextMenuViews : public RenderViewContextMenuViews {
 public:
  ~LuxxleRenderViewContextMenuViews() override;
  LuxxleRenderViewContextMenuViews(const LuxxleRenderViewContextMenuViews&) =
      delete;
  LuxxleRenderViewContextMenuViews& operator=(
      const LuxxleRenderViewContextMenuViews&) = delete;

  // Factory function to create an instance.
  static RenderViewContextMenuViews* Create(
      // Non-const reference passed in the parent class upstream
      // NOLINTNEXTLINE(runtime/references)
      content::RenderFrameHost& render_frame_host,
      const content::ContextMenuParams& params);

  void Show() override;

 protected:
  // Non-const reference passed in the parent class upstream
  // NOLINTNEXTLINE(runtime/references)
  LuxxleRenderViewContextMenuViews(content::RenderFrameHost& render_frame_host,
                                  const content::ContextMenuParams& params);
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_RENDERER_CONTEXT_MENU_LUXXLE_RENDER_VIEW_CONTEXT_MENU_VIEWS_H_
