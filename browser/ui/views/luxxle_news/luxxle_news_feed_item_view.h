// Copyright (c) 2022 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_FEED_ITEM_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_FEED_ITEM_VIEW_H_

#include <vector>

#include "luxxle/browser/luxxle_news/luxxle_news_tab_helper.h"
#include "ui/views/controls/label.h"
#include "ui/views/view.h"

namespace content {
class WebContents;
}

namespace views {
class MdTextButton;
}

class LuxxleNewsFeedItemView : public views::View,
                              public LuxxleNewsTabHelper::PageFeedsObserver {
  METADATA_HEADER(LuxxleNewsFeedItemView, views::View)
 public:

  LuxxleNewsFeedItemView(const GURL& feed_url, content::WebContents* contents);
  LuxxleNewsFeedItemView(const LuxxleNewsFeedItemView&) = delete;
  LuxxleNewsFeedItemView& operator=(const LuxxleNewsFeedItemView&) = delete;
  ~LuxxleNewsFeedItemView() override;

  void Update();
  void OnPressed();

  // LuxxleNewsTabHelper::PageFeedsObserver:
  void OnAvailableFeedsChanged(const std::vector<GURL>& feed_urls) override;

 private:
  bool loading_ = false;
  raw_ptr<views::Label> title_ = nullptr;
  raw_ptr<views::MdTextButton> subscribe_button_ = nullptr;

  GURL feed_url_;
  raw_ptr<content::WebContents> contents_;
  raw_ptr<LuxxleNewsTabHelper> tab_helper_;

  base::ScopedObservation<LuxxleNewsTabHelper,
                          LuxxleNewsTabHelper::PageFeedsObserver>
      tab_helper_observation_{this};
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_FEED_ITEM_VIEW_H_
