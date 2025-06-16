/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_BRAVE_ADS_INSETS_UTIL_H_
#define LUXXLE_BROWSER_UI_VIEWS_BRAVE_ADS_INSETS_UTIL_H_

namespace gfx {
class FontList;
class Insets;
}  // namespace gfx

namespace brave_ads {

void AdjustInsetsForFontList(gfx::Insets* insets,
                             const gfx::FontList& font_list);

}  // namespace brave_ads

#endif  // LUXXLE_BROWSER_UI_VIEWS_BRAVE_ADS_INSETS_UTIL_H_
