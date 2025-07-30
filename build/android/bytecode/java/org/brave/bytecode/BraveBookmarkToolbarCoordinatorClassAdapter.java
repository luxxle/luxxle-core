/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkToolbarCoordinatorClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkToolbarCoordinator";
    static String sLuxxleBookmarkToolbarCoordinatorClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkToolbarCoordinator";

    public LuxxleBookmarkToolbarCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBookmarkToolbarCoordinatorClassName, sLuxxleBookmarkToolbarCoordinatorClassName);

        deleteField(sLuxxleBookmarkToolbarCoordinatorClassName, "mToolbar");
        makeProtectedField(sBookmarkToolbarCoordinatorClassName, "mToolbar");
    }
}
