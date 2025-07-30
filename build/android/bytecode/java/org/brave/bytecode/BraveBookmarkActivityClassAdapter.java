/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkActivityClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkActivityClassName =
            "org/chromium/chrome/browser/app/bookmarks/BookmarkActivity";
    static String sLuxxleBookmarkActivityClassName =
            "org/chromium/chrome/browser/app/bookmarks/LuxxleBookmarkActivity";

    public LuxxleBookmarkActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sLuxxleBookmarkActivityClassName, "mBookmarkManagerCoordinator");
        makeProtectedField(sBookmarkActivityClassName, "mBookmarkManagerCoordinator");
    }
}
