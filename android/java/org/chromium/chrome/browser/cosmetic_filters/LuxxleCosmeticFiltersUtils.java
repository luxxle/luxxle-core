/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.cosmetic_filters;

import org.jni_zero.CalledByNative;
import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;

import org.chromium.base.Log;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.tab.Tab;

@JNINamespace("cosmetic_filters")
public class LuxxleCosmeticFiltersUtils {
    private static final String TAG = "CosmeticFiltersUtils";

    public static boolean launchContentPickerForWebContent(Tab tab) {
        return LuxxleCosmeticFiltersUtilsJni.get().launchContentPickerForWebContent(tab);
    }

    @CalledByNative
    public static void showCustomFilterSettings() {
        try {
            LuxxleActivity.getLuxxleActivity().openLuxxleCreateCustomFiltersSettings();
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "open create custom filter settings" + e);
        }
    }

    @CalledByNative
    public static int getThemeBackgroundColor() {
        int backgroundColor = 0;
        try {
            backgroundColor = LuxxleActivity.getLuxxleActivity().getLuxxleThemeBackgroundColor();
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "Get theme background color" + e);
        }
        return backgroundColor;
    }

    @NativeMethods
    interface Natives {
        boolean launchContentPickerForWebContent(Tab tab);
    }
}
