/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleSystemAccountManagerDelegateAdapter extends LuxxleClassVisitor {
    static String sSystemAccountManagerDelegateClassName =
            "org/chromium/components/signin/SystemAccountManagerDelegate";
    static String sLuxxleSystemAccountManagerDelegateClassName =
            "org/chromium/components/signin/LuxxleSystemAccountManagerDelegate";

    public LuxxleSystemAccountManagerDelegateAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sSystemAccountManagerDelegateClassName,
                sLuxxleSystemAccountManagerDelegateClassName);
    }
}
