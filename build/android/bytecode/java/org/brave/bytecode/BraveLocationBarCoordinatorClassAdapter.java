/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleLocationBarCoordinatorClassAdapter extends LuxxleClassVisitor {
    static String sLocationBarCoordinator =
            "org/chromium/chrome/browser/omnibox/LocationBarCoordinator";
    static String sLuxxleLocationBarCoordinator =
            "org/chromium/chrome/browser/omnibox/LuxxleLocationBarCoordinator";

    public LuxxleLocationBarCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sLocationBarCoordinator, sLuxxleLocationBarCoordinator);

        deleteField(sLuxxleLocationBarCoordinator, "mLocationBarMediator");
        makeProtectedField(sLocationBarCoordinator, "mLocationBarMediator");

        deleteField(sLuxxleLocationBarCoordinator, "mUrlBar");
        makeProtectedField(sLocationBarCoordinator, "mUrlBar");
    }
}
