/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleFeedSurfaceMediatorClassAdapter extends LuxxleClassVisitor {
    static String sFeedSurfaceMediatorClassName =
            "org/chromium/chrome/browser/feed/FeedSurfaceMediator";
    static String sLuxxleFeedSurfaceMediatorClassName =
            "org/chromium/chrome/browser/feed/LuxxleFeedSurfaceMediator";

    public LuxxleFeedSurfaceMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sFeedSurfaceMediatorClassName, sLuxxleFeedSurfaceMediatorClassName);

        deleteField(sLuxxleFeedSurfaceMediatorClassName, "mCoordinator");
        makeProtectedField(sFeedSurfaceMediatorClassName, "mCoordinator");

        deleteField(sLuxxleFeedSurfaceMediatorClassName, "mSnapScrollHelper");
        makeProtectedField(sFeedSurfaceMediatorClassName, "mSnapScrollHelper");
    }
}
