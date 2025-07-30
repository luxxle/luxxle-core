/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_COLOR_MIXER_H_
#define LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_COLOR_MIXER_H_

#include "ui/color/color_provider_key.h"

namespace tabs {

// Called from
// //luxxle/chromium_src/chrome/browser/ui/color/tab_strip_color_mixer.cc
void AddLuxxleTabThemeColorMixer(ui::ColorProvider* provider,
                                const ui::ColorProviderKey& key);

// Called from
// //luxxle/browser/themes/luxxle_private_window_theme_supplier.cc
void AddLuxxleTabPrivateThemeColorMixer(ui::ColorProvider* provider,
                                       const ui::ColorProviderKey& key);
void AddLuxxleTabTorThemeColorMixer(ui::ColorProvider* provider,
                                   const ui::ColorProviderKey& key);

}  // namespace tabs

#endif  // LUXXLE_BROWSER_UI_TABS_LUXXLE_TAB_COLOR_MIXER_H_
