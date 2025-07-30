/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_BOOKMARK_BUTTON_H_
#define LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_BOOKMARK_BUTTON_H_

#include "chrome/browser/ui/views/toolbar/toolbar_button.h"
#include "ui/base/metadata/metadata_header_macros.h"

class LuxxleBookmarkButton : public ToolbarButton {
  METADATA_HEADER(LuxxleBookmarkButton, ToolbarButton)
 public:
  explicit LuxxleBookmarkButton(PressedCallback callback);
  LuxxleBookmarkButton(const LuxxleBookmarkButton&) = delete;
  LuxxleBookmarkButton& operator=(const LuxxleBookmarkButton&) = delete;
  ~LuxxleBookmarkButton() override;

  void SetToggled(bool on);
  void UpdateImageAndText();

 private:
  bool active_ = false;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_TOOLBAR_BOOKMARK_BUTTON_H_
