/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */
package org.chromium.chrome.browser.tabmodel;

import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.base.LuxxleReflectionUtil;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabLaunchType;

/** Luxxle's super class for {@link TabGroupModelFilter} */
public class LuxxleTabGroupModelFilter {
    /**
     * This variable will be used instead of {@link TabGroupModelFilterImpl}'s variable, that will
     * be deleted in bytecode.
     */
    protected boolean mIsResetting;

    /** Call from {@link TabGroupModelFilterImpl} will be redirected here via bytecode. */
    public boolean shouldUseParentIds(Tab tab) {
        if (linkClicked(tab.getLaunchType())
                && ChromeSharedPreferences.getInstance()
                        .readBoolean(
                                LuxxlePreferenceKeys.LUXXLE_TAB_GROUPS_ENABLED,
                                ChromeSharedPreferences.getInstance()
                                        .readBoolean(
                                                LuxxlePreferenceKeys
                                                        .LUXXLE_TAB_GROUPS_ENABLED_DEFAULT_VALUE,
                                                true))
                && isTabModelRestored()
                && !mIsResetting) {
            return true;
        }
        // Otherwise just call parent.
        return (boolean)
                LuxxleReflectionUtil.invokeMethod(
                        TabGroupModelFilterImpl.class, this, "shouldUseParentIds", Tab.class, tab);
    }

    /** Determine if a launch type is the result of linked being clicked. */
    private boolean linkClicked(@TabLaunchType int type) {
        return type == TabLaunchType.FROM_LINK || type == TabLaunchType.FROM_LONGPRESS_FOREGROUND;
    }

    private boolean isTabModelRestored() {
        return (boolean)
                LuxxleReflectionUtil.invokeMethod(
                        TabGroupModelFilterImpl.class, this, "isTabModelRestored");
    }
}
