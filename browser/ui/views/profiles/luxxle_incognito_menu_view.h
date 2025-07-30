/* Copyright 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_PROFILES_LUXXLE_INCOGNITO_MENU_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_PROFILES_LUXXLE_INCOGNITO_MENU_VIEW_H_

#include "chrome/browser/ui/views/profiles/incognito_menu_view.h"

class LuxxleIncognitoMenuView : public IncognitoMenuView {
 public:
  using IncognitoMenuView::IncognitoMenuView;

  LuxxleIncognitoMenuView(const LuxxleIncognitoMenuView&) = delete;
  LuxxleIncognitoMenuView& operator=(const LuxxleIncognitoMenuView&) = delete;
  ~LuxxleIncognitoMenuView() override = default;

  // ProfileMenuViewBase:
  void BuildMenu() override;
  void AddedToWidget() override;

 private:
  friend class IncognitoMenuView;

  // views::BubbleDialogDelegateView:
  std::u16string GetAccessibleWindowTitle() const override;

  // Button actions.
  void OnExitButtonClicked() override;

  void AddTorButton();
  void OnTorProfileButtonClicked();
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_PROFILES_LUXXLE_INCOGNITO_MENU_VIEW_H_
