/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/frame/luxxle_browser_frame.h"

#include "luxxle/browser/themes/luxxle_private_window_theme_supplier.h"
#include "luxxle/browser/ui/tabs/features.h"
#include "luxxle/browser/ui/tabs/shared_pinned_tab_service.h"
#include "luxxle/browser/ui/tabs/shared_pinned_tab_service_factory.h"
#include "luxxle/browser/ui/views/frame/luxxle_browser_root_view.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_frame.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/color/color_provider_key.h"

LuxxleBrowserFrame::LuxxleBrowserFrame(BrowserView* browser_view)
    : BrowserFrame(browser_view), view_(browser_view) {
  if (view_->browser()->profile()->IsIncognitoProfile() ||
      view_->browser()->profile()->IsTor() ||
      view_->browser()->profile()->IsGuestSession()) {
    theme_supplier_ = base::MakeRefCounted<LuxxlePrivateWindowThemeSupplier>(
        !view_->browser()->profile()->IsTor());
  }
}

LuxxleBrowserFrame::~LuxxleBrowserFrame() = default;

#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC)
// Tor/Guest profile should use DarkAura. If not, their native ui is affected by
// normal windows theme change.
const ui::NativeTheme* LuxxleBrowserFrame::GetNativeTheme() const {
  if ((view_->browser()->profile()->IsIncognitoProfile() ||
       view_->browser()->profile()->IsTor() ||
       view_->browser()->profile()->IsGuestSession()) &&
      ThemeServiceFactory::GetForProfile(view_->browser()->profile())
          ->UsingDefaultTheme()) {
    return ui::NativeTheme::GetInstanceForDarkUI();
  }
  return views::Widget::GetNativeTheme();
}
#endif

ui::ColorProviderKey::ThemeInitializerSupplier*
LuxxleBrowserFrame::GetCustomTheme() const {
  // To provider private(tor) windows's theme color via color provider,
  // we use |theme_supplier_| for both as upstream doesn't use separated
  // mix for private window.
  if (theme_supplier_) {
    return theme_supplier_.get();
  }

  return BrowserFrame::GetCustomTheme();
}

views::internal::RootView* LuxxleBrowserFrame::CreateRootView() {
  root_view_ = new LuxxleBrowserRootView(browser_view_, this);
  return root_view_;
}

void LuxxleBrowserFrame::SetTabDragKind(TabDragKind kind) {
  const bool should_sync_shared_pinned_tab =
      base::FeatureList::IsEnabled(tabs::features::kLuxxleSharedPinnedTabs) &&
      tab_drag_kind_ == TabDragKind::kAllTabs && kind == TabDragKind::kNone;

  BrowserFrame::SetTabDragKind(kind);

  if (should_sync_shared_pinned_tab) {
    SharedPinnedTabServiceFactory::GetForProfile(browser_view_->GetProfile())
        ->TabDraggingEnded(browser_view_->browser());
  }
}
