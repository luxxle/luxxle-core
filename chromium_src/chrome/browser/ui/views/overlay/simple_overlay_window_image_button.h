/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_OVERLAY_SIMPLE_OVERLAY_WINDOW_IMAGE_BUTTON_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_OVERLAY_SIMPLE_OVERLAY_WINDOW_IMAGE_BUTTON_H_

#define UpdateImage                                              \
  UpdateImage_Unused();                                          \
  void override_icon(const gfx::VectorIcon& icon) {              \
    *const_cast<raw_ref<const gfx::VectorIcon>*>(&icon_) = icon; \
  }                                                              \
  friend class LuxxleVideoOverlayWindowViews;                     \
  void UpdateImage

#include "src/chrome/browser/ui/views/overlay/simple_overlay_window_image_button.h"  // IWYU pragma: export

#undef UpdateImage

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_OVERLAY_SIMPLE_OVERLAY_WINDOW_IMAGE_BUTTON_H_
