/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleSettingsIntentUtilClassAdapter extends LuxxleClassVisitor {
    static String sSettingsIntentUtilClassName =
            "org/chromium/chrome/browser/settings/SettingsIntentUtil";

    static String sLuxxleSettingsIntentUtilClassName =
            "org/chromium/chrome/browser/settings/LuxxleSettingsIntentUtil";

    public LuxxleSettingsIntentUtilClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeMethodOwner(
                sSettingsIntentUtilClassName, "createIntent", sLuxxleSettingsIntentUtilClassName);
    }
}
