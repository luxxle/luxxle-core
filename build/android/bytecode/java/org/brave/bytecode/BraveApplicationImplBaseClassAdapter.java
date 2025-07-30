/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleApplicationImplBaseClassAdapter extends LuxxleClassVisitor {
    static String sChromeApplicationImplClassName =
            "org/chromium/chrome/browser/ChromeApplicationImpl";

    static String sLuxxleApplicationImplBaseClassName =
            "org/chromium/chrome/browser/LuxxleApplicationImplBase";

    public LuxxleApplicationImplBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sChromeApplicationImplClassName, sLuxxleApplicationImplBaseClassName);
    }
}
