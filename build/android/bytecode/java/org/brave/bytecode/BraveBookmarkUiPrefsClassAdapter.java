/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBookmarkUiPrefsClassAdapter extends LuxxleClassVisitor {
    static String sBookmarkUiPrefsClassName =
            "org/chromium/chrome/browser/bookmarks/BookmarkUiPrefs";
    static String sLuxxleBookmarkUiPrefsClassName =
            "org/chromium/chrome/browser/bookmarks/LuxxleBookmarkUiPrefs";

    public LuxxleBookmarkUiPrefsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sBookmarkUiPrefsClassName, sLuxxleBookmarkUiPrefsClassName);
    }
}
