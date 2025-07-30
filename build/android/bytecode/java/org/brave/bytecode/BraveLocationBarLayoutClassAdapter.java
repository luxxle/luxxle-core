/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleLocationBarLayoutClassAdapter extends LuxxleClassVisitor {
    static String sLuxxleLocationBarLayout =
            "org/chromium/chrome/browser/omnibox/LuxxleLocationBarLayout";
    static String sLocationBarPhone = "org/chromium/chrome/browser/omnibox/LocationBarPhone";
    static String sLocationBarTablet = "org/chromium/chrome/browser/omnibox/LocationBarTablet";
    static String sSearchActivityLocationBarLayout =
            "org/chromium/chrome/browser/searchwidget/SearchActivityLocationBarLayout";

    public LuxxleLocationBarLayoutClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sLocationBarPhone, sLuxxleLocationBarLayout);
        changeSuperName(sLocationBarTablet, sLuxxleLocationBarLayout);
        changeSuperName(sSearchActivityLocationBarLayout, sLuxxleLocationBarLayout);
    }
}
