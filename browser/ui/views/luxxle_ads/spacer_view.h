/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ADS_SPACER_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ADS_SPACER_VIEW_H_

namespace views {
class View;
}  // namespace views

namespace luxxle_ads {

views::View* CreateFlexibleSpacerView(int spacing);
views::View* CreateFixedSizeSpacerView(int spacing);

}  // namespace luxxle_ads

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_ADS_SPACER_VIEW_H_
