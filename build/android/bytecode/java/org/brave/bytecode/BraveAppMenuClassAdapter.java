/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleAppMenuClassAdapter extends LuxxleClassVisitor {
    static String sAppMenuClassName = "org/chromium/chrome/browser/ui/appmenu/AppMenu";

    static String sLuxxleAppMenuClassName = "org/chromium/chrome/browser/ui/appmenu/LuxxleAppMenu";

    public LuxxleAppMenuClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sAppMenuClassName, sLuxxleAppMenuClassName);

        changeMethodOwner(sAppMenuClassName, "getPopupPosition", sLuxxleAppMenuClassName);

        makePublicMethod(sAppMenuClassName, "runMenuItemEnterAnimations");
        addMethodAnnotation(
                sLuxxleAppMenuClassName, "runMenuItemEnterAnimations", "Ljava/lang/Override;");
    }
}
