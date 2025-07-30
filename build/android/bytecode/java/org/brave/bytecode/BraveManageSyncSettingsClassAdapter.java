/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

/**
 * Adapter, used to make some fields from upstream's ManageSyncSettings be public
 */
public class LuxxleManageSyncSettingsClassAdapter extends LuxxleClassVisitor {
    static String sManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/ManageSyncSettings";
    static String sLuxxleManageSyncSettingsClassName =
            "org/chromium/chrome/browser/sync/settings/LuxxleManageSyncSettings";

    LuxxleManageSyncSettingsClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sLuxxleManageSyncSettingsClassName, "mGoogleActivityControls");
        makeProtectedField(sManageSyncSettingsClassName, "mGoogleActivityControls");

        deleteField(sLuxxleManageSyncSettingsClassName, "mSyncEncryption");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncEncryption");

        deleteField(sLuxxleManageSyncSettingsClassName, "mSyncEverything");
        makeProtectedField(sManageSyncSettingsClassName, "mSyncEverything");
    }
}
