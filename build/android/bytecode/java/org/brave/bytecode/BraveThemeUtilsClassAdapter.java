/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleThemeUtilsClassAdapter extends LuxxleClassVisitor {
    static String sThemeUtilsClassName = "org/chromium/chrome/browser/theme/ThemeUtils";
    static String sLuxxleThemeUtilsClassName = "org/chromium/chrome/browser/theme/LuxxleThemeUtils";

    public LuxxleThemeUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sThemeUtilsClassName,
                "getTextBoxColorForToolbarBackgroundInNonNativePage", sLuxxleThemeUtilsClassName);
    }
}
