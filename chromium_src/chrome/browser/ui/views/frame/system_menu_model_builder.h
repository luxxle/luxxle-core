/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_SYSTEM_MENU_MODEL_BUILDER_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_SYSTEM_MENU_MODEL_BUILDER_H_

#include "chrome/browser/ui/views/frame/system_menu_model_delegate.h"

#define Init                                \
  UnUsed();                                 \
                                            \
 private:                                   \
  friend class LuxxleSystemMenuModelBuilder; \
                                            \
 public:                                    \
  void Init

#define BuildSystemMenuForBrowserWindow virtual BuildSystemMenuForBrowserWindow

#include "src/chrome/browser/ui/views/frame/system_menu_model_builder.h"  // IWYU pragma: export

#undef BuildSystemMenuForBrowserWindow
#undef Init

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_SYSTEM_MENU_MODEL_BUILDER_H_
