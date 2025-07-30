/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleSingleWebsiteSettingsClassAdapter extends LuxxleClassVisitor {
    static String sSingleWebsiteSettingsClassName =
            "org/chromium/components/browser_ui/site_settings/SingleWebsiteSettings";
    static String sLuxxleSingleWebsiteSettingsClassName =
            "org/chromium/components/browser_ui/site_settings/LuxxleSingleWebsiteSettings";

    public LuxxleSingleWebsiteSettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sSingleWebsiteSettingsClassName, sLuxxleSingleWebsiteSettingsClassName);

        changeMethodOwner(sSingleWebsiteSettingsClassName, "getPreferenceKey",
                sLuxxleSingleWebsiteSettingsClassName);
        changeMethodOwner(sSingleWebsiteSettingsClassName, "setupContentSettingsPreferences",
                sLuxxleSingleWebsiteSettingsClassName);

        makePublicMethod(sSingleWebsiteSettingsClassName, "setupContentSettingsPreference");
        changeMethodOwner(sLuxxleSingleWebsiteSettingsClassName, "setupContentSettingsPreference",
                sSingleWebsiteSettingsClassName);
    }
}
