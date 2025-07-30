/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleLocationBarMediatorClassAdapter extends LuxxleClassVisitor {
    static String sLocationBarMediator = "org/chromium/chrome/browser/omnibox/LocationBarMediator";
    static String sLuxxleLocationBarMediator =
            "org/chromium/chrome/browser/omnibox/LuxxleLocationBarMediator";

    public LuxxleLocationBarMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sLocationBarMediator, sLuxxleLocationBarMediator);

        deleteMethod(sLuxxleLocationBarMediator, "shouldShowDeleteButton");
        makePublicMethod(sLocationBarMediator, "shouldShowDeleteButton");

        deleteField(sLuxxleLocationBarMediator, "mNativeInitialized");
        makeProtectedField(sLocationBarMediator, "mNativeInitialized");
        deleteField(sLuxxleLocationBarMediator, "mWindowAndroid");
        makeProtectedField(sLocationBarMediator, "mWindowAndroid");
        deleteField(sLuxxleLocationBarMediator, "mLocationBarLayout");
        makeProtectedField(sLocationBarMediator, "mLocationBarLayout");
        deleteField(sLuxxleLocationBarMediator, "mIsUrlFocusChangeInProgress");
        makeProtectedField(sLocationBarMediator, "mIsUrlFocusChangeInProgress");
        deleteField(sLuxxleLocationBarMediator, "mUrlHasFocus");
        makeProtectedField(sLocationBarMediator, "mUrlHasFocus");
        deleteField(sLuxxleLocationBarMediator, "mIsTablet");
        makeProtectedField(sLocationBarMediator, "mIsTablet");
        deleteField(sLuxxleLocationBarMediator, "mIsLocationBarFocusedFromNtpScroll");
        makeProtectedField(sLocationBarMediator, "mIsLocationBarFocusedFromNtpScroll");
        deleteField(sLuxxleLocationBarMediator, "mContext");
        makeProtectedField(sLocationBarMediator, "mContext");
        deleteField(sLuxxleLocationBarMediator, "mBrandedColorScheme");
        makeProtectedField(sLocationBarMediator, "mBrandedColorScheme");
        deleteField(sLuxxleLocationBarMediator, "mTemplateUrlServiceSupplier");
        makeProtectedField(sLocationBarMediator, "mTemplateUrlServiceSupplier");
    }
}
