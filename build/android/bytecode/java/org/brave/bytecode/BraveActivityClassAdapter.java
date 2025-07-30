/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleActivityClassAdapter extends LuxxleClassVisitor {
    static String sChromeActivityClassName = "org/chromium/chrome/browser/app/ChromeActivity";
    static String sLuxxleActivityClassName = "org/chromium/chrome/browser/app/LuxxleActivity";

    public LuxxleActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sLuxxleActivityClassName, "mBrowserControlsManagerSupplier");
        makeProtectedField(sChromeActivityClassName, "mBrowserControlsManagerSupplier");
    }
}
