/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_BUBBLE_CONTROLLER_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_BUBBLE_CONTROLLER_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "content/public/browser/web_contents_user_data.h"

namespace content {
class WebContents;
}

class LuxxleNewsBubbleView;
class LuxxleNewsActionIconView;

namespace luxxle_news {

class LuxxleNewsBubbleController
    : public content::WebContentsUserData<LuxxleNewsBubbleController> {
 public:
  static LuxxleNewsBubbleController* CreateOrGetFromWebContents(
      content::WebContents* web_contents);

  ~LuxxleNewsBubbleController() override;

  void ShowBubble(base::WeakPtr<LuxxleNewsActionIconView> anchor_view);
  LuxxleNewsBubbleView* GetBubble();
  void OnBubbleClosed();
  base::WeakPtr<LuxxleNewsBubbleController> AsWeakPtr();

 private:
  friend class content::WebContentsUserData<LuxxleNewsBubbleController>;
  WEB_CONTENTS_USER_DATA_KEY_DECL();

  explicit LuxxleNewsBubbleController(content::WebContents* web_contents);

  raw_ptr<LuxxleNewsBubbleView> bubble_ = nullptr;
  raw_ptr<content::WebContents> web_contents_ = nullptr;

  base::WeakPtrFactory<LuxxleNewsBubbleController> weak_ptr_factory_{this};
};

}  // namespace luxxle_news

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_BUBBLE_CONTROLLER_H_
