/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_

#include "luxxle/browser/ui/views/tabs/luxxle_tab_container.h"
#include "chrome/browser/ui/views/tabs/tab_container.h"
#include "chrome/browser/ui/views/tabs/tab_slot_controller.h"

class LuxxleTabHoverCardController;

#define UpdateHoverCard                    \
  UpdateHoverCard_Unused();                \
  friend class LuxxleTabHoverTest;          \
  friend class LuxxleTabStrip;              \
  friend class VerticalTabStripRegionView; \
  void UpdateHoverCard

#define ShouldDrawStrokes   \
  UnUsed() { return true; } \
  virtual bool ShouldDrawStrokes
#define GetDragContext                                                  \
  Unused_GetDragContext() {                                             \
    return nullptr;                                                     \
  }                                                                     \
  friend class LuxxleTabStrip;                                           \
  friend class LuxxleTabDragContext;                                     \
  bool IsTabTiled(const Tab* tab) const override;                       \
  bool IsFirstTabInTile(const Tab* tab) const override;                 \
  static constexpr bool IsUsingLuxxleTabHoverCardController() {          \
    return std::is_same_v<std::unique_ptr<LuxxleTabHoverCardController>, \
                          decltype(TabStrip::hover_card_controller_)>;  \
  }                                                                     \
  virtual TabDragContext* GetDragContext
#define TabHoverCardController LuxxleTabHoverCardController
#include "src/chrome/browser/ui/views/tabs/tab_strip.h"  // IWYU pragma: export
#undef TabHoverCardController
#undef GetDragContext
#undef ShouldDrawStrokes
#undef UpdateHoverCard

static_assert(TabStrip::IsUsingLuxxleTabHoverCardController(),
              "Should use LuxxleTabHoverCardController");

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_TABS_TAB_STRIP_H_
