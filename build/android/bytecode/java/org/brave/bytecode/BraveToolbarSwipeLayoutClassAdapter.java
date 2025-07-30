/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleToolbarSwipeLayoutClassAdapter extends LuxxleClassVisitor {
    static String sToolbarSwipeLayoutClassName =
            "org/chromium/chrome/browser/compositor/layouts/ToolbarSwipeLayout";
    static String sLuxxleToolbarSwipeLayoutClassName =
            "org/chromium/chrome/browser/compositor/layouts/LuxxleToolbarSwipeLayout";

    public LuxxleToolbarSwipeLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sToolbarSwipeLayoutClassName, sLuxxleToolbarSwipeLayoutClassName);

        deleteField(sLuxxleToolbarSwipeLayoutClassName, "mMoveToolbar");
        makeProtectedField(sToolbarSwipeLayoutClassName, "mMoveToolbar");
    }
}
