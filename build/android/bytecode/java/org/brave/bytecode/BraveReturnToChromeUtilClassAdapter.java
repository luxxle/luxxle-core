/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleReturnToChromeUtilClassAdapter extends LuxxleClassVisitor {
    static String sReturnToChromeUtilClassName =
            "org/chromium/chrome/browser/tasks/ReturnToChromeUtil";
    static String sLuxxleReturnToChromeUtilClassName =
            "org/chromium/chrome/browser/tasks/LuxxleReturnToChromeUtil";

    public LuxxleReturnToChromeUtilClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sReturnToChromeUtilClassName,
                "shouldShowNtpAsHomeSurfaceAtStartup",
                sLuxxleReturnToChromeUtilClassName);
    }
}
