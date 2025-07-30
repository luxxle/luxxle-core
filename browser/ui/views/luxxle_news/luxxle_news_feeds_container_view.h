// Copyright (c) 2022 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_FEEDS_CONTAINER_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_FEEDS_CONTAINER_VIEW_H_

#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/gfx/geometry/size.h"
#include "ui/views/layout/layout_types.h"
#include "ui/views/view.h"

namespace content {
class WebContents;
}

class LuxxleNewsFeedsContainerView : public views::View {
  METADATA_HEADER(LuxxleNewsFeedsContainerView, views::View)
 public:

  explicit LuxxleNewsFeedsContainerView(content::WebContents* contents);
  LuxxleNewsFeedsContainerView(const LuxxleNewsFeedsContainerView&) = delete;
  LuxxleNewsFeedsContainerView& operator=(const LuxxleNewsFeedsContainerView&) =
      delete;
  ~LuxxleNewsFeedsContainerView() override;

  // views::View
  void OnThemeChanged() override;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_FEEDS_CONTAINER_VIEW_H_
