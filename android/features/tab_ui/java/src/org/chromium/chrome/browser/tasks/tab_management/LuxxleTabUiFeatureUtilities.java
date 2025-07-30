/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.tasks.tab_management;

import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.build.annotations.NullMarked;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;

@NullMarked
public class LuxxleTabUiFeatureUtilities {
    public static boolean isLuxxleTabGroupsEnabled() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(
                        LuxxlePreferenceKeys.LUXXLE_TAB_GROUPS_ENABLED,
                        ChromeSharedPreferences.getInstance()
                                .readBoolean(
                                        LuxxlePreferenceKeys.LUXXLE_TAB_GROUPS_ENABLED_DEFAULT_VALUE,
                                        true));
    }
}
