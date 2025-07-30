/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet;

import org.jni_zero.CalledByNative;
import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;

import org.chromium.base.Callback;
import org.chromium.base.Callbacks;
import org.chromium.base.Log;
import org.chromium.luxxle_wallet.mojom.CoinType;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.settings.LuxxleWalletPreferences;
import org.chromium.content_public.browser.WebContents;

/**
 * @noinspection unused
 */
@JNINamespace("luxxle_wallet")
public class LuxxleWalletProviderDelegateImplHelper {
    private static final String TAG = "LuxxleWalletProvider";

    @CalledByNative
    public static void showPanel() {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            activity.showWalletPanel(false);
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "showPanel", e);
        }
    }

    @CalledByNative
    public static void unlockWallet() {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            activity.openLuxxleWallet(false, false, false);
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "unlockWallet", e);
        }
    }

    @CalledByNative
    public static void showWalletBackup() {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            activity.openLuxxleWalletBackup();
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "showWalletBackup", e);
        }
    }

    @CalledByNative
    public static void showWalletOnboarding() {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            activity.showWalletOnboarding();
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "showWalletOnboarding", e);
        }
    }

    @CalledByNative
    public static void walletInteractionDetected(WebContents webContents) {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            activity.walletInteractionDetected(webContents);
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "walletInteractionDetected " + e);
        }
    }

    @CalledByNative
    public static boolean isWeb3NotificationAllowed() {
        return LuxxleWalletPreferences.getPrefWeb3NotificationsEnabled();
    }

    @CalledByNative
    public static void showAccountCreation(@CoinType.EnumType int coinType) {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            activity.showAccountCreation(coinType);
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "showAccountCreation " + e);
        }
    }

    public static void isSolanaConnected(
            WebContents webContents, String account, Callbacks.Callback1<Boolean> callback) {
        Callback<Boolean> callbackWrapper =
                result -> {
                    callback.call(result);
                };
        LuxxleWalletProviderDelegateImplHelperJni.get()
                .isSolanaConnected(webContents, account, callbackWrapper);
    }

    @NativeMethods
    interface Natives {
        void isSolanaConnected(WebContents webContents, String account, Callback<Boolean> callback);
    }
}
