/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_OMNIBOX_LUXXLE_OMNIBOX_RESULT_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_OMNIBOX_LUXXLE_OMNIBOX_RESULT_VIEW_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/views/omnibox/omnibox_result_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

class LuxxleSearchConversionPromotionView;
class LuxxleOmniboxPopupViewViews;

namespace views {
class Label;
}

// This will render luxxle specific matches such as the luxxler search conversion
// promotion.
class LuxxleOmniboxResultView : public OmniboxResultView {
  METADATA_HEADER(LuxxleOmniboxResultView, OmniboxResultView)
 public:
  using OmniboxResultView::OmniboxResultView;
  LuxxleOmniboxResultView(const LuxxleOmniboxResultView&) = delete;
  LuxxleOmniboxResultView& operator=(const LuxxleOmniboxResultView&) = delete;
  ~LuxxleOmniboxResultView() override;

  void OpenMatch();
  void RefreshOmniboxResult();
  LuxxleOmniboxPopupViewViews* GetPopupView();

  // OmniboxResultView overrides:
  void SetMatch(const AutocompleteMatch& match) override;
  void OnSelectionStateChanged() override;
  gfx::Image GetIcon() const override;
  void OnThemeChanged() override;
  void OnPaintBackground(gfx::Canvas* canvas) override;

 private:
  void ResetChildren();
  void UpdateForLuxxleSearchConversion();
  void HandleSelectionStateChangedForPromotionView();
  void UpdateForLeoMatch();

  // Luxxle search conversion promotion
  raw_ptr<LuxxleSearchConversionPromotionView> luxxle_search_promotion_view_ =
      nullptr;

  raw_ptr<views::Label> leo_match_label_ = nullptr;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_OMNIBOX_LUXXLE_OMNIBOX_RESULT_VIEW_H_
