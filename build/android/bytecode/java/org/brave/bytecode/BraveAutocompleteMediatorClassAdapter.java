/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleAutocompleteMediatorClassAdapter extends LuxxleClassVisitor {
    static String sAutocompleteMediator =
            "org/chromium/chrome/browser/omnibox/suggestions/AutocompleteMediator";
    static String sLuxxleAutocompleteMediator =
            "org/chromium/chrome/browser/omnibox/suggestions/LuxxleAutocompleteMediator";

    public LuxxleAutocompleteMediatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sAutocompleteMediator, sLuxxleAutocompleteMediator);

        deleteField(sLuxxleAutocompleteMediator, "mDropdownViewInfoListManager");
        makeProtectedField(sAutocompleteMediator, "mDropdownViewInfoListManager");

        deleteField(sLuxxleAutocompleteMediator, "mDropdownViewInfoListBuilder");
        makeProtectedField(sAutocompleteMediator, "mDropdownViewInfoListBuilder");
    }
}
