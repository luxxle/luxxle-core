/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

/**
 * Adapter, used to make some fields from upstream's PasswordAccessReauthenticationHelper be public
 */
public class LuxxlePasswordAccessReauthenticationHelperClassAdapter extends LuxxleClassVisitor {
    static String sHelperClassName =
            "org/chromium/chrome/browser/password_manager/settings/PasswordAccessReauthenticationHelper"; // presubmit: ignore-long-line
    static String sLuxxleHelperClassName =
            "org/chromium/chrome/browser/sync/settings/LuxxlePasswordAccessReauthenticationHelper";

    LuxxlePasswordAccessReauthenticationHelperClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sLuxxleHelperClassName, "mCallback");
        makeProtectedField(sHelperClassName, "mCallback");

        deleteField(sLuxxleHelperClassName, "mFragmentManager");
        makeProtectedField(sHelperClassName, "mFragmentManager");
    }
}
