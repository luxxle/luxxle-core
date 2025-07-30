/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkManagerMediatorClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkManagerMediatorClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkManagerMediator";
    static String sLuxxleBookmarkManagerMediatorClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkManagerMediator";

    public LuxxleBookmarkManagerMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sBookmarkManagerMediatorClassName, sLuxxleBookmarkManagerMediatorClassName);

        deleteField(sLuxxleBookmarkManagerMediatorClassName, "mBookmarkModel");
        makeProtectedField(sBookmarkManagerMediatorClassName, "mBookmarkModel");
        deleteField(sLuxxleBookmarkManagerMediatorClassName, "mContext");
        makeProtectedField(sBookmarkManagerMediatorClassName, "mContext");
    }
}
