/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleExternalNavigationHandlerClassAdapter extends LuxxleClassVisitor {
    static String sExternalNavigationHandlerClassName =
            "org/chromium/components/external_intents/ExternalNavigationHandler";
    static String sLuxxleExternalNavigationHandlerClassName =
            "org/chromium/chrome/browser/externalnav/LuxxleExternalNavigationHandler";

    public LuxxleExternalNavigationHandlerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sExternalNavigationHandlerClassName, sLuxxleExternalNavigationHandlerClassName);
    }
}
