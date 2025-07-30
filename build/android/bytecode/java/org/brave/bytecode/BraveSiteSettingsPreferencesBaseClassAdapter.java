/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleSiteSettingsPreferencesBaseClassAdapter extends LuxxleClassVisitor {
    static String sSiteSettingsClassName =
            "org/chromium/components/browser_ui/site_settings/SiteSettings";
    static String sLuxxleSiteSettingsPreferencesBaseClassName =
            "org/chromium/components/browser_ui/site_settings/LuxxleSiteSettingsPreferencesBase";

    public LuxxleSiteSettingsPreferencesBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sSiteSettingsClassName, sLuxxleSiteSettingsPreferencesBaseClassName);
    }
}
