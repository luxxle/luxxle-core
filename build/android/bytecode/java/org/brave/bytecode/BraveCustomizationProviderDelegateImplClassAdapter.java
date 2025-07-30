/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleCustomizationProviderDelegateImplClassAdapter extends LuxxleClassVisitor {
    static String sCustomizationProviderDelegateImplClassName =
            "org/chromium/chrome/browser/partnercustomizations/CustomizationProviderDelegateUpstreamImpl";
    static String sLuxxleCustomizationProviderDelegateImplClassName =
            "org/chromium/chrome/browser/partnercustomizations/LuxxleCustomizationProviderDelegateImpl";

    public LuxxleCustomizationProviderDelegateImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sCustomizationProviderDelegateImplClassName,
                sLuxxleCustomizationProviderDelegateImplClassName);
    }
}
