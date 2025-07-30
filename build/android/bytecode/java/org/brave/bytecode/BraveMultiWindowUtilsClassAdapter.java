/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleMultiWindowUtilsClassAdapter extends LuxxleClassVisitor {
    static String sMultiWindowUtilsClassName =
            "org/chromium/chrome/browser/multiwindow/MultiWindowUtils";
    static String sLuxxleMultiWindowUtilsClassName =
            "org/chromium/chrome/browser/multiwindow/LuxxleMultiWindowUtils";

    public LuxxleMultiWindowUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sMultiWindowUtilsClassName, sLuxxleMultiWindowUtilsClassName);
        changeMethodOwner(
                sMultiWindowUtilsClassName,
                "shouldShowManageWindowsMenu",
                sLuxxleMultiWindowUtilsClassName);
    }
}
