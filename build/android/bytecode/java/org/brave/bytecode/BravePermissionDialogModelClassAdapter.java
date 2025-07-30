/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxlePermissionDialogModelClassAdapter extends LuxxleClassVisitor {
    static String sPermissionDialogModelClassName =
            "org/chromium/components/permissions/PermissionDialogModelFactory";

    static String sLuxxlePermissionDialogModelClassName =
            "org/chromium/components/permissions/LuxxlePermissionDialogModel";

    public LuxxlePermissionDialogModelClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(
                sPermissionDialogModelClassName, "getModel", sLuxxlePermissionDialogModelClassName);
    }
}
