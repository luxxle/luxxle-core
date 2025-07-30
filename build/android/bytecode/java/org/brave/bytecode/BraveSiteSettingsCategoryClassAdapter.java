/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleSiteSettingsCategoryClassAdapter extends LuxxleClassVisitor {
    static String sSiteSettingsCategoryClassName =
            "org/chromium/components/browser_ui/site_settings/SiteSettingsCategory";
    static String sLuxxleSiteSettingsCategoryClassName =
            "org/chromium/components/browser_ui/site_settings/LuxxleSiteSettingsCategory";

    public LuxxleSiteSettingsCategoryClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sSiteSettingsCategoryClassName, "contentSettingsType",
                sLuxxleSiteSettingsCategoryClassName);
        changeMethodOwner(sSiteSettingsCategoryClassName, "preferenceKey",
                sLuxxleSiteSettingsCategoryClassName);
    }
}
