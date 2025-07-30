/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleTabGroupModelFilterClassAdapter extends LuxxleClassVisitor {
    static String sTabGroupModelFilterClassName =
            "org/chromium/chrome/browser/tabmodel/TabGroupModelFilterImpl";
    static String sLuxxleTabGroupModelFilterClassName =
            "org/chromium/chrome/browser/tabmodel/LuxxleTabGroupModelFilter";

    public LuxxleTabGroupModelFilterClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sTabGroupModelFilterClassName, sLuxxleTabGroupModelFilterClassName);

        deleteField(sTabGroupModelFilterClassName, "mIsResetting");

        changeMethodOwner(
                sTabGroupModelFilterClassName,
                "shouldUseParentIds",
                sLuxxleTabGroupModelFilterClassName);
    }
}
