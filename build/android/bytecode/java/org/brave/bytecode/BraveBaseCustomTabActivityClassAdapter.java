/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleBaseCustomTabActivityClassAdapter extends LuxxleClassVisitor {
    static String sBaseCustomTabActivityClassName =
            "org/chromium/chrome/browser/customtabs/BaseCustomTabActivity";
    static String sFullScreenCustomTabActivityClassName =
            "org/chromium/chrome/browser/customtabs/FullScreenCustomTabActivity";

    public LuxxleBaseCustomTabActivityClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sFullScreenCustomTabActivityClassName, "mBaseCustomTabRootUiCoordinator");
        makeProtectedField(sBaseCustomTabActivityClassName, "mBaseCustomTabRootUiCoordinator");

        deleteField(sFullScreenCustomTabActivityClassName, "mIntentDataProvider");
        makeProtectedField(sBaseCustomTabActivityClassName, "mIntentDataProvider");

        deleteField(sFullScreenCustomTabActivityClassName, "mToolbarCoordinator");
        makeProtectedField(sBaseCustomTabActivityClassName, "mToolbarCoordinator");

        deleteField(sFullScreenCustomTabActivityClassName, "mTabController");
        makeProtectedField(sBaseCustomTabActivityClassName, "mTabController");

        deleteField(sFullScreenCustomTabActivityClassName, "mMinimizationManagerHolder");
        makeProtectedField(sBaseCustomTabActivityClassName, "mMinimizationManagerHolder");

        deleteField(sFullScreenCustomTabActivityClassName, "mCustomTabFeatureOverridesManager");
        makeProtectedField(sBaseCustomTabActivityClassName, "mCustomTabFeatureOverridesManager");

        deleteMethod(sFullScreenCustomTabActivityClassName, "getAppHeaderCoordinator");
        makePublicMethod(sBaseCustomTabActivityClassName, "getAppHeaderCoordinator");
    }
}
