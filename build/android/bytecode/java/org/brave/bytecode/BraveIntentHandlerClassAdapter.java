/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleIntentHandlerClassAdapter extends LuxxleClassVisitor {
    static String sIntentHandlerClassName = "org/chromium/chrome/browser/IntentHandler";
    static String sLuxxleIntentHandlerClassName = "org/chromium/chrome/browser/LuxxleIntentHandler";

    public LuxxleIntentHandlerClassAdapter(ClassVisitor visitor) {
        super(visitor);

        makePublicMethod(sIntentHandlerClassName, "getUrlForCustomTab");
        changeMethodOwner(
                sLuxxleIntentHandlerClassName, "getUrlForCustomTab", sIntentHandlerClassName);

        makePublicMethod(sIntentHandlerClassName, "getUrlForWebapp");
        changeMethodOwner(sLuxxleIntentHandlerClassName, "getUrlForWebapp", sIntentHandlerClassName);

        makePublicMethod(sIntentHandlerClassName, "isJavascriptSchemeOrInvalidUrl");
        changeMethodOwner(sLuxxleIntentHandlerClassName, "isJavascriptSchemeOrInvalidUrl",
                sIntentHandlerClassName);

        changeMethodOwner(
                sIntentHandlerClassName, "extractUrlFromIntent", sLuxxleIntentHandlerClassName);
    }
}
