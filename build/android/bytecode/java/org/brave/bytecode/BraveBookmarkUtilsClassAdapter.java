/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkUtilsClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkUtilsClassName = "org/chromium/chrome/browser/bookmarks/BookmarkUtils";
    static String sLuxxleBookmarkUtilsClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkUtils";

    public LuxxleBookmarkUtilsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sBookmarkUtilsClassName, "addOrEditBookmark", sLuxxleBookmarkUtilsClassName);

        changeMethodOwner(sBookmarkUtilsClassName, "isSpecialFolder", sLuxxleBookmarkUtilsClassName);
    }
}
