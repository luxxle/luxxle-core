/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_SIDE_PANEL_LUXXLE_SIDE_PANEL_UTILS_H_
#define LUXXLE_BROWSER_UI_SIDE_PANEL_LUXXLE_SIDE_PANEL_UTILS_H_

namespace content {
class WebContents;
}  // namespace content

class SidePanelRegistry;

namespace luxxle {

// Register per-tab(contextual) side-panel registry.
// Defined at //luxxle/browser/ui/views/side_panel/luxxle_side_panel_utils.cc as
// the implementation is view-layer specific.
void RegisterContextualSidePanel(SidePanelRegistry* registry,
                                 content::WebContents* web_contents);

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_UI_SIDE_PANEL_LUXXLE_SIDE_PANEL_UTILS_H_
