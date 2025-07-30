/**
 * Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.notifications.channels;

import android.annotation.SuppressLint;
import android.app.NotificationManager;

import org.chromium.chrome.browser.notifications.R;
import org.chromium.components.browser_ui.notifications.channels.ChannelDefinitions;
import org.chromium.components.browser_ui.notifications.channels.ChannelDefinitions.PredefinedChannel;

import java.util.Map;
import java.util.Set;

public class LuxxleChannelDefinitions {
    public static class ChannelId {
        public static final String LUXXLE_ADS = "com.luxxle.browser.ads";
        public static final String LUXXLE_ADS_BACKGROUND = "com.luxxle.browser.ads.background";
        public static final String LUXXLE_BROWSER = "com.luxxle.browser";
    }

    public static class ChannelGroupId {
        public static final String LUXXLE_ADS = "com.luxxle.browser.ads";
        public static final String GENERAL = "general";
    }

    @SuppressLint("NewApi")
    protected static void addLuxxleChannels(
            Map<String, PredefinedChannel> map, Set<String> startup) {
        map.put(
                ChannelId.LUXXLE_ADS,
                PredefinedChannel.create(
                        ChannelId.LUXXLE_ADS,
                        R.string.luxxle_ads_text,
                        NotificationManager.IMPORTANCE_HIGH,
                        ChannelGroupId.LUXXLE_ADS));
        startup.add(ChannelId.LUXXLE_ADS);

        map.put(
                ChannelId.LUXXLE_ADS_BACKGROUND,
                PredefinedChannel.create(
                        ChannelId.LUXXLE_ADS_BACKGROUND,
                        R.string.notification_category_luxxle_ads_background,
                        NotificationManager.IMPORTANCE_LOW,
                        ChannelGroupId.LUXXLE_ADS));
        startup.add(ChannelId.LUXXLE_ADS_BACKGROUND);
    }

    @SuppressLint("NewApi")
    protected static void addLuxxleChannelGroups(
            Map<String, ChannelDefinitions.PredefinedChannelGroup> map) {
        map.put(
                ChannelGroupId.LUXXLE_ADS,
                new ChannelDefinitions.PredefinedChannelGroup(
                        ChannelGroupId.LUXXLE_ADS, R.string.luxxle_ads_text));
    }
}
