/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleLogoMediatorClassAdapter extends LuxxleClassVisitor {
    static String sLogoMediator = "org/chromium/chrome/browser/logo/LogoMediator";
    static String sLuxxleLogoMediator = "org/chromium/chrome/browser/logo/LuxxleLogoMediator";

    public LuxxleLogoMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sLogoMediator, sLuxxleLogoMediator);

        deleteField(sLuxxleLogoMediator, "mLogoModel");
        makeProtectedField(sLogoMediator, "mLogoModel");

        deleteField(sLuxxleLogoMediator, "mShouldShowLogo");
        makeProtectedField(sLogoMediator, "mShouldShowLogo");

        makePublicMethod(sLogoMediator, "updateVisibility");
        addMethodAnnotation(sLuxxleLogoMediator, "updateVisibility", "Ljava/lang/Override;");
    }
}
