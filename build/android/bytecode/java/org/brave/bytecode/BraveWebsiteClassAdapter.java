/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleWebsiteClassAdapter extends LuxxleClassVisitor {
    static String sWebsiteClassName = "org/chromium/components/browser_ui/site_settings/Website";
    static String sLuxxleWebsiteClassName =
            "org/chromium/components/browser_ui/site_settings/LuxxleWebsite";

    public LuxxleWebsiteClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sWebsiteClassName, sLuxxleWebsiteClassName);

        makePrivateMethod(sWebsiteClassName, "setContentSetting");

        makePublicMethod(sLuxxleWebsiteClassName, "setContentSetting");
        changeMethodOwner(sWebsiteClassName, "setContentSetting", sLuxxleWebsiteClassName);
    }
}
