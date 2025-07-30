/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleMostVisitedTilesLayoutBaseClassAdapter extends LuxxleClassVisitor {
    static String sMostVisitedTilesLayoutClassName =
            "org/chromium/chrome/browser/suggestions/tile/MostVisitedTilesLayout";
    static String sLuxxleMostVisitedTilesLayoutBaseClassName =
            "org/chromium/chrome/browser/suggestions/tile/LuxxleMostVisitedTilesLayoutBase";

    public LuxxleMostVisitedTilesLayoutBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(
                sMostVisitedTilesLayoutClassName, sLuxxleMostVisitedTilesLayoutBaseClassName);

        changeSuperName(
                "org/chromium/chrome/browser/suggestions/tile/TilesLinearLayout",
                "android/widget/GridLayout");

        deleteMethod(sLuxxleMostVisitedTilesLayoutBaseClassName, "getColumnCount");
        deleteMethod(sLuxxleMostVisitedTilesLayoutBaseClassName, "setColumnCount");
        deleteMethod(sLuxxleMostVisitedTilesLayoutBaseClassName, "getRowCount");
        deleteMethod(sLuxxleMostVisitedTilesLayoutBaseClassName, "setRowCount");
    }
}
