/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleRadioButtonGroupHomepagePreferenceClassAdapter extends LuxxleClassVisitor {
    static String sRadioButtonGroupHomepagePreferenceClassName =
            "org/chromium/chrome/browser/homepage/settings/RadioButtonGroupHomepagePreference";
    static String sLuxxleRadioButtonGroupHomepagePreferenceClassName =
            "org/chromium/chrome/browser/homepage/settings/LuxxleRadioButtonGroupHomepagePreference";

    public LuxxleRadioButtonGroupHomepagePreferenceClassAdapter(ClassVisitor visitor) {
        super(visitor);

        makeNonFinalClass(sRadioButtonGroupHomepagePreferenceClassName);

        changeSuperName(
                sLuxxleRadioButtonGroupHomepagePreferenceClassName,
                sRadioButtonGroupHomepagePreferenceClassName);
    }
}
