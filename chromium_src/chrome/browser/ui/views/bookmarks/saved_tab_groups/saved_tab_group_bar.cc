/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/bookmarks/saved_tab_groups/luxxle_saved_tab_group_button.h"

// Create a LuxxleSavedTabGroupButton instead of SavedTabGroupButton (we don't
// want to replace SavedTabGroupButton everywhere in this file as that led to
// unnecessary complications in upstream tests).
#define AddChildViewAt(VIEW, INDEX)                                        \
  AddChildViewAt(                                                          \
      std::make_unique<LuxxleSavedTabGroupButton>(                          \
          group,                                                           \
          base::BindRepeating(&SavedTabGroupBar::OnTabGroupButtonPressed,  \
                              base::Unretained(this), group.saved_guid()), \
          browser_, animations_enabled_),                                  \
      INDEX)

#include "src/chrome/browser/ui/views/bookmarks/saved_tab_groups/saved_tab_group_bar.cc"

#undef AddChildViewAt
