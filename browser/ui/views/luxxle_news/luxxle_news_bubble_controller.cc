/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/luxxle_news/luxxle_news_bubble_controller.h"

#include <vector>

#include "base/memory/ptr_util.h"
#include "luxxle/browser/ui/views/luxxle_news/luxxle_news_action_icon_view.h"
#include "luxxle/browser/ui/views/luxxle_news/luxxle_news_bubble_view.h"
#include "ui/views/bubble/bubble_dialog_delegate_view.h"

namespace luxxle_news {
// static
LuxxleNewsBubbleController*
LuxxleNewsBubbleController::CreateOrGetFromWebContents(
    content::WebContents* web_contents) {
  CHECK(web_contents);
  LuxxleNewsBubbleController::CreateForWebContents(web_contents);
  return LuxxleNewsBubbleController::FromWebContents(web_contents);
}

LuxxleNewsBubbleController::~LuxxleNewsBubbleController() = default;

void LuxxleNewsBubbleController::ShowBubble(
    base::WeakPtr<LuxxleNewsActionIconView> anchor_view) {
  if (!anchor_view) {
    return;
  }

  bubble_ = new LuxxleNewsBubbleView(anchor_view.get(), web_contents_);
  views::BubbleDialogDelegateView::CreateBubble(
      base::WrapUnique(
          static_cast<views::BubbleDialogDelegateView*>(bubble_.get())))
      ->Show();
}

LuxxleNewsBubbleView* LuxxleNewsBubbleController::GetBubble() {
  return bubble_;
}

void LuxxleNewsBubbleController::OnBubbleClosed() {
  bubble_ = nullptr;
}

base::WeakPtr<LuxxleNewsBubbleController>
LuxxleNewsBubbleController::AsWeakPtr() {
  return weak_ptr_factory_.GetWeakPtr();
}

LuxxleNewsBubbleController::LuxxleNewsBubbleController(
    content::WebContents* web_contents)
    : content::WebContentsUserData<LuxxleNewsBubbleController>(*web_contents),
      web_contents_(web_contents) {}

WEB_CONTENTS_USER_DATA_KEY_IMPL(LuxxleNewsBubbleController);

}  // namespace luxxle_news
