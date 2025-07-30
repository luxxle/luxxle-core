/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleAutocompleteMediatorBaseClassAdapter extends LuxxleClassVisitor {
    static String sAutocompleteMediator =
            "org/chromium/chrome/browser/omnibox/suggestions/AutocompleteMediator";
    static String sLuxxleAutocompleteMediatorBase =
            "org/chromium/chrome/browser/omnibox/suggestions/LuxxleAutocompleteMediatorBase";

    public LuxxleAutocompleteMediatorBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sAutocompleteMediator, sLuxxleAutocompleteMediatorBase);

        makeProtectedField(sAutocompleteMediator, "mContext");
        makeProtectedField(sAutocompleteMediator, "mDataProvider");
        changeMethodOwner(
                sAutocompleteMediator, "loadUrlForOmniboxMatch", sLuxxleAutocompleteMediatorBase);
    }
}
