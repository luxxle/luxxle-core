/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/tabs/luxxle_tab_color_mixer.h"

#include "base/containers/fixed_flat_map.h"
#include "luxxle/browser/ui/color/luxxle_color_id.h"
#include "luxxle/ui/color/nala/nala_color_id.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "ui/color/color_mixer.h"
#include "ui/color/color_provider.h"
#include "ui/color/color_provider_key.h"
#include "ui/color/color_recipe.h"
#include "ui/color/color_transform.h"

namespace tabs {

void AddLuxxleTabThemeColorMixer(ui::ColorProvider* provider,
                                const ui::ColorProviderKey& key) {
  auto& mixer = provider->AddMixer();

  if (key.custom_theme) {
    mixer[kColorLuxxleSplitViewTileBackgroundHorizontal] = {
        SkColorSetARGB(0x1A, 0x00, 0x00, 0x00)};
    mixer[kColorLuxxleSplitViewTileBackgroundVertical] = {
        kColorLuxxleSplitViewTileBackgroundHorizontal};
    mixer[kColorLuxxleSplitViewTileBackgroundBorder] = {
        SkColorSetARGB(0x34, 0xFF, 0xFF, 0xFF)};
    mixer[kColorLuxxleSplitViewTileDivider] = {kColorTabDividerFrameActive};
    mixer[kColorLuxxleVerticalTabActiveBackground] = {
        kColorTabBackgroundInactiveFrameActive};
    mixer[kColorLuxxleVerticalTabHoveredBackground] = {
        ui::AlphaBlend(kColorLuxxleVerticalTabActiveBackground,
                       kColorLuxxleVerticalTabInactiveBackground,
                       /* 40% opacity */ 0.4 * SK_AlphaOPAQUE)};
  } else {
    mixer[kColorLuxxleSplitViewTileBackgroundHorizontal] = {
        nala::kColorDesktopbrowserTabbarSplitViewBackgroundHorizontal};
    mixer[kColorLuxxleSplitViewTileBackgroundVertical] = {
        nala::kColorDesktopbrowserTabbarSplitViewBackgroundVertical};
    mixer[kColorLuxxleSplitViewTileBackgroundBorder] = {SK_ColorTRANSPARENT};
    mixer[kColorLuxxleSplitViewTileDivider] = {
        nala::kColorDesktopbrowserTabbarSplitViewDivider};
    mixer[kColorLuxxleVerticalTabActiveBackground] = {
        nala::kColorDesktopbrowserTabbarActiveTabVertical};
    mixer[kColorLuxxleVerticalTabHoveredBackground] = {
        nala::kColorDesktopbrowserTabbarHoverTabVertical};
  }

  mixer[kColorLuxxleVerticalTabInactiveBackground] = {kColorToolbar};
  mixer[kColorLuxxleVerticalTabSeparator] = {
      nala::kColorDesktopbrowserToolbarButtonOutline};
  mixer[kColorLuxxleVerticalTabNTBIconColor] = {
      kColorTabForegroundInactiveFrameActive};
  mixer[kColorLuxxleVerticalTabNTBTextColor] = {
      kColorTabForegroundInactiveFrameActive};
  mixer[kColorLuxxleVerticalTabNTBShortcutTextColor] = {
      kColorTabForegroundActiveFrameActive};
  mixer[kColorLuxxleSplitViewMenuItemIcon] = {nala::kColorIconDefault};
  mixer[kColorLuxxleSplitViewUrl] = {nala::kColorTextTertiary};
  mixer[kColorLuxxleSplitViewMenuButtonBorder] = {nala::kColorDividerSubtle};
  mixer[kColorLuxxleSplitViewActiveWebViewBorder] = {
      nala::kColorPrimitiveBrandsRorange1};
  mixer[kColorLuxxleSplitViewMenuButtonBackground] = {
      nala::kColorContainerBackground};
  mixer[kColorLuxxleSplitViewMenuButtonIcon] = {nala::kColorIconInteractive};

  mixer[kColorLuxxleSharedPinnedTabDummyViewThumbnailBorder] = {
      nala::kColorDividerSubtle};
  mixer[kColorLuxxleSharedPinnedTabDummyViewDescription] = {
      nala::kColorTextSecondary};
  mixer[kColorLuxxleSharedPinnedTabDummyViewTitle] = {nala::kColorTextPrimary};
  mixer[kColorLuxxleSharedPinnedTabDummyViewBackground] = {
      nala::kColorContainerBackground};
  mixer[kColorLuxxleSplitViewInactiveWebViewBorder] = {
      nala::kColorDesktopbrowserToolbarButtonOutline};
}

void AddLuxxleTabPrivateThemeColorMixer(ui::ColorProvider* provider,
                                       const ui::ColorProviderKey& key) {
  auto& mixer = provider->AddMixer();
  mixer[kColorLuxxleVerticalTabActiveBackground] = {
      nala::kColorPrimitivePrivateWindow30};
  mixer[kColorLuxxleVerticalTabInactiveBackground] = {
      mixer.GetResultColor(kColorToolbar)};
  mixer[kColorLuxxleVerticalTabHoveredBackground] = {
      nala::kColorPrimitivePrivateWindow15};
  mixer[kColorLuxxleSplitViewTileBackgroundHorizontal] = {
      nala::kColorPrimitivePrivateWindow10};
  mixer[kColorLuxxleSplitViewTileBackgroundVertical] = {
      nala::kColorPrimitivePrivateWindow5};
  mixer[kColorLuxxleSplitViewTileDivider] = {
      nala::kColorPrimitivePrivateWindow20};
  mixer[kColorLuxxleSplitViewTileBackgroundBorder] = {SK_ColorTRANSPARENT};
}

void AddLuxxleTabTorThemeColorMixer(ui::ColorProvider* provider,
                                   const ui::ColorProviderKey& key) {
  auto& mixer = provider->AddMixer();
  mixer[kColorLuxxleVerticalTabActiveBackground] = {
      nala::kColorPrimitiveTorWindow30};
  mixer[kColorLuxxleVerticalTabInactiveBackground] = {
      mixer.GetResultColor(kColorToolbar)};
  mixer[kColorLuxxleVerticalTabHoveredBackground] = {
      nala::kColorPrimitiveTorWindow15};
  mixer[kColorLuxxleSplitViewTileBackgroundHorizontal] = {
      nala::kColorPrimitiveTorWindow10};
  mixer[kColorLuxxleSplitViewTileBackgroundVertical] = {
      nala::kColorPrimitiveTorWindow5};
  mixer[kColorLuxxleSplitViewTileDivider] = {nala::kColorPrimitiveTorWindow20};
  mixer[kColorLuxxleSplitViewTileBackgroundBorder] = {SK_ColorTRANSPARENT};
}

}  // namespace tabs
