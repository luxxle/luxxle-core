/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_BOOKMARKS_SAVED_TAB_GROUPS_LUXXLE_SAVED_TAB_GROUP_BUTTON_H_
#define LUXXLE_BROWSER_UI_VIEWS_BOOKMARKS_SAVED_TAB_GROUPS_LUXXLE_SAVED_TAB_GROUP_BUTTON_H_

#include <utility>

#include "chrome/browser/ui/views/bookmarks/saved_tab_groups/saved_tab_group_button.h"

namespace tab_groups {

// A replacement for `SavedTabGroupButton` that matches the styling of Luxxle's
// tab group headers.
class LuxxleSavedTabGroupButton : public SavedTabGroupButton {
  METADATA_HEADER(LuxxleSavedTabGroupButton, SavedTabGroupButton)

 public:
  template <typename... Args>
  explicit LuxxleSavedTabGroupButton(Args&&... args)
      : SavedTabGroupButton(std::forward<Args>(args)...) {
    Initialize();
  }

  ~LuxxleSavedTabGroupButton() override;

  void PaintButtonContents(gfx::Canvas* canvas) override;

 private:
  void Initialize();
  void UpdateButtonLayout() override;
};

}  // namespace tab_groups

#endif  // LUXXLE_BROWSER_UI_VIEWS_BOOKMARKS_SAVED_TAB_GROUPS_LUXXLE_SAVED_TAB_GROUP_BUTTON_H_
