/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleToolbarPositionControllerClassAdapter extends LuxxleClassVisitor {
    static String sToolbarPositionControllerClassName =
            "org/chromium/chrome/browser/toolbar/ToolbarPositionController";
    static String sLuxxleToolbarPositionControllerClassName =
            "org/chromium/chrome/browser/toolbar/LuxxleToolbarPositionController";

    public LuxxleToolbarPositionControllerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sToolbarPositionControllerClassName,
                "calculateStateTransition",
                sLuxxleToolbarPositionControllerClassName);
    }
}
