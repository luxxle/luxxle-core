/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleMediaSessionHelperClassAdapter extends LuxxleClassVisitor {
    static String sMediaSessionHelperClassName =
            "org/chromium/components/browser_ui/media/MediaSessionHelper";

    static String sLuxxleMediaSessionHelperClassName =
            "org/chromium/components/browser_ui/media/LuxxleMediaSessionHelper";

    public LuxxleMediaSessionHelperClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sMediaSessionHelperClassName, sLuxxleMediaSessionHelperClassName);

        changeMethodOwner(
                sMediaSessionHelperClassName,
                "showNotification",
                sLuxxleMediaSessionHelperClassName);
        changeMethodOwner(
                sMediaSessionHelperClassName,
                "createMediaSessionObserver",
                sLuxxleMediaSessionHelperClassName);
    }
}
