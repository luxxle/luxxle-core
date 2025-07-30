/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleFullscreenVideoPictureInPictureControllerClassAdapter extends LuxxleClassVisitor {
    static String sFullscreenVideoPictureInPictureController =
            "org/chromium/chrome/browser/media/FullscreenVideoPictureInPictureController";
    static String sLuxxleFullscreenVideoPictureInPictureController =
            "org/chromium/chrome/browser/media/LuxxleFullscreenVideoPictureInPictureController";

    public LuxxleFullscreenVideoPictureInPictureControllerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(
                sFullscreenVideoPictureInPictureController,
                sLuxxleFullscreenVideoPictureInPictureController);
        deleteField(sFullscreenVideoPictureInPictureController, "mDismissPending");
        changeMethodOwner(
                sFullscreenVideoPictureInPictureController,
                "dismissActivityIfNeeded",
                sLuxxleFullscreenVideoPictureInPictureController);
    }
}
