/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleNotificationBuilderClassAdapter extends LuxxleClassVisitor {
    static String sStandardNotificationBuilderClassName =
            "org/chromium/chrome/browser/notifications/StandardNotificationBuilder";
    static String sLuxxleNotificationBuilderClassName =
            "org/chromium/chrome/browser/notifications/LuxxleNotificationBuilder";

    public LuxxleNotificationBuilderClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sStandardNotificationBuilderClassName, sLuxxleNotificationBuilderClassName);
    }
}
