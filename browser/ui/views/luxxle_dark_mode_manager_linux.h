/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_DARK_MODE_MANAGER_LINUX_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_DARK_MODE_MANAGER_LINUX_H_

#include "chrome/browser/ui/views/dark_mode_manager_linux.h"

namespace ui {

class LuxxleDarkModeManagerLinux : public DarkModeManagerLinux {
 public:
  LuxxleDarkModeManagerLinux();
  ~LuxxleDarkModeManagerLinux() override;

 private:
  // DarkModeManagerLinux overrides:
  void SetColorScheme(bool prefer_dark_theme, bool from_toolkit_theme) override;
};

}  // namespace ui

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_DARK_MODE_MANAGER_LINUX_H_
