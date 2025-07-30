/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleFullscreenHtmlApiHandlerCompatClassAdapter extends LuxxleClassVisitor {
    static String sFullscreenHtmlApiHandlerCompat =
            "org/chromium/chrome/browser/fullscreen/FullscreenHtmlApiHandlerCompat";
    static String sLuxxleFullscreenHtmlApiHandlerCompat =
            "org/chromium/chrome/browser/fullscreen/LuxxleFullscreenHtmlApiHandlerCompat";

    public LuxxleFullscreenHtmlApiHandlerCompatClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sFullscreenHtmlApiHandlerCompat, sLuxxleFullscreenHtmlApiHandlerCompat);
    }
}
