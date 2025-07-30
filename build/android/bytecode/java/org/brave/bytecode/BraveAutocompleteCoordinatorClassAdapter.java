/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleAutocompleteCoordinatorClassAdapter extends LuxxleClassVisitor {
    static String sAutocompleteCoordinator =
            "org/chromium/chrome/browser/omnibox/suggestions/AutocompleteCoordinator";

    static String sLuxxleAutocompleteCoordinator =
            "org/chromium/chrome/browser/omnibox/suggestions/LuxxleAutocompleteCoordinator";

    public LuxxleAutocompleteCoordinatorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sAutocompleteCoordinator, sLuxxleAutocompleteCoordinator);
        changeMethodOwner(
                sAutocompleteCoordinator, "createViewProvider", sLuxxleAutocompleteCoordinator);
    }
}
