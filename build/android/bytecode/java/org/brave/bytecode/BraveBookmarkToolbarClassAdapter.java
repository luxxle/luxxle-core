/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkToolbarClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkToolbarClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkToolbar";
    static String sLuxxleBookmarkToolbarClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkToolbar";

    public LuxxleBookmarkToolbarClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkToolbarClassName, sLuxxleBookmarkToolbarClassName);
        deleteField(sLuxxleBookmarkToolbarClassName, "mBookmarkModel");
        makeProtectedField(sBookmarkToolbarClassName, "mBookmarkModel");
    }
}
