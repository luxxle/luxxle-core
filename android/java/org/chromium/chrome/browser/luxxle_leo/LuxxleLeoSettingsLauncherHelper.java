/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.luxxle_leo;

import android.app.Activity;
import android.content.Context;
import android.view.inputmethod.InputMethodManager;

import org.jni_zero.CalledByNative;

import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.settings.LuxxleLeoPreferences;
import org.chromium.chrome.browser.settings.SettingsNavigationFactory;
import org.chromium.chrome.browser.util.TabUtils;
import org.chromium.components.browser_ui.settings.SettingsNavigation;
import org.chromium.content_public.browser.WebContents;
import org.chromium.ui.base.WindowAndroid;

/** Launches Luxxle Leo settings page or subscription. */
public class LuxxleLeoSettingsLauncherHelper {
    private static final String ACCOUNT_PAGE_URL = "https://account.luxxle.com/";
    private static SettingsNavigation sLauncher;

    @CalledByNative
    private static void showLuxxleLeoSettings(WebContents webContents) {
        Context context = webContents.getTopLevelNativeWindow().getActivity().get();
        if (context == null) {
            return;
        }
        getLauncher().startSettings(context, LuxxleLeoPreferences.class);
    }

    @CalledByNative
    private static void goPremium(WebContents webContents) {
        LuxxleLeoUtils.goPremium(webContents.getTopLevelNativeWindow().getActivity().get());
    }

    @CalledByNative
    private static void managePremium(WebContents webContents) {
        if (LuxxleLeoPrefUtils.getIsSubscriptionActive(Profile.fromWebContents(webContents))) {
            LuxxleLeoUtils.openManageSubscription();
        } else {
            TabUtils.openURLWithLuxxleActivity(ACCOUNT_PAGE_URL);
        }
    }

    @CalledByNative
    private static void openURL(String url) {
        TabUtils.openURLWithLuxxleActivity(url);
    }

    @CalledByNative
    private static void handleVoiceRecognition(WebContents webContents, String conversationUuid) {
        new LuxxleLeoVoiceRecognitionHandler(
                        webContents.getTopLevelNativeWindow(), webContents, conversationUuid)
                .startVoiceRecognition();
    }

    @CalledByNative
    private static void handleShowSoftKeyboard(WebContents webContents) {
        WindowAndroid windowAndroid = webContents.getTopLevelNativeWindow();
        if (windowAndroid == null) {
            return;
        }
        Activity activity = windowAndroid.getActivity().get();
        if (activity == null) {
            return;
        }
        InputMethodManager imm =
                (InputMethodManager) activity.getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(activity.getWindow().getCurrentFocus(), 0);
    }

    @CalledByNative
    private static void closeActivity(WebContents webContents) {
        WindowAndroid windowAndroid = webContents.getTopLevelNativeWindow();
        if (windowAndroid == null) {
            return;
        }
        Activity activity = windowAndroid.getActivity().get();
        if (activity == null) {
            return;
        }
        activity.finish();
    }

    private static SettingsNavigation getLauncher() {
        return sLauncher != null ? sLauncher : SettingsNavigationFactory.createSettingsNavigation();
    }
}
