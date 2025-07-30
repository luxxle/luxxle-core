/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleStatusMediatorClassAdapter extends LuxxleClassVisitor {
    static String sStatusMediatorClassName =
            "org/chromium/chrome/browser/omnibox/status/StatusMediator";
    static String sLuxxleStatusMediatorClassName =
            "org/chromium/chrome/browser/omnibox/status/LuxxleStatusMediator";

    public LuxxleStatusMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sStatusMediatorClassName, sLuxxleStatusMediatorClassName);

        deleteField(sLuxxleStatusMediatorClassName, "mUrlHasFocus");
        makeProtectedField(sStatusMediatorClassName, "mUrlHasFocus");
    }
}
