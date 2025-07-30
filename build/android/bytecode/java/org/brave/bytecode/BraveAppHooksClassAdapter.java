/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleAppHooksClassAdapter extends LuxxleClassVisitor {
    static String sAppHooksClassName = "org/chromium/chrome/browser/AppHooks";
    static String sLuxxleAppHooksClassName = "org/chromium/chrome/browser/LuxxleAppHooks";

    public LuxxleAppHooksClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sAppHooksClassName, sLuxxleAppHooksClassName);
    }
}
