/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleVariationsSeedFetcherClassAdapter extends LuxxleClassVisitor {
    static String sVariationsSeedFetcherClassName =
            "org/chromium/components/variations/firstrun/VariationsSeedFetcher";
    static String sLuxxleVariationsSeedFetcherClassName =
            "org/chromium/components/variations/firstrun/LuxxleVariationsSeedFetcher";

    public LuxxleVariationsSeedFetcherClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sVariationsSeedFetcherClassName, "get", sLuxxleVariationsSeedFetcherClassName);

        deleteField(sLuxxleVariationsSeedFetcherClassName, "sLock");
        makeProtectedField(sVariationsSeedFetcherClassName, "sLock");
        deleteField(sLuxxleVariationsSeedFetcherClassName, "DEFAULT_VARIATIONS_SERVER_URL");
        makeProtectedField(sVariationsSeedFetcherClassName, "DEFAULT_VARIATIONS_SERVER_URL");
        deleteField(sLuxxleVariationsSeedFetcherClassName, "DEFAULT_FAST_VARIATIONS_SERVER_URL");
        makeProtectedField(sVariationsSeedFetcherClassName, "DEFAULT_FAST_VARIATIONS_SERVER_URL");
    }
}
