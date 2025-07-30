/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleIdentityDiscControllerClassAdapter extends LuxxleClassVisitor {
    static String sIdentityDiscControllerClassName =
            "org/chromium/chrome/browser/identity_disc/IdentityDiscController";
    static String sLuxxleIdentityDiscControllerClassName =
            "org/chromium/chrome/browser/identity_disc/LuxxleIdentityDiscController";

    public LuxxleIdentityDiscControllerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sIdentityDiscControllerClassName, sLuxxleIdentityDiscControllerClassName);

        makePublicMethod(sIdentityDiscControllerClassName, "calculateButtonData");
        addMethodAnnotation(
                sLuxxleIdentityDiscControllerClassName,
                "calculateButtonData",
                "Ljava/lang/Override;");
    }
}
