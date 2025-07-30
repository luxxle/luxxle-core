/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleForegroundServiceUtilsClassAdapter extends LuxxleClassVisitor {
    static String sForegroundServiceUtilsClassName =
            "org/chromium/components/browser_ui/notifications/ForegroundServiceUtils";

    static String sLuxxleForegroundServiceUtilsClassName =
            "org/chromium/components/browser_ui/notifications/LuxxleForegroundServiceUtils";

    public LuxxleForegroundServiceUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sForegroundServiceUtilsClassName, sLuxxleForegroundServiceUtilsClassName);
    }
}
