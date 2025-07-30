/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleSingleCategorySettingsClassAdapter extends LuxxleClassVisitor {
    static String sSingleCategorySettingsClassName =
            "org/chromium/components/browser_ui/site_settings/SingleCategorySettings";
    static String sLuxxleSingleCategorySettingsClassName =
            "org/chromium/components/browser_ui/site_settings/LuxxleSingleCategorySettings";

    public LuxxleSingleCategorySettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sSingleCategorySettingsClassName, sLuxxleSingleCategorySettingsClassName);
        changeMethodOwner(
                sSingleCategorySettingsClassName,
                "onOptionsItemSelected",
                sLuxxleSingleCategorySettingsClassName);
        changeMethodOwner(
                sSingleCategorySettingsClassName,
                "getAddExceptionDialogMessageResourceId",
                sLuxxleSingleCategorySettingsClassName);
        changeMethodOwner(
                sSingleCategorySettingsClassName,
                "resetList",
                sLuxxleSingleCategorySettingsClassName);
    }
}
