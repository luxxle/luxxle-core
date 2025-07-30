/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleStatusBarColorControllerClassAdapter extends LuxxleClassVisitor {
    static String sStatusBarColorControllerClassName =
            "org/chromium/chrome/browser/ui/system/StatusBarColorController";
    static String sLuxxleStatusBarColorControllerClassName =
            "org/chromium/chrome/browser/ui/system/LuxxleStatusBarColorController";

    public LuxxleStatusBarColorControllerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sStatusBarColorControllerClassName, sLuxxleStatusBarColorControllerClassName);

        deleteField(sLuxxleStatusBarColorControllerClassName, "mBackgroundColorForNtp");
        makeProtectedField(sStatusBarColorControllerClassName, "mBackgroundColorForNtp");
    }
}
