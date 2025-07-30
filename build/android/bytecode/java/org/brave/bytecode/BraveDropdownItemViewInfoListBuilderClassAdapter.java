/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleDropdownItemViewInfoListBuilderClassAdapter extends LuxxleClassVisitor {
    static String sDropdownItemViewInfoListBuilder =
            "org/chromium/chrome/browser/omnibox/suggestions/DropdownItemViewInfoListBuilder";

    static String sLuxxleDropdownItemViewInfoListBuilder =
            "org/chromium/chrome/browser/omnibox/suggestions/LuxxleDropdownItemViewInfoListBuilder";

    public LuxxleDropdownItemViewInfoListBuilderClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sDropdownItemViewInfoListBuilder, sLuxxleDropdownItemViewInfoListBuilder);
    }
}
