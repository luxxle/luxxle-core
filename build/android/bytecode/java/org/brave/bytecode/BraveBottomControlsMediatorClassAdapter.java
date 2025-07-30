/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBottomControlsMediatorClassAdapter extends LuxxleClassVisitor {
    static String sBottomControlsMediatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/BottomControlsMediator";
    static String sLuxxleBottomControlsMediatorClassName =
            "org/chromium/chrome/browser/toolbar/bottom/LuxxleBottomControlsMediator";

    public LuxxleBottomControlsMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBottomControlsMediatorClassName, sLuxxleBottomControlsMediatorClassName);

        deleteField(sLuxxleBottomControlsMediatorClassName, "mModel");
        makeProtectedField(sBottomControlsMediatorClassName, "mModel");

        deleteField(sLuxxleBottomControlsMediatorClassName, "mBottomControlsStacker");
        makeProtectedField(sBottomControlsMediatorClassName, "mBottomControlsStacker");
    }
}
