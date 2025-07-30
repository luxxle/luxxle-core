/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkManagerCoordinatorClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkManagerCoordinatorClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkManagerCoordinator";
    static String sLuxxleBookmarkManagerCoordinatorClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkManagerCoordinator";

    public LuxxleBookmarkManagerCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBookmarkManagerCoordinatorClassName, sLuxxleBookmarkManagerCoordinatorClassName);
        deleteField(sLuxxleBookmarkManagerCoordinatorClassName, "mMediator");
        makeProtectedField(sBookmarkManagerCoordinatorClassName, "mMediator");
    }
}
