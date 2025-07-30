/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleFullscreenHtmlApiHandlerLegacyClassAdapter extends LuxxleClassVisitor {
    static String sFullscreenHtmlApiHandlerLegacy =
            "org/chromium/chrome/browser/fullscreen/FullscreenHtmlApiHandlerLegacy";
    static String sLuxxleFullscreenHtmlApiHandlerLegacy =
            "org/chromium/chrome/browser/fullscreen/LuxxleFullscreenHtmlApiHandlerLegacy";

    public LuxxleFullscreenHtmlApiHandlerLegacyClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sFullscreenHtmlApiHandlerLegacy, sLuxxleFullscreenHtmlApiHandlerLegacy);
    }
}
