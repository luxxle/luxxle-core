/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkBridgeClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkBridgeClassName = "org/chromium/chrome/browser/bookmarks/BookmarkBridge";
    static String sLuxxleBookmarkBridgeClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkBridge";

    public LuxxleBookmarkBridgeClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkBridgeClassName, sLuxxleBookmarkBridgeClassName);
        deleteField(sLuxxleBookmarkBridgeClassName, "mNativeBookmarkBridge");
        makeProtectedField(sBookmarkBridgeClassName, "mNativeBookmarkBridge");
    }
}
