/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleFullscreenHtmlApiHandlerBaseClassAdapter extends LuxxleClassVisitor {
    static String sFullscreenHtmlApiHandlerBase =
            "org/chromium/chrome/browser/fullscreen/FullscreenHtmlApiHandlerBase";
    static String sLuxxleFullscreenHtmlApiHandlerBase =
            "org/chromium/chrome/browser/fullscreen/LuxxleFullscreenHtmlApiHandlerBase";

    public LuxxleFullscreenHtmlApiHandlerBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sFullscreenHtmlApiHandlerBase, sLuxxleFullscreenHtmlApiHandlerBase);
    }
}
