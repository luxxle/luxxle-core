/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleSearchEngineAdapterClassAdapter extends LuxxleClassVisitor {
    static String sSearchEngineAdapterClassName =
            "org/chromium/chrome/browser/search_engines/settings/SearchEngineAdapter";

    static String sLuxxleSearchEngineAdapterClassName =
            "org/chromium/chrome/browser/search_engines/settings/LuxxleSearchEngineAdapter";

    static String sLuxxleBaseSearchEngineAdapterClassName =
            "org/chromium/chrome/browser/search_engines/settings/LuxxleBaseSearchEngineAdapter";

    static String sSearchEngineSettingsClassName =
            "org/chromium/chrome/browser/search_engines/settings/SearchEngineSettings";

    static String sLuxxleSearchEnginePreferenceClassName =
            "org/chromium/chrome/browser/search_engines/settings/LuxxleSearchEnginePreference";

    static String sMethodGetSearchEngineSourceType = "getSearchEngineSourceType";

    static String sMethodSortAndFilterUnnecessaryTemplateUrl =
            "sortAndFilterUnnecessaryTemplateUrl";

    public LuxxleSearchEngineAdapterClassAdapter(ClassVisitor visitor) {
        super(visitor);
        changeSuperName(sSearchEngineAdapterClassName, sLuxxleBaseSearchEngineAdapterClassName);

        changeMethodOwner(sSearchEngineAdapterClassName, sMethodGetSearchEngineSourceType,
                sLuxxleBaseSearchEngineAdapterClassName);

        changeMethodOwner(sSearchEngineAdapterClassName, sMethodSortAndFilterUnnecessaryTemplateUrl,
                sLuxxleBaseSearchEngineAdapterClassName);

        deleteField(sLuxxleSearchEngineAdapterClassName, "mProfile");
        makeProtectedField(sSearchEngineAdapterClassName, "mProfile");
    }
}
