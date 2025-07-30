/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleSearchEnginePreferenceClassAdapter extends LuxxleClassVisitor {
    static String sSearchEngineSettingsClassName =
            "org/chromium/chrome/browser/search_engines/settings/SearchEngineSettings";

    static String sLuxxleSearchEnginePreferenceClassName =
            "org/chromium/chrome/browser/search_engines/settings/LuxxleSearchEnginePreference";

    public LuxxleSearchEnginePreferenceClassAdapter(ClassVisitor visitor) {
        super(visitor);

        deleteField(sLuxxleSearchEnginePreferenceClassName, "mSearchEngineAdapter");
        makeProtectedField(sSearchEngineSettingsClassName, "mSearchEngineAdapter");

        deleteField(sLuxxleSearchEnginePreferenceClassName, "mProfile");
        makeProtectedField(sSearchEngineSettingsClassName, "mProfile");

        makePublicMethod(sSearchEngineSettingsClassName, "createAdapterIfNecessary");
        addMethodAnnotation(sLuxxleSearchEnginePreferenceClassName, "createAdapterIfNecessary",
                "Ljava/lang/Override;");
    }
}
