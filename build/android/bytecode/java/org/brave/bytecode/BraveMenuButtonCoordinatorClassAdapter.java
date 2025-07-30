/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleMenuButtonCoordinatorClassAdapter extends LuxxleClassVisitor {
    static String sMenuButtonCoordinator =
            "org/chromium/chrome/browser/toolbar/menu_button/MenuButtonCoordinator";
    static String sLuxxleMenuButtonCoordinator =
            "org/chromium/chrome/browser/toolbar/menu_button/LuxxleMenuButtonCoordinator";

    public LuxxleMenuButtonCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sMenuButtonCoordinator, sLuxxleMenuButtonCoordinator);
    }
}
