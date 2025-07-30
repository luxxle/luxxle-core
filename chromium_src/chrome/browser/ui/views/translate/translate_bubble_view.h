/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_

class TranslateBubbleView;
using LuxxleTranslateBubbleView = TranslateBubbleView;

#define TranslateBubbleView TranslateBubbleView_ChromiumImpl
#define CreateTranslateIcon virtual CreateTranslateIcon
#define is_in_incognito_window_           \
  unused_is_in_incognito_window_ = false; \
  friend LuxxleTranslateBubbleView;        \
  bool is_in_incognito_window_ /* make the field non-constant */
#include "src/chrome/browser/ui/views/translate/translate_bubble_view.h"  // IWYU pragma: export
#undef is_in_incognito_window_
#undef CreateTranslateIcon
#undef TranslateBubbleView

#include "luxxle/components/translate/core/common/luxxle_translate_features.h"

class TranslateBubbleView : public TranslateBubbleView_ChromiumImpl {
  METADATA_HEADER(TranslateBubbleView, TranslateBubbleView_ChromiumImpl)
 public:

  template <class... Args>
  explicit TranslateBubbleView(Args&&... args)
      : TranslateBubbleView_ChromiumImpl(std::forward<Args>(args)...) {
    if (!translate::IsLuxxleAutoTranslateEnabled()) {
      // Setting incognito mode disables UI elements connected to auto
      // translate. "Never translate lang" options should still work.
      is_in_incognito_window_ = true;
    }
  }

  std::unique_ptr<views::ImageView> CreateTranslateIcon() override;
};

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TRANSLATE_TRANSLATE_BUBBLE_VIEW_H_
