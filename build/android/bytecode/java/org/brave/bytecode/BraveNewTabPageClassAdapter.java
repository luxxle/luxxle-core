/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleNewTabPageClassAdapter extends LuxxleClassVisitor {
    static String sNewTabPageClassName = "org/chromium/chrome/browser/ntp/NewTabPage";
    static String sLuxxleNewTabPageClassName = "org/chromium/chrome/browser/ntp/LuxxleNewTabPage";

    public LuxxleNewTabPageClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sNewTabPageClassName, sLuxxleNewTabPageClassName);

        deleteField(sLuxxleNewTabPageClassName, "mBrowserControlsStateProvider");
        makeProtectedField(sNewTabPageClassName, "mBrowserControlsStateProvider");

        deleteField(sLuxxleNewTabPageClassName, "mNewTabPageLayout");
        makeProtectedField(sNewTabPageClassName, "mNewTabPageLayout");

        deleteField(sLuxxleNewTabPageClassName, "mFeedSurfaceProvider");
        makeProtectedField(sNewTabPageClassName, "mFeedSurfaceProvider");

        deleteField(sLuxxleNewTabPageClassName, "mToolbarSupplier");
        makeProtectedField(sNewTabPageClassName, "mToolbarSupplier");

        deleteField(sLuxxleNewTabPageClassName, "mBottomSheetController");
        makeProtectedField(sNewTabPageClassName, "mBottomSheetController");

        deleteField(sLuxxleNewTabPageClassName, "mTabStripHeightSupplier");
        makeProtectedField(sNewTabPageClassName, "mTabStripHeightSupplier");

        makePublicMethod(sNewTabPageClassName, "updateSearchProviderHasLogo");
        addMethodAnnotation(
                sLuxxleNewTabPageClassName, "updateSearchProviderHasLogo", "Ljava/lang/Override;");
    }
}
