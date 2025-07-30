/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxlePureJavaExceptionReporterClassAdapter extends LuxxleClassVisitor {
    static String sChromePureJavaExceptionReporterClassName =
            "org/chromium/chrome/browser/crash/ChromePureJavaExceptionReporter";
    static String sLuxxlePureJavaExceptionReporterClassName =
            "org/chromium/chrome/browser/crash/LuxxlePureJavaExceptionReporter";

    public LuxxlePureJavaExceptionReporterClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sChromePureJavaExceptionReporterClassName,
                sLuxxlePureJavaExceptionReporterClassName);
    }
}
