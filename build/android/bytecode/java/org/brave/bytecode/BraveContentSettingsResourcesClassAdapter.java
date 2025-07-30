/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleContentSettingsResourcesClassAdapter extends LuxxleClassVisitor {
    static String sContentSettingsResourcesClassName =
            "org/chromium/components/browser_ui/site_settings/ContentSettingsResources";
    static String sLuxxleContentSettingsResourcesClassName =
            "org/chromium/components/browser_ui/site_settings/LuxxleContentSettingsResources";
    static String sContentSettingsResourcesResourceItemClassName =
            "org/chromium/components/browser_ui/site_settings/ContentSettingsResources$ResourceItem";
    static String sLuxxleContentSettingsResourcesResourceItemClassName =
            "org/chromium/components/browser_ui/site_settings/LuxxleContentSettingsResources$ResourceItem";

    public LuxxleContentSettingsResourcesClassAdapter(ClassVisitor visitor) {
        super(visitor);

        makePublicMethod(sContentSettingsResourcesClassName, "getResourceItem");
        changeMethodOwner(sContentSettingsResourcesClassName, "getResourceItem",
                sLuxxleContentSettingsResourcesClassName);
        makePublicInnerClass(sContentSettingsResourcesClassName, "ResourceItem");
        redirectConstructor(sLuxxleContentSettingsResourcesResourceItemClassName,
                sContentSettingsResourcesResourceItemClassName);
        redirectTypeInMethod(sLuxxleContentSettingsResourcesClassName, "getResourceItem",
                sLuxxleContentSettingsResourcesResourceItemClassName,
                sContentSettingsResourcesResourceItemClassName);
    }
}
