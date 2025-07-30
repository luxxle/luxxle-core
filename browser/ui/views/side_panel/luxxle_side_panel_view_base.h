/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_SIDE_PANEL_VIEW_BASE_H_
#define LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_SIDE_PANEL_VIEW_BASE_H_

#include "base/scoped_observation.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/view.h"
#include "ui/views/view_observer.h"

class LuxxleSidePanelViewBase : public views::View, public views::ViewObserver {
  METADATA_HEADER(LuxxleSidePanelViewBase, views::View)
 public:
  static constexpr inline int kHeaderHeight = 60;

  LuxxleSidePanelViewBase();
  ~LuxxleSidePanelViewBase() override;
  LuxxleSidePanelViewBase(const LuxxleSidePanelViewBase&) = delete;
  LuxxleSidePanelViewBase& operator=(const LuxxleSidePanelViewBase&) = delete;

 protected:
  void StartObservingWebWebViewVisibilityChange(views::View* web_view);

 private:
  // views::ViewObserver overrides:
  void OnViewVisibilityChanged(views::View* observed_view,
                               views::View* starting_view) override;

  base::ScopedObservation<views::View, views::ViewObserver> view_observation_{
      this};
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_SIDE_PANEL_LUXXLE_SIDE_PANEL_VIEW_BASE_H_
