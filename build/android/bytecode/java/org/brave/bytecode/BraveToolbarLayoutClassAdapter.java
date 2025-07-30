/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleToolbarLayoutClassAdapter extends LuxxleClassVisitor {
    static String sCustomTabToolbarClassName =
            "org/chromium/chrome/browser/customtabs/features/toolbar/CustomTabToolbar";
    static String sToolbarPhoneClassName = "org/chromium/chrome/browser/toolbar/top/ToolbarPhone";
    static String sToolbarTabletClassName = "org/chromium/chrome/browser/toolbar/top/ToolbarTablet";
    static String sLuxxleToolbarLayoutClassName =
            "org/chromium/chrome/browser/toolbar/top/LuxxleToolbarLayoutImpl";

    public LuxxleToolbarLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sCustomTabToolbarClassName, sLuxxleToolbarLayoutClassName);

        changeSuperName(sToolbarPhoneClassName, sLuxxleToolbarLayoutClassName);

        changeSuperName(sToolbarTabletClassName, sLuxxleToolbarLayoutClassName);

        deleteMethod(sToolbarPhoneClassName, "onHomeButtonUpdate");
    }
}
