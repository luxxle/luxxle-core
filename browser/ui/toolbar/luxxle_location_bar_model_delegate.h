/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_LOCATION_BAR_MODEL_DELEGATE_H_
#define LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_LOCATION_BAR_MODEL_DELEGATE_H_

#include "base/compiler_specific.h"
#include "chrome/browser/ui/browser_location_bar_model_delegate.h"

class Browser;

class LuxxleLocationBarModelDelegate : public BrowserLocationBarModelDelegate {
 public:
  explicit LuxxleLocationBarModelDelegate(Browser* browser);
  LuxxleLocationBarModelDelegate(const LuxxleLocationBarModelDelegate&) = delete;
  LuxxleLocationBarModelDelegate& operator=(
      const LuxxleLocationBarModelDelegate&) = delete;
  ~LuxxleLocationBarModelDelegate() override;
  static void FormattedStringFromURL(const GURL& url,
                                     std::u16string* new_formatted_url);

 private:
  std::u16string FormattedStringWithEquivalentMeaning(
      const GURL& url,
      const std::u16string& formatted_url) const override;
  bool GetURL(GURL* url) const override;

  raw_ptr<Browser> browser_ = nullptr;
};

#endif  // LUXXLE_BROWSER_UI_TOOLBAR_LUXXLE_LOCATION_BAR_MODEL_DELEGATE_H_
