/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleStartupHelperClassAdapter extends LuxxleClassVisitor {
    static String sStartupHelperClassName =
            "org/chromium/chrome/browser/tab_group_sync/StartupHelper";
    static String sLuxxleStartupHelperClassName =
            "org/chromium/chrome/browser/tab_group_sync/LuxxleStartupHelper";

    public LuxxleStartupHelperClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sStartupHelperClassName, sLuxxleStartupHelperClassName);
        changeMethodOwner(
                sStartupHelperClassName,
                "handleUnsavedLocalTabGroups",
                sLuxxleStartupHelperClassName);
    }
}
