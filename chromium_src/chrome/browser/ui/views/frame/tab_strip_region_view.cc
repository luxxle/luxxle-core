/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/tabs/luxxle_new_tab_button.h"
#include "luxxle/browser/ui/views/tabs/luxxle_tab_search_button.h"

// Currently, `NewTabButton::kButtonSize` is used to calculate tab strip button
// borders. Since the size of buttons varies depending upon the tabs update
// feature flag, replace the `kButtonSize` identifier with a call to the static
// `LuxxleNewTabButton::GetButtonSize` function.
#define kButtonSize GetButtonSize()

#define NewTabButton LuxxleNewTabButton

#include "src/chrome/browser/ui/views/frame/tab_strip_region_view.cc"

#undef NewTabButton
#undef kButtonSize
