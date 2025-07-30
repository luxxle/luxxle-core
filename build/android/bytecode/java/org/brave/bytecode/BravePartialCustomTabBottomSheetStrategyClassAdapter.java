/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxlePartialCustomTabBottomSheetStrategyClassAdapter extends LuxxleClassVisitor {
    static String sNamespace = "org/chromium/chrome/browser/customtabs/features/partialcustomtab/";
    static String sStrategy = sNamespace + "PartialCustomTabBottomSheetStrategy";
    static String sLuxxleStrategy = sNamespace + "LuxxlePartialCustomTabBottomSheetStrategy";

    LuxxlePartialCustomTabBottomSheetStrategyClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sStrategy, sLuxxleStrategy);

        deleteField(sLuxxleStrategy, "mStopShowingSpinner");
        makeProtectedField(sStrategy, "mStopShowingSpinner");

        deleteField(sLuxxleStrategy, "mTab");
        makeProtectedField(sStrategy, "mTab");
    }
}
