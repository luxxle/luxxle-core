/* Copyright 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_WINDOW_FRAME_GRAPHIC_H_
#define LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_WINDOW_FRAME_GRAPHIC_H_

namespace gfx {
class Canvas;
class Rect;
}  // namespace gfx

namespace content {
class BrowserContext;
}  // namespace content

class LuxxleWindowFrameGraphic {
 public:
  explicit LuxxleWindowFrameGraphic(content::BrowserContext* context);
  virtual ~LuxxleWindowFrameGraphic();

  LuxxleWindowFrameGraphic(const LuxxleWindowFrameGraphic&) = delete;
  LuxxleWindowFrameGraphic& operator=(const LuxxleWindowFrameGraphic&) = delete;

  void Paint(gfx::Canvas* canvas, const gfx::Rect& frame_bounds);

 private:
  const bool is_tor_window_;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_WINDOW_FRAME_GRAPHIC_H_
