/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkDelegateClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkDelegateClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkDelegate";
    static String sLuxxleBookmarkDelegateClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkDelegate";

    public LuxxleBookmarkDelegateClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkDelegateClassName, sLuxxleBookmarkDelegateClassName);
    }
}
