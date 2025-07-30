/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleFreIntentCreatorClassAdapter extends LuxxleClassVisitor {
    static String sFreIntentCreatorClassName =
            "org/chromium/chrome/browser/firstrun/FreIntentCreator";

    static String sLuxxleFreIntentCreatorClassName =
            "org/chromium/chrome/browser/firstrun/LuxxleFreIntentCreator";

    public LuxxleFreIntentCreatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sFreIntentCreatorClassName, sLuxxleFreIntentCreatorClassName);
    }
}
