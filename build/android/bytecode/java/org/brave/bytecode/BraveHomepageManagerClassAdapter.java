/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleHomepageManagerClassAdapter extends LuxxleClassVisitor {
    static String sHomepageManagerClassName =
            "org/chromium/chrome/browser/homepage/HomepageManager";
    static String sLuxxleHomepageManagerClassName =
            "org/chromium/chrome/browser/homepage/LuxxleHomepageManager";

    public LuxxleHomepageManagerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sHomepageManagerClassName, sLuxxleHomepageManagerClassName);
    }
}
