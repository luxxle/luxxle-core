/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleNewTabPageLayoutClassAdapter extends LuxxleClassVisitor {
    static String sNewTabPageLayoutClassName = "org/chromium/chrome/browser/ntp/NewTabPageLayout";
    static String sLuxxleNewTabPageLayoutClassName =
            "org/chromium/chrome/browser/ntp/LuxxleNewTabPageLayout";
    static String sNewTabPageLayoutSuperClassName = "android/widget/FrameLayout";

    public LuxxleNewTabPageLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sLuxxleNewTabPageLayoutClassName, "mMvTilesContainerLayout");
        makeProtectedField(sNewTabPageLayoutClassName, "mMvTilesContainerLayout");

        deleteField(sLuxxleNewTabPageLayoutClassName, "mLogoCoordinator");
        makeProtectedField(sNewTabPageLayoutClassName, "mLogoCoordinator");

        deleteField(sLuxxleNewTabPageLayoutClassName, "mInitialTileNum");
        makeProtectedField(sNewTabPageLayoutClassName, "mInitialTileNum");

        deleteField(sLuxxleNewTabPageLayoutClassName, "mProfile");
        makeProtectedField(sNewTabPageLayoutClassName, "mProfile");

        makePublicMethod(sNewTabPageLayoutClassName, "insertSiteSectionView");
        addMethodAnnotation(
                sLuxxleNewTabPageLayoutClassName, "insertSiteSectionView", "Ljava/lang/Override;");

        makePublicMethod(sNewTabPageLayoutClassName, "setSearchProviderTopMargin");
        addMethodAnnotation(
                sLuxxleNewTabPageLayoutClassName,
                "setSearchProviderTopMargin",
                "Ljava/lang/Override;");

        makePublicMethod(sNewTabPageLayoutClassName, "setSearchProviderBottomMargin");
        addMethodAnnotation(
                sLuxxleNewTabPageLayoutClassName,
                "setSearchProviderBottomMargin",
                "Ljava/lang/Override;");

        deleteMethod(sLuxxleNewTabPageLayoutClassName, "getLogoMargin");
        makePublicMethod(sNewTabPageLayoutClassName, "getLogoMargin");

        changeSuperName(sNewTabPageLayoutClassName, sNewTabPageLayoutSuperClassName);
    }
}
