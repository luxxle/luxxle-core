/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleMostVisitedTilesMediatorClassAdapter extends LuxxleClassVisitor {
    static String sMostVisitedTilesMediatorClassName =
            "org/chromium/chrome/browser/suggestions/tile/MostVisitedTilesMediator";
    static String sLuxxleMostVisitedTilesMediatorClassName =
            "org/chromium/chrome/browser/suggestions/tile/LuxxleMostVisitedTilesMediator";

    public LuxxleMostVisitedTilesMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sMostVisitedTilesMediatorClassName, sLuxxleMostVisitedTilesMediatorClassName);

        makePublicMethod(sMostVisitedTilesMediatorClassName, "updateTilePlaceholderVisibility");
        addMethodAnnotation(sLuxxleMostVisitedTilesMediatorClassName,
                "updateTilePlaceholderVisibility", "Ljava/lang/Override;");

        deleteField(sLuxxleMostVisitedTilesMediatorClassName, "mTileGroup");
        makeProtectedField(sMostVisitedTilesMediatorClassName, "mTileGroup");
    }
}
