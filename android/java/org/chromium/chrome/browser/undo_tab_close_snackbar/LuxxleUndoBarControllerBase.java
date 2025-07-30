/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.undo_tab_close_snackbar;

import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.base.LuxxleReflectionUtil;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager;

import java.util.List;

public class LuxxleUndoBarControllerBase implements SnackbarManager.SnackbarController {
    public void showUndoBar(List<Tab> closedTabs, boolean isAllTabs) {
        boolean showUndoBar =
                ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePreferenceKeys.SHOW_UNDO_WHEN_TABS_CLOSED, true);
        if (!showUndoBar) {
            if (closedTabs.isEmpty()) {
                return;
            }
            Object actionData = closedTabs.size() == 1 ? closedTabs.get(0).getId() : closedTabs;
            onDismissNoAction(actionData);
            return;
        }

        LuxxleReflectionUtil.invokeMethod(
                UndoBarController.class,
                this,
                "showUndoBar",
                List.class,
                closedTabs,
                boolean.class,
                isAllTabs);
    }
}
