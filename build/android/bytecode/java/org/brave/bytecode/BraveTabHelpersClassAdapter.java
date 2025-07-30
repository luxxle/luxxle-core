/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleTabHelpersClassAdapter extends LuxxleClassVisitor {
    static String sTabHelpersClassName = "org/chromium/chrome/browser/tab/TabHelpers";
    static String sLuxxleTabHelpersClassName = "org/chromium/chrome/browser/tab/LuxxleTabHelpers";

    public LuxxleTabHelpersClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sTabHelpersClassName, "initTabHelpers", sLuxxleTabHelpersClassName);
    }
}
