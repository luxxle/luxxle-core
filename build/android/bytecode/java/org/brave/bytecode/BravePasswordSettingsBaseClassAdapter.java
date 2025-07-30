/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxlePasswordSettingsBaseClassAdapter extends LuxxleClassVisitor {
    static String sPasswordSettingsClassName =
            "org/chromium/chrome/browser/password_manager/settings/PasswordSettings";
    static String sLuxxlePasswordSettingsBaseClassName =
            "org/chromium/chrome/browser/password_manager/settings/LuxxlePasswordSettingsBase";

    public LuxxlePasswordSettingsBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sPasswordSettingsClassName, sLuxxlePasswordSettingsBaseClassName);

        changeMethodOwner(
                sPasswordSettingsClassName,
                "createCheckPasswords",
                sLuxxlePasswordSettingsBaseClassName);
        deleteMethod(sPasswordSettingsClassName, "createCheckPasswords");
    }
}
