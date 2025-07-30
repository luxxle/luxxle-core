/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleReaderModeManagerClassAdapter extends LuxxleClassVisitor {
    static String sReaderModeManagerClassName =
            "org/chromium/chrome/browser/dom_distiller/ReaderModeManager";
    static String sLuxxleReaderModeManagerClassName =
            "org/chromium/chrome/browser/dom_distiller/LuxxleReaderModeManager";

    public LuxxleReaderModeManagerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sReaderModeManagerClassName, sLuxxleReaderModeManagerClassName);

        deleteField(sLuxxleReaderModeManagerClassName, "mTab");
        makeProtectedField(sReaderModeManagerClassName, "mTab");
    }
}
