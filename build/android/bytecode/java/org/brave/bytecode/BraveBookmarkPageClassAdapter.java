/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkPageClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkPageClassName = "org/chromium/chrome/browser/bookmarks/BookmarkPage";
    static String sLuxxleBookmarkPageClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkPage";

    public LuxxleBookmarkPageClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkPageClassName, sLuxxleBookmarkPageClassName);
        deleteField(sLuxxleBookmarkPageClassName, "mBookmarkManagerCoordinator");
        makeProtectedField(sBookmarkPageClassName, "mBookmarkManagerCoordinator");
    }
}
