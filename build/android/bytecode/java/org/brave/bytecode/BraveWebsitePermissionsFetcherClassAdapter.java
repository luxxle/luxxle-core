/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleWebsitePermissionsFetcherClassAdapter extends LuxxleClassVisitor {
    static String sWebsitePermissionsFetcherClassName =
            "org/chromium/components/browser_ui/site_settings/WebsitePermissionsFetcher";
    static String sLuxxleWebsitePermissionsFetcherClassName =
            "org/chromium/components/browser_ui/site_settings/LuxxleWebsitePermissionsFetcher";

    public LuxxleWebsitePermissionsFetcherClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(sWebsitePermissionsFetcherClassName, "getPermissionsType",
                sLuxxleWebsitePermissionsFetcherClassName);
    }
}
