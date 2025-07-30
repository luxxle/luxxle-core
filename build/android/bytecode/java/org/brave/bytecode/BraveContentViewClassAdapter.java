/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleContentViewClassAdapter extends LuxxleClassVisitor {
    static String sContentViewClassName =
            "org/chromium/components/embedder_support/view/ContentView";
    static String sLuxxleContentViewClassName =
            "org/chromium/components/embedder_support/view/LuxxleContentView";

    public LuxxleContentViewClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sContentViewClassName, sLuxxleContentViewClassName);
    }
}
