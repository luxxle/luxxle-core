/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_SYSTEM_MENU_MODEL_DELEGATE_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_SYSTEM_MENU_MODEL_DELEGATE_H_

#include "ui/menus/simple_menu_model.h"

#define IsCommandIdChecked                               \
  IsCommandIdChecked_ChromiumImpl(int command_id) const; \
  bool IsCommandIdChecked

#include "src/chrome/browser/ui/views/frame/system_menu_model_delegate.h"  // IWYU pragma: export

#undef IsCommandIdChecked

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_FRAME_SYSTEM_MENU_MODEL_DELEGATE_H_
