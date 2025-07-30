/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_PROFILES_LUXXLE_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_
#define LUXXLE_BROWSER_UI_VIEWS_PROFILES_LUXXLE_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_

#include "chrome/browser/ui/views/profiles/avatar_toolbar_button_delegate.h"

class LuxxleAvatarToolbarButtonDelegate final
    : public AvatarToolbarButtonDelegate {
 public:
  using AvatarToolbarButtonDelegate::AvatarToolbarButtonDelegate;
  LuxxleAvatarToolbarButtonDelegate(AvatarToolbarButton* button,
                                   Browser* browser);
  LuxxleAvatarToolbarButtonDelegate(const LuxxleAvatarToolbarButtonDelegate&) =
      delete;
  LuxxleAvatarToolbarButtonDelegate& operator=(
      const LuxxleAvatarToolbarButtonDelegate&) = delete;

  gfx::Image GetGaiaAccountImage() const;

  // Overrides from AvatarToolbarButtonDelegate:
  ui::ImageModel GetAvatarIcon(
      int icon_size,
      SkColor icon_color,
      const ui::ColorProvider* color_provider) const override;
  std::u16string GetAvatarTooltipText() const override;

 private:
  raw_ptr<Browser> browser_;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_PROFILES_LUXXLE_AVATAR_TOOLBAR_BUTTON_DELEGATE_H_
