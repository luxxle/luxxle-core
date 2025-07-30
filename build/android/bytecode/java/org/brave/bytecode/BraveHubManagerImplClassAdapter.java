/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleHubManagerImplClassAdapter extends LuxxleClassVisitor {
    static String sHubManagerImplClassName = "org/chromium/chrome/browser/hub/HubManagerImpl";
    static String sLuxxleHubManagerImplClassName =
            "org/chromium/chrome/browser/hub/LuxxleHubManagerImpl";

    public LuxxleHubManagerImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sHubManagerImplClassName, sLuxxleHubManagerImplClassName);
    }
}
