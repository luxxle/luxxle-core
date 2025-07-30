/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleClearBrowsingDataFragmentAdapter extends LuxxleClassVisitor {
    static String sFragment = "org/chromium/chrome/browser/browsing_data/ClearBrowsingDataFragment";
    static String sLuxxleFragment =
            "org/chromium/chrome/browser/browsing_data/LuxxleClearBrowsingDataFragment";

    public LuxxleClearBrowsingDataFragmentAdapter(ClassVisitor visitor) {
        super(visitor);
        redirectConstructor(sFragment, sLuxxleFragment);
    }
}
