/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleNotificationPermissionRationaleDialogControllerClassAdapter
        extends LuxxleClassVisitor {
    static String sContoller =
            "org/chromium/chrome/browser/notifications/permissions/NotificationPermissionRationaleDialogController";
    static String sLuxxleContoller =
            "org/chromium/chrome/browser/notifications/permissions/LuxxleNotificationPermissionRationaleDialogController";

    public LuxxleNotificationPermissionRationaleDialogControllerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sContoller, sLuxxleContoller);
    }
}
