/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.search_engines.settings;

import org.jni_zero.NativeMethods;

import org.chromium.base.ThreadUtils;

public class LuxxleSearchEnginePrefHelper {
    private static LuxxleSearchEnginePrefHelper sInstance;

    private LuxxleSearchEnginePrefHelper() {}

    public static LuxxleSearchEnginePrefHelper getInstance() {
        ThreadUtils.assertOnUiThread();
        if (sInstance == null) {
            sInstance = new LuxxleSearchEnginePrefHelper();
        }
        return sInstance;
    }

    public void setFetchSEFromNative(boolean value) {
        LuxxleSearchEnginePrefHelperJni.get().setFetchSEFromNative(value);
    }

    public boolean getFetchSEFromNative() {
        return LuxxleSearchEnginePrefHelperJni.get().getFetchSEFromNative();
    }

    @NativeMethods
    interface Natives {
        void setFetchSEFromNative(boolean value);
        boolean getFetchSEFromNative();
    }
}
