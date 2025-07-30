/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.preferences.website;

import org.jni_zero.CalledByNative;
import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;

import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.profiles.ProfileManager;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class LuxxleShieldsContentSettings {
    public static final String RESOURCE_IDENTIFIER_ADS = "shieldsAds";
    public static final String RESOURCE_IDENTIFIER_TRACKERS = "trackers";
    public static final String RESOURCE_IDENTIFIER_DATA_SAVED = "data_saved";
    public static final String RESOURCE_IDENTIFIER_LUXXLE_SHIELDS = "luxxleShields";
    public static final String RESOURCE_IDENTIFIER_FINGERPRINTING = "fingerprinting";
    public static final String RESOURCE_IDENTIFIER_COOKIES = "shieldsCookies";
    public static final String RESOURCE_IDENTIFIER_REFERRERS = "referrers";
    public static final String RESOURCE_IDENTIFIER_JAVASCRIPTS = "javascript";
    public static final String RESOURCE_IDENTIFIER_HTTPS_UPGRADE = "httpsUpgrade";
    public static final String RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE =
            "forgetFirstPartyStorage";

    public static final String BLOCK_RESOURCE = "block";
    public static final String BLOCK_THIRDPARTY_RESOURCE = "block_third_party";
    public static final String DEFAULT = "default";
    public static final String ALLOW_RESOURCE = "allow";
    public static final String AGGRESSIVE = "aggressive";

    public static final int ALWAYS = 0;
    public static final int ASK = 1;
    public static final int NEVER = 2;

    private long mNativeLuxxleShieldsContentSettings;
    private List<LuxxleShieldsContentSettingsObserver> mLuxxleShieldsContentSettingsObservers;
    private static LuxxleShieldsContentSettings sInstance;

    public static LuxxleShieldsContentSettings getInstance() {
        if (sInstance == null) sInstance = new LuxxleShieldsContentSettings();
        return sInstance;
    }

    private LuxxleShieldsContentSettings() {
        mNativeLuxxleShieldsContentSettings = 0;
        mLuxxleShieldsContentSettingsObservers =
            new ArrayList<LuxxleShieldsContentSettingsObserver>();
        init();
    }

    public void addObserver(LuxxleShieldsContentSettingsObserver observer) {
        mLuxxleShieldsContentSettingsObservers.add(observer);
    }

    public void removeObserver(LuxxleShieldsContentSettingsObserver observer) {
        mLuxxleShieldsContentSettingsObservers.remove(observer);
    }

    private void init() {
        if (mNativeLuxxleShieldsContentSettings == 0) {
            LuxxleShieldsContentSettingsJni.get().init(this);
        }
    }

    /**
     * A finalizer is required to ensure that the native object associated with this descriptor gets
     * torn down, otherwise there would be a memory leak.
     */
    @SuppressWarnings("Finalize")
    @Override
    protected void finalize() {
        destroy();
    }

    private void destroy() {
        if (mNativeLuxxleShieldsContentSettings == 0) {
            return;
        }
        LuxxleShieldsContentSettingsJni.get().destroy(mNativeLuxxleShieldsContentSettings);
        mNativeLuxxleShieldsContentSettings = 0;
    }

    public static void resetCosmeticFilter(String url) {
        LuxxleShieldsContentSettingsJni.get().resetCosmeticFilter(url);
    }

    public static boolean areAnyBlockedElementsPresent(String url) {
        return LuxxleShieldsContentSettingsJni.get().areAnyBlockedElementsPresent(url);
    }

    public static void setShields(
            Profile profile,
            String host,
            String resourceIndentifier,
            boolean value,
            boolean fromTopShields) {
        String setting_string = (value ? BLOCK_RESOURCE : ALLOW_RESOURCE);
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_LUXXLE_SHIELDS)) {
            LuxxleShieldsContentSettingsJni.get().setLuxxleShieldsEnabled(value, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            LuxxleShieldsContentSettingsJni.get().setNoScriptControlType(setting_string, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE)) {
            LuxxleShieldsContentSettingsJni.get().setForgetFirstPartyStorageEnabled(
                    value, host, profile);
        }
    }

    public static void setShieldsValue(Profile profile, String host, String resourceIndentifier,
            String settingOption, boolean fromTopShields) {
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            LuxxleShieldsContentSettingsJni.get().setFingerprintingControlType(
                    settingOption, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTPS_UPGRADE)) {
            LuxxleShieldsContentSettingsJni.get().setHttpsUpgradeControlType(
                    settingOption, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_COOKIES)) {
            LuxxleShieldsContentSettingsJni.get().setCookieControlType(settingOption, host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_TRACKERS)) {
            LuxxleShieldsContentSettingsJni.get().setCosmeticFilteringControlType(
                    DEFAULT.equals(settingOption) ? BLOCK_THIRDPARTY_RESOURCE : settingOption, host,
                    profile);
            LuxxleShieldsContentSettingsJni.get().setAdControlType(
                    BLOCK_THIRDPARTY_RESOURCE.equals(settingOption) ? DEFAULT : settingOption, host,
                    profile);
        }
    }

    public static boolean getShields(Profile profile, String host, String resourceIndentifier) {
        String settings = BLOCK_RESOURCE;
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_LUXXLE_SHIELDS)) {
            return LuxxleShieldsContentSettingsJni.get().getLuxxleShieldsEnabled(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            settings = LuxxleShieldsContentSettingsJni.get().getNoScriptControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE)) {
            return LuxxleShieldsContentSettingsJni.get().getForgetFirstPartyStorageEnabled(
                    host, profile);
        }

        return !settings.equals(ALLOW_RESOURCE);
    }

    public static String getShieldsValue(Profile profile, String host, String resourceIndentifier) {
        String settings = BLOCK_RESOURCE;
        if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            settings = LuxxleShieldsContentSettingsJni.get().getFingerprintingControlType(
                    host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_HTTPS_UPGRADE)) {
            settings =
                    LuxxleShieldsContentSettingsJni.get().getHttpsUpgradeControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_COOKIES)) {
            settings = LuxxleShieldsContentSettingsJni.get().getCookieControlType(host, profile);
        } else if (resourceIndentifier.equals(RESOURCE_IDENTIFIER_TRACKERS)) {
            settings = LuxxleShieldsContentSettingsJni.get().getCosmeticFilteringControlType(
                    host, profile);
            if (settings.equals(BLOCK_THIRDPARTY_RESOURCE)) {
                settings = DEFAULT;
            }
        }
        return settings;
    }

    public static void setFingerprintingPref(String value) {
        setShieldsValue(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                value,
                false);
    }

    public static void setHttpsUpgradePref(String value) {
        setShieldsValue(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_HTTPS_UPGRADE,
                value,
                false);
    }

    public static void setCookiesPref(String value) {
        setShieldsValue(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES,
                value,
                false);
    }

    public static void setTrackersPref(String value) {
        setShieldsValue(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS,
                value,
                false);
    }

    public static void setJavascriptPref(boolean value) {
        setShields(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS,
                value,
                false);
    }

    public static void setForgetFirstPartyStoragePref(boolean value) {
        setShields(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE,
                value,
                false);
    }

    public static boolean getJavascriptPref() {
        return getShields(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS);
    }

    public static String getTrackersPref() {
        return getShieldsValue(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS);
    }

    public static String getFingerprintingPref() {
        return getShieldsValue(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING);
    }

    public static String getHttpsUpgradePref() {
        return getShieldsValue(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_HTTPS_UPGRADE);
    }

    public static boolean getForgetFirstPartyStoragePref() {
        return getShields(
                ProfileManager.getLastUsedRegularProfile(),
                "",
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE);
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeLuxxleShieldsContentSettings == 0;
        mNativeLuxxleShieldsContentSettings = nativePtr;
    }

    @CalledByNative
    private void blockedEvent(int tabId, String blockType, String subresource) {
        for (LuxxleShieldsContentSettingsObserver observer : mLuxxleShieldsContentSettingsObservers) {
            observer.blockEvent(tabId, blockType, subresource);
        }
    }

    @CalledByNative
    private void savedBandwidth(long savings) {
        for (LuxxleShieldsContentSettingsObserver observer : mLuxxleShieldsContentSettingsObservers) {
            observer.savedBandwidth(savings);
        }
    }

    @NativeMethods
    interface Natives {
        void init(LuxxleShieldsContentSettings self);
        void destroy(long nativeLuxxleShieldsContentSettings);

        void setLuxxleShieldsEnabled(boolean enabled, String url, Profile profile);
        boolean getLuxxleShieldsEnabled(String url, Profile profile);
        void setAdControlType(String type, String url, Profile profile);
        String getAdControlType(String url, Profile profile);
        void setCookieControlType(String type, String url, Profile profile);
        String getCookieControlType(String url, Profile profile);
        void setFingerprintingControlType(String type, String url, Profile profile);
        String getFingerprintingControlType(String url, Profile profile);
        void setHttpsUpgradeControlType(String type, String url, Profile profile);
        String getHttpsUpgradeControlType(String url, Profile profile);
        void setNoScriptControlType(String type, String url, Profile profile);
        String getNoScriptControlType(String url, Profile profile);
        void setForgetFirstPartyStorageEnabled(boolean enabled, String url, Profile profile);
        boolean getForgetFirstPartyStorageEnabled(String url, Profile profile);

        void setCosmeticFilteringControlType(String type, String url, Profile profile);

        String getCosmeticFilteringControlType(String url, Profile profile);

        void resetCosmeticFilter(String url);

        boolean areAnyBlockedElementsPresent(String url);
    }
}
