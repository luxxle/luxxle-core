/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/profiles/luxxle_avatar_toolbar_button_delegate.h"

#include "luxxle/app/vector_icons/vector_icons.h"
#include "luxxle/browser/ui/color/color_palette.h"
#include "luxxle/components/vector_icons/vector_icons.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/app/vector_icons/vector_icons.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/layout_constants.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/models/image_model.h"
#include "ui/color/color_provider.h"

LuxxleAvatarToolbarButtonDelegate::LuxxleAvatarToolbarButtonDelegate(
    AvatarToolbarButton* button,
    Browser* browser)
    : AvatarToolbarButtonDelegate(button, browser), browser_(browser) {}

gfx::Image LuxxleAvatarToolbarButtonDelegate::GetGaiaAccountImage() const {
  return gfx::Image();
}

ui::ImageModel LuxxleAvatarToolbarButtonDelegate::GetAvatarIcon(
    int icon_size,
    SkColor icon_color,
    const ui::ColorProvider* color_provider) const {
  const auto location_bar_icon_size = GetLayoutConstant(LOCATION_BAR_ICON_SIZE);
  if (browser_->profile()->IsTor()) {
    return ui::ImageModel::FromVectorIcon(kLeoProductTorIcon,
                                          SkColorSetRGB(0x3C, 0x82, 0x3C),
                                          location_bar_icon_size);
  }

  if (browser_->profile()->IsIncognitoProfile()) {
    return ui::ImageModel::FromVectorIcon(
        kIncognitoIcon, SkColorSetRGB(0xFF, 0xFF, 0xFF), icon_size);
  }

  if (browser_->profile()->IsGuestSession()) {
    return ui::ImageModel::FromVectorIcon(kUserMenuGuestIcon, icon_color,
                                          location_bar_icon_size);
  }

  return AvatarToolbarButtonDelegate::GetAvatarIcon(location_bar_icon_size,
                                                    icon_color, color_provider);
}

std::u16string LuxxleAvatarToolbarButtonDelegate::GetAvatarTooltipText() const {
  if (browser_->profile()->IsTor()) {
    return l10n_util::GetStringUTF16(IDS_TOR_AVATAR_BUTTON_TOOLTIP_TEXT);
  }

  return AvatarToolbarButtonDelegate::GetAvatarTooltipText();
}
