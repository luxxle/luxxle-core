/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_VIEWS_DELEGATE_LINUX_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_VIEWS_DELEGATE_LINUX_H_

#include "chrome/browser/ui/views/chrome_views_delegate.h"

class LuxxleViewsDelegateLinux : public ChromeViewsDelegate {
 public:
  LuxxleViewsDelegateLinux() = default;
  LuxxleViewsDelegateLinux(const LuxxleViewsDelegateLinux&) = delete;
  LuxxleViewsDelegateLinux& operator=(const LuxxleViewsDelegateLinux&) = delete;
  ~LuxxleViewsDelegateLinux() override = default;
 private:
  // ChromeViewsDelegate overrides:
  gfx::ImageSkia* GetDefaultWindowIcon() const override;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_VIEWS_DELEGATE_LINUX_H_
