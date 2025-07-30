/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_PLAYER_LUXXLE_PLAYER_ACTION_ICON_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_PLAYER_LUXXLE_PLAYER_ACTION_ICON_VIEW_H_

#include "chrome/browser/ui/views/page_action/page_action_icon_view.h"
#include "url/gurl.h"

class Browser;

class LuxxlePlayerActionIconView : public PageActionIconView {
  METADATA_HEADER(LuxxlePlayerActionIconView, PageActionIconView)
 public:

  LuxxlePlayerActionIconView(
      CommandUpdater* command_updater,
      Browser& browser,
      IconLabelBubbleView::Delegate* icon_label_bubble_delegate,
      PageActionIconView::Delegate* page_action_icon_delegate);
  LuxxlePlayerActionIconView(const LuxxlePlayerActionIconView&) = delete;
  LuxxlePlayerActionIconView& operator=(const LuxxlePlayerActionIconView&) =
      delete;
  ~LuxxlePlayerActionIconView() override;

  // PageActionIconView:
  void OnExecuting(ExecuteSource execute_source) override;
  views::BubbleDialogDelegate* GetBubble() const override;
  const gfx::VectorIcon& GetVectorIcon() const override;
  void UpdateIconImage() override;
  void UpdateImpl() override;
  void UpdateBorder() override;

 private:
  raw_ref<Browser> browser_;

  // A url to open luxxle player corresponding to the current tab.
  GURL player_url_;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_PLAYER_LUXXLE_PLAYER_ACTION_ICON_VIEW_H_
