/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_LUXXLE_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_
#define LUXXLE_BROWSER_UI_LUXXLE_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"

class LuxxleBrowserContentSettingBubbleModelDelegate
    : public BrowserContentSettingBubbleModelDelegate {
 public:
  explicit LuxxleBrowserContentSettingBubbleModelDelegate(Browser* browser);
  LuxxleBrowserContentSettingBubbleModelDelegate(
      const LuxxleBrowserContentSettingBubbleModelDelegate&) = delete;
  LuxxleBrowserContentSettingBubbleModelDelegate& operator=(
      const LuxxleBrowserContentSettingBubbleModelDelegate&) = delete;
  ~LuxxleBrowserContentSettingBubbleModelDelegate() override;

  void ShowWidevineLearnMorePage();
  void ShowLearnMorePage(ContentSettingsType type) override;

 private:
  const raw_ptr<Browser> browser_;
};

#endif  // LUXXLE_BROWSER_UI_LUXXLE_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_
