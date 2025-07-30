/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleTabUiThemeProviderClassAdapter extends LuxxleClassVisitor {
    static String sTabUiThemeProviderClassName =
            "org/chromium/chrome/browser/tasks/tab_management/TabUiThemeProvider";
    static String sLuxxleTabUiThemeProviderClassName =
            "org/chromium/chrome/browser/tasks/tab_management/LuxxleTabUiThemeProvider";

    public LuxxleTabUiThemeProviderClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sTabUiThemeProviderClassName,
                "getActionButtonTintList",
                sLuxxleTabUiThemeProviderClassName);
    }
}
