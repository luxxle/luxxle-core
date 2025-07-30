/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleNotificationPlatformBridgeClassAdapter extends LuxxleClassVisitor {
    static String sNotificationPlatformBridgeClassName =
            "org/chromium/chrome/browser/notifications/NotificationPlatformBridge";
    static String sLuxxleNotificationPlatformBridgeClassName =
            "org/chromium/chrome/browser/notifications/LuxxleNotificationPlatformBridge";

    public LuxxleNotificationPlatformBridgeClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sNotificationPlatformBridgeClassName,
                "dispatchNotificationEvent",
                sLuxxleNotificationPlatformBridgeClassName);

        changeMethodOwner(
                sNotificationPlatformBridgeClassName,
                "prepareNotificationBuilder",
                sLuxxleNotificationPlatformBridgeClassName);
    }
}
