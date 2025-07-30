/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleLauncherActivityClassAdapter extends LuxxleClassVisitor {
    static String sChromeLauncherActivityClassName =
            "org/chromium/chrome/browser/document/ChromeLauncherActivity";
    static String sLuxxleLauncherActivityClassName =
            "org/chromium/chrome/browser/document/LuxxleLauncherActivity";

    public LuxxleLauncherActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sChromeLauncherActivityClassName, sLuxxleLauncherActivityClassName);
    }
}
