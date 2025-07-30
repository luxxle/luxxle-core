/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleHelpAndFeedbackLauncherImplClassAdapter extends LuxxleClassVisitor {
    static String sHelpAndFeedbackLauncherImplClassName =
            "org/chromium/chrome/browser/feedback/HelpAndFeedbackLauncherImpl";

    static String sLuxxleHelpAndFeedbackLauncherImplClassName =
            "org/chromium/chrome/browser/feedback/LuxxleHelpAndFeedbackLauncherImpl";

    public LuxxleHelpAndFeedbackLauncherImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sHelpAndFeedbackLauncherImplClassName, sLuxxleHelpAndFeedbackLauncherImplClassName);
    }
}
