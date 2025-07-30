/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxlePreferenceFragmentClassAdapter extends LuxxleClassVisitor {
    static String sDeveloperSettingsClassName =
            "org/chromium/chrome/browser/tracing/settings/DeveloperSettings";

    static String sLuxxlePreferenceFragmentClassName =
            "org/chromium/chrome/browser/settings/LuxxlePreferenceFragment";

    public LuxxlePreferenceFragmentClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sDeveloperSettingsClassName, sLuxxlePreferenceFragmentClassName);
    }
}
