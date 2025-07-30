/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.util;

import android.app.Activity;

import com.google.android.material.color.DynamicColorsOptions;

public class LuxxleDynamicColors {
    public static void applyToActivityIfAvailable(Activity activity) {
        // We disable dynamic colors as it causes styling issues with Luxxle theme.
    }

    public static void applyToActivityIfAvailable(
            Activity activity, DynamicColorsOptions dynamicColorsOptions) {
        // We disable dynamic colors as it causes styling issues with Luxxle theme.
    }
}
