/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleMultiInstanceManagerApi31ClassAdapter extends LuxxleClassVisitor {
    static String sMultiInstanceManagerApi31 =
            "org/chromium/chrome/browser/multiwindow/MultiInstanceManagerApi31";
    static String sLuxxleMultiInstanceManagerApi31 =
            "org/chromium/chrome/browser/multiwindow/LuxxleMultiInstanceManagerApi31";

    public LuxxleMultiInstanceManagerApi31ClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sMultiInstanceManagerApi31, sLuxxleMultiInstanceManagerApi31);

        deleteField(sLuxxleMultiInstanceManagerApi31, "mInstanceId");
        makeProtectedField(sMultiInstanceManagerApi31, "mInstanceId");
    }
}
