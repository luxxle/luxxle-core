/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LOCATION_BAR_LUXXLE_STAR_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LOCATION_BAR_LUXXLE_STAR_VIEW_H_

#include "chrome/browser/ui/views/location_bar/star_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

class LuxxleStarView : public StarView {
  METADATA_HEADER(LuxxleStarView, StarView)
 public:
  using StarView::StarView;

  LuxxleStarView(const LuxxleStarView&) = delete;
  LuxxleStarView& operator=(const LuxxleStarView&) = delete;

 protected:
  // views::View:
  void UpdateImpl() override;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LOCATION_BAR_LUXXLE_STAR_VIEW_H_
