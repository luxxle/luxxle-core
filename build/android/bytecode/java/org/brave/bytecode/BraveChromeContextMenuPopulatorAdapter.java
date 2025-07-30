/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleChromeContextMenuPopulatorAdapter extends LuxxleClassVisitor {
    static String sController =
            "org/chromium/chrome/browser/contextmenu/ChromeContextMenuPopulator";
    static String sLuxxleController =
            "org/chromium/chrome/browser/contextmenu/LuxxleChromeContextMenuPopulator";

    public LuxxleChromeContextMenuPopulatorAdapter(ClassVisitor visitor) {
        super(visitor);
        redirectConstructor(sController, sLuxxleController);

        deleteMethod(sLuxxleController, "getProfile");
        makePublicMethod(sController, "getProfile");

        deleteField(sLuxxleController, "mItemDelegate");
        makeProtectedField(sController, "mItemDelegate");

        deleteField(sLuxxleController, "mParams");
        makeProtectedField(sController, "mParams");
    }
}
