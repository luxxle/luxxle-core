/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleShareDelegateImplClassAdapter extends LuxxleClassVisitor {
    static String sShareDelegateImplClassName =
            "org/chromium/chrome/browser/share/ShareDelegateImpl";
    static String sLuxxleShareDelegateImplClassName =
            "org/chromium/chrome/browser/share/LuxxleShareDelegateImpl";

    public LuxxleShareDelegateImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sShareDelegateImplClassName, sLuxxleShareDelegateImplClassName);
    }
}
