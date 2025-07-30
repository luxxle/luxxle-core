/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleTabbedNavigationBarColorControllerBaseClassAdapter extends LuxxleClassVisitor {
    static String sTabbedNavigationBarColorControllerClassName =
            "org/chromium/chrome/browser/tabbed_mode/TabbedNavigationBarColorController";
    static String sLuxxleTabbedNavigationBarColorControllerBaseClassName =
            "org/chromium/chrome/browser/tabbed_mode/LuxxleTabbedNavigationBarColorControllerBase";

    public LuxxleTabbedNavigationBarColorControllerBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(
                sTabbedNavigationBarColorControllerClassName,
                sLuxxleTabbedNavigationBarColorControllerBaseClassName);

        deleteField(sTabbedNavigationBarColorControllerClassName, "mContext");

        deleteField(sTabbedNavigationBarColorControllerClassName, "mActiveTab");

        deleteField(sTabbedNavigationBarColorControllerClassName, "mTabModelSelector");

        changeMethodOwner(
                sTabbedNavigationBarColorControllerClassName,
                "getNavigationBarColor",
                sLuxxleTabbedNavigationBarColorControllerBaseClassName);

        changeMethodOwner(
                sTabbedNavigationBarColorControllerClassName,
                "useActiveTabColor",
                sLuxxleTabbedNavigationBarColorControllerBaseClassName);
    }
}
