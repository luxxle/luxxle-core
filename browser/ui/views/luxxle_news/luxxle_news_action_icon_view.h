// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_ACTION_ICON_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_ACTION_ICON_VIEW_H_

#include <string>
#include <vector>

#include "base/scoped_observation.h"
#include "luxxle/browser/luxxle_news/luxxle_news_tab_helper.h"
#include "chrome/browser/ui/views/page_action/page_action_icon_view.h"
#include "components/prefs/pref_member.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/gfx/vector_icon_types.h"
#include "ui/views/view.h"

class Profile;

namespace luxxle_news {
class LuxxleNewsBubbleController;
}

// LocationBar action for Luxxle News which shows a bubble allowing the user to
// manage feed subscriptions for the current Tab
class LuxxleNewsActionIconView : public PageActionIconView,
                                public LuxxleNewsTabHelper::PageFeedsObserver,
                                public content::WebContentsObserver {
  METADATA_HEADER(LuxxleNewsActionIconView, PageActionIconView)
 public:
  LuxxleNewsActionIconView(
      Profile* profile,
      IconLabelBubbleView::Delegate* icon_label_bubble_delegate,
      PageActionIconView::Delegate* page_action_icon_delegate);
  LuxxleNewsActionIconView(const LuxxleNewsActionIconView&) = delete;
  LuxxleNewsActionIconView& operator=(const LuxxleNewsActionIconView&) = delete;
  ~LuxxleNewsActionIconView() override;

  base::WeakPtr<LuxxleNewsActionIconView> AsWeakPtr();

  // PageActionIconView:
  views::BubbleDialogDelegate* GetBubble() const override;
  void UpdateImpl() override;
  std::u16string GetTextForTooltipAndAccessibleName() const override;
  bool ShouldShowLabel() const override;

  // LuxxleNewsTabHelper::PageFeedsObserver:
  void OnAvailableFeedsChanged(const std::vector<GURL>& feeds) override;

  // views::View:
  void OnThemeChanged() override;

  // content::WebContentsObserver
  void WebContentsDestroyed() override;

 protected:
  luxxle_news::LuxxleNewsBubbleController* GetController() const;

  // PageActionIconView:
  void OnExecuting(PageActionIconView::ExecuteSource execute_source) override;
  const gfx::VectorIcon& GetVectorIcon() const override;

 private:
  void UpdateIconColor(bool subscribed);
  void ShowLuxxleNewsBubble();

  base::ScopedObservation<LuxxleNewsTabHelper,
                          LuxxleNewsTabHelper::PageFeedsObserver>
      page_feeds_observer_{this};
  BooleanPrefMember should_show_;
  BooleanPrefMember opted_in_;
  BooleanPrefMember news_enabled_;

  base::WeakPtrFactory<LuxxleNewsActionIconView> weak_ptr_factory_{this};
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_NEWS_LUXXLE_NEWS_ACTION_ICON_VIEW_H_
