/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/bookmarks/luxxle_bookmark_bar_view.h"

#include "luxxle/browser/ui/bookmark/luxxle_bookmark_prefs.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "components/bookmarks/browser/bookmark_model.h"
#include "ui/base/metadata/metadata_impl_macros.h"

LuxxleBookmarkBarView::LuxxleBookmarkBarView(Browser* browser,
                                           BrowserView* browser_view)
    : BookmarkBarView(browser, browser_view) {
  show_all_bookmarks_button_pref_.Init(
      luxxle::bookmarks::prefs::kShowAllBookmarksButton,
      browser_->profile()->GetPrefs(),
      base::BindRepeating(
          &LuxxleBookmarkBarView::OnShowAllBookmarksButtonPrefChanged,
          base::Unretained(this)));

  MaybeUpdateOtherAndManagedButtonsVisibility();
}

LuxxleBookmarkBarView::~LuxxleBookmarkBarView() = default;

void LuxxleBookmarkBarView::MaybeUpdateOtherAndManagedButtonsVisibility() {
  if (bookmark_service_ && bookmark_service_->bookmark_model() &&
      bookmark_service_->bookmark_model()->loaded()) {
    UpdateOtherAndManagedButtonsVisibility();
  }
}

bool LuxxleBookmarkBarView::UpdateOtherAndManagedButtonsVisibility() {
  bool result = BookmarkBarView::UpdateOtherAndManagedButtonsVisibility();
  if (all_bookmarks_button_ && all_bookmarks_button_->GetVisible() &&
      !show_all_bookmarks_button_pref_.GetValue()) {
    all_bookmarks_button_->SetVisible(false);
    UpdateBookmarksSeparatorVisibility();
    return true;
  }
  return result;
}

void LuxxleBookmarkBarView::OnShowAllBookmarksButtonPrefChanged() {
  if (UpdateOtherAndManagedButtonsVisibility()) {
    UpdateBookmarksSeparatorVisibility();
    LayoutAndPaint();
  }
}

BEGIN_METADATA(LuxxleBookmarkBarView)
END_METADATA
