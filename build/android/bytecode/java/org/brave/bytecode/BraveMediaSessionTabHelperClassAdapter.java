/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleMediaSessionTabHelperClassAdapter extends LuxxleClassVisitor {
    static String sMediaSessionTabHelper =
            "org/chromium/chrome/browser/media/ui/MediaSessionTabHelper";
    static String sLuxxleMediaSessionTabHelper =
            "org/chromium/chrome/browser/media/ui/LuxxleMediaSessionTabHelper";

    public LuxxleMediaSessionTabHelperClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sMediaSessionTabHelper, sLuxxleMediaSessionTabHelper);
        deleteField(sLuxxleMediaSessionTabHelper, "mTab");
        makeProtectedField(sMediaSessionTabHelper, "mTab");
    }
}
