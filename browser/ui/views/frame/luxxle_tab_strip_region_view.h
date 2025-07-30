/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_TAB_STRIP_REGION_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_TAB_STRIP_REGION_VIEW_H_

#include "chrome/browser/ui/views/frame/tab_strip_region_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

// `TabStripRegionView` is customized to insert a medium-sized gap before the
// first tab when the browser frame is not fullscreen or maximized.
class LuxxleTabStripRegionView : public TabStripRegionView {
  METADATA_HEADER(LuxxleTabStripRegionView, TabStripRegionView)

 public:
  template <typename... Args>
  explicit LuxxleTabStripRegionView(Args&&... args)
      : TabStripRegionView(std::forward<Args>(args)...) {
    Initialize();
  }

  ~LuxxleTabStripRegionView() override;

  void Layout(PassKey) override;

 private:
  void UpdateTabStripMargin() override;
  void Initialize();
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_TAB_STRIP_REGION_VIEW_H_
