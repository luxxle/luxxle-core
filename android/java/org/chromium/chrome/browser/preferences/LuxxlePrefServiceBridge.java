/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences;

import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;

import org.chromium.base.ThreadUtils;
import org.chromium.chrome.browser.profiles.Profile;

/**
 * Please don't add anything in that file. We are going to refactor it soon.
 * Check this PRs on how to handle preferences correctly:
 * https://github.com/luxxle/luxxle-core/pull/16356
 * https://github.com/luxxle/luxxle-core/pull/15905
 * For the local_state based prefs please look on the PR:
 * https://github.com/luxxle/luxxle-core/pull/16486
 * Contact code owners if you have additional questions.
 */

@JNINamespace("chrome::android")
public class LuxxlePrefServiceBridge {
    private LuxxlePrefServiceBridge() {
    }

    private static LuxxlePrefServiceBridge sInstance;

    public static LuxxlePrefServiceBridge getInstance() {
        ThreadUtils.assertOnUiThread();
        if (sInstance == null) {
            sInstance = new LuxxlePrefServiceBridge();
        }
        return sInstance;
    }

    public void setCookiesBlockType(String type) {
        LuxxlePrefServiceBridgeJni.get().setCookiesBlockType(type);
    }

    public String getCookiesBlockType() {
        return LuxxlePrefServiceBridgeJni.get().getCookiesBlockType();
    }

    public void setPlayYTVideoInBrowserEnabled(boolean enabled) {
        LuxxlePrefServiceBridgeJni.get().setPlayYTVideoInBrowserEnabled(enabled);
    }

    public boolean getPlayYTVideoInBrowserEnabled() {
        return LuxxlePrefServiceBridgeJni.get().getPlayYTVideoInBrowserEnabled();
    }

    public void setDesktopModeEnabled(boolean enabled) {
        LuxxlePrefServiceBridgeJni.get().setDesktopModeEnabled(enabled);
    }

    public boolean getDesktopModeEnabled() {
        return LuxxlePrefServiceBridgeJni.get().getDesktopModeEnabled();
    }

    public long getTrackersBlockedCount(Profile profile) {
        return LuxxlePrefServiceBridgeJni.get().getTrackersBlockedCount(profile);
    }

    public long getAdsBlockedCount(Profile profile) {
        return LuxxlePrefServiceBridgeJni.get().getAdsBlockedCount(profile);
    }

    public long getDataSaved(Profile profile) {
        return LuxxlePrefServiceBridgeJni.get().getDataSaved(profile);
    }

    public void resetPromotionLastFetchStamp() {
        LuxxlePrefServiceBridgeJni.get().resetPromotionLastFetchStamp();
    }

    public void setOldTrackersBlockedCount(Profile profile, long count) {
        LuxxlePrefServiceBridgeJni.get().setOldTrackersBlockedCount(profile, count);
    }

    public void setOldAdsBlockedCount(Profile profile, long count) {
        LuxxlePrefServiceBridgeJni.get().setOldAdsBlockedCount(profile, count);
    }

    public void setOldHttpsUpgradesCount(Profile profile, long count) {
        LuxxlePrefServiceBridgeJni.get().setOldHttpsUpgradesCount(profile, count);
    }

    public boolean getBooleanForContentSetting(int contentType) {
        return LuxxlePrefServiceBridgeJni.get().getBooleanForContentSetting(contentType);
    }

    public void setWebrtcPolicy(int policy) {
        LuxxlePrefServiceBridgeJni.get().setWebrtcPolicy(policy);
    }

    public int getWebrtcPolicy() {
        return LuxxlePrefServiceBridgeJni.get().getWebrtcPolicy();
    }

    public void setNewsOptIn(boolean value) {
        LuxxlePrefServiceBridgeJni.get().setNewsOptIn(value);
    }

    public boolean getNewsOptIn() {
        return LuxxlePrefServiceBridgeJni.get().getNewsOptIn();
    }

    public void setShowNews(boolean value) {
        LuxxlePrefServiceBridgeJni.get().setShowNews(value);
    }

    public boolean getShowNews() {
        return LuxxlePrefServiceBridgeJni.get().getShowNews();
    }

    @NativeMethods
    interface Natives {
        void setCookiesBlockType(String type);
        String getCookiesBlockType();

        void setPlayYTVideoInBrowserEnabled(boolean enabled);

        boolean getPlayYTVideoInBrowserEnabled();

        void setDesktopModeEnabled(boolean enabled);

        boolean getDesktopModeEnabled();

        long getTrackersBlockedCount(Profile profile);

        long getAdsBlockedCount(Profile profile);

        long getDataSaved(Profile profile);

        // Used to pass total stat from upgrading old tabs based browser
        // to a new core based
        void setOldTrackersBlockedCount(Profile profile, long count);

        void setOldAdsBlockedCount(Profile profile, long count);

        void setOldHttpsUpgradesCount(Profile profile, long count);

        void resetPromotionLastFetchStamp();

        boolean getBooleanForContentSetting(int contentType);

        void setWebrtcPolicy(int policy);

        int getWebrtcPolicy();

        void setNewsOptIn(boolean value);

        boolean getNewsOptIn();

        void setShowNews(boolean value);

        boolean getShowNews();
    }
}
