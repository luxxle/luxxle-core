/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleQuickActionSearchWidgetProviderClassAdapter extends LuxxleClassVisitor {
    static String sQuickActionSearchWidgetProviderClassName =
            "org/chromium/chrome/browser/quickactionsearchwidget/QuickActionSearchWidgetProvider";
    static String sLuxxleQuickActionSearchWidgetProviderClassName =
            "org/chromium/chrome/browser/quickactionsearchwidget/LuxxleQuickActionSearchWidgetProvider"; // presubmit: ignore-long-line

    public LuxxleQuickActionSearchWidgetProviderClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(
                sQuickActionSearchWidgetProviderClassName,
                "setWidgetEnabled",
                sLuxxleQuickActionSearchWidgetProviderClassName);
    }
}
