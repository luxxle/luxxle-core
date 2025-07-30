/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleCachedFlagClassAdapter extends LuxxleClassVisitor {
    static String sCachedFlagClassName = "org/chromium/components/cached_flags/CachedFlag";
    static String sLuxxleCachedFlagClassName =
            "org/chromium/components/cached_flags/LuxxleCachedFlag";

    public LuxxleCachedFlagClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sCachedFlagClassName, sLuxxleCachedFlagClassName);

        deleteField(sLuxxleCachedFlagClassName, "mDefaultValue");
        makeProtectedField(sCachedFlagClassName, "mDefaultValue");
    }
}
