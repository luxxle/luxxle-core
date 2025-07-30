/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_OPAQUE_BROWSER_FRAME_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_OPAQUE_BROWSER_FRAME_VIEW_H_

#include <memory>

#include "chrome/browser/ui/views/frame/opaque_browser_frame_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

class LuxxleWindowFrameGraphic;

class LuxxleOpaqueBrowserFrameView : public OpaqueBrowserFrameView {
  METADATA_HEADER(LuxxleOpaqueBrowserFrameView, OpaqueBrowserFrameView)
 public:
  LuxxleOpaqueBrowserFrameView(BrowserFrame* frame,
                              BrowserView* browser_view,
                              OpaqueBrowserFrameViewLayout* layout);
  ~LuxxleOpaqueBrowserFrameView() override;

  LuxxleOpaqueBrowserFrameView(const LuxxleOpaqueBrowserFrameView&) = delete;
  LuxxleOpaqueBrowserFrameView& operator=(
      const LuxxleOpaqueBrowserFrameView&) = delete;

  // OpaqueBrowserFrameView overrides:
  void OnPaint(gfx::Canvas* canvas) override;
  int NonClientHitTest(const gfx::Point& point) override;
  void UpdateCaptionButtonPlaceholderContainerBackground() override;
  void PaintClientEdge(gfx::Canvas* canvas) const override;
  int GetTopInset(bool restored) const override;
  int GetTopAreaHeight() const override;

 private:
  bool ShouldShowVerticalTabs() const;
  std::unique_ptr<LuxxleWindowFrameGraphic> frame_graphic_;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_OPAQUE_BROWSER_FRAME_VIEW_H_
