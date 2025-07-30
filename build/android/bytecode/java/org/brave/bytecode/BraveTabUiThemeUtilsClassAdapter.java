/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleTabUiThemeUtilsClassAdapter extends LuxxleClassVisitor {
    static String sTabUiThemeUtilsClassName = "org/chromium/chrome/browser/tab_ui/TabUiThemeUtils";
    static String sLuxxleTabUiThemeUtilsClassName =
            "org/chromium/chrome/browser/tab_ui/LuxxleTabUiThemeUtils";

    public LuxxleTabUiThemeUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sTabUiThemeUtilsClassName, "getTitleTextColor", sLuxxleTabUiThemeUtilsClassName);

        changeMethodOwner(
                sTabUiThemeUtilsClassName,
                "getCardViewBackgroundColor",
                sLuxxleTabUiThemeUtilsClassName);
    }
}
