/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_BROWSER_FRAME_H_
#define LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_BROWSER_FRAME_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "build/build_config.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "ui/color/color_provider_key.h"

class CustomThemeSupplier;

class LuxxleBrowserFrame : public BrowserFrame {
 public:
  explicit LuxxleBrowserFrame(BrowserView* browser_view);
  LuxxleBrowserFrame(const LuxxleBrowserFrame&) = delete;
  LuxxleBrowserFrame& operator=(const LuxxleBrowserFrame&) = delete;
  ~LuxxleBrowserFrame() override;

  const BrowserView* browser_view() const { return view_; }

  // BrowserFrame overrides:
#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC)
  const ui::NativeTheme* GetNativeTheme() const override;
#endif
  ui::ColorProviderKey::ThemeInitializerSupplier* GetCustomTheme()
      const override;
  views::internal::RootView* CreateRootView() override;
  void SetTabDragKind(TabDragKind kind) override;

 private:
  raw_ptr<BrowserView> view_ = nullptr;
  scoped_refptr<CustomThemeSupplier> theme_supplier_;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_BROWSER_FRAME_H_
