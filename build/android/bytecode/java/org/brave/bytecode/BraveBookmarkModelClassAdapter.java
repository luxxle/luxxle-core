/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkModelClassAdapter extends LuxxleClassVisitor {
    static String sLuxxleBookmarkBridgeClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkBridge";
    static String sBookmarkModelClassName = "org/chromium/chrome/browser/bookmarks/BookmarkModel";
    static String sLuxxleBookmarkModelClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkModel";

    public LuxxleBookmarkModelClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sBookmarkModelClassName, sLuxxleBookmarkBridgeClassName);
        redirectConstructor(sBookmarkModelClassName, sLuxxleBookmarkModelClassName);
        deleteMethod(sLuxxleBookmarkModelClassName, "importBookmarks");
        deleteMethod(sLuxxleBookmarkModelClassName, "exportBookmarks");
    }
}
