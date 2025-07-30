/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_OVERLAY_LUXXLE_BACK_TO_TAB_LABEL_BUTTON_H_
#define LUXXLE_BROWSER_UI_VIEWS_OVERLAY_LUXXLE_BACK_TO_TAB_LABEL_BUTTON_H_

#include "chrome/browser/ui/views/overlay/back_to_tab_label_button.h"
#include "ui/base/metadata/metadata_header_macros.h"

class LuxxleBackToTabLabelButton : public BackToTabLabelButton {
  METADATA_HEADER(LuxxleBackToTabLabelButton, BackToTabLabelButton)

 public:
  explicit LuxxleBackToTabLabelButton(PressedCallback callback);
  ~LuxxleBackToTabLabelButton() override;

  // BackToTabLabelButton:
  void OnThemeChanged() override;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_OVERLAY_LUXXLE_BACK_TO_TAB_LABEL_BUTTON_H_
