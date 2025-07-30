/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.ntp;

import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.settings.AppearancePreferences;
import org.chromium.chrome.browser.settings.BackgroundImagesPreferences;

public class NtpUtil {
    public static boolean shouldDisplayTopSites() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(BackgroundImagesPreferences.PREF_SHOW_TOP_SITES, true);
    }

    public static void setDisplayTopSites(boolean shouldDisplayTopSites) {
        ChromeSharedPreferences.getInstance()
                .writeBoolean(
                        BackgroundImagesPreferences.PREF_SHOW_TOP_SITES, shouldDisplayTopSites);
    }

    public static boolean shouldDisplayLuxxleStats() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(BackgroundImagesPreferences.PREF_SHOW_LUXXLE_STATS, true);
    }

    public static void setDisplayLuxxleStats(boolean shouldDisplayLuxxleStats) {
        ChromeSharedPreferences.getInstance()
                .writeBoolean(
                        BackgroundImagesPreferences.PREF_SHOW_LUXXLE_STATS, shouldDisplayLuxxleStats);
    }

    public static boolean shouldShowRewardsIcon() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(AppearancePreferences.PREF_SHOW_LUXXLE_REWARDS_ICON, true);
    }
}
