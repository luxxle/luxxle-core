/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleIncognitoTabSwitcherPaneClassAdapter extends LuxxleClassVisitor {
    static String sIncognitoTabSwitcherPaneClassName =
            "org/chromium/chrome/browser/tasks/tab_management/IncognitoTabSwitcherPane";

    static String sLuxxleIncognitoTabSwitcherPaneClassName =
            "org/chromium/chrome/browser/tasks/tab_management/LuxxleIncognitoTabSwitcherPane";

    public LuxxleIncognitoTabSwitcherPaneClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sIncognitoTabSwitcherPaneClassName, sLuxxleIncognitoTabSwitcherPaneClassName);
    }
}
