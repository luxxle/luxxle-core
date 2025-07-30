/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/luxxle_browser_window.h"

#include <vector>

// Provide a base implementation (important for `TestBrowserWindow ` in tests)
// For real implementation, see `LuxxleBrowserView`.

speedreader::SpeedreaderBubbleView* LuxxleBrowserWindow::ShowSpeedreaderBubble(
    speedreader::SpeedreaderTabHelper* tab_helper,
    speedreader::SpeedreaderBubbleLocation location) {
  return nullptr;
}

gfx::Rect LuxxleBrowserWindow::GetShieldsBubbleRect() {
  return gfx::Rect();
}

// static
LuxxleBrowserWindow* LuxxleBrowserWindow::From(BrowserWindow* window) {
  return static_cast<LuxxleBrowserWindow*>(window);
}

#if defined(TOOLKIT_VIEWS)
sidebar::Sidebar* LuxxleBrowserWindow::InitSidebar() {
  return nullptr;
}

void LuxxleBrowserWindow::ToggleSidebar() {}

bool LuxxleBrowserWindow::HasSelectedURL() const {
  return false;
}

void LuxxleBrowserWindow::CleanAndCopySelectedURL() {}

bool LuxxleBrowserWindow::ShowLuxxleHelpBubbleView(const std::string& text) {
  return false;
}
#endif  // defined(TOOLKIT_VIEWS)

bool LuxxleBrowserWindow::IsInTabDragging() const {
  return false;
}
