/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleEditUrlSuggestionProcessorBaseClassAdapter extends LuxxleClassVisitor {
    static String sEditUrlSuggestionProcessor =
            "org/chromium/chrome/browser/omnibox/suggestions/editurl/EditUrlSuggestionProcessor";
    static String sLuxxleEditUrlSuggestionProcessorBase =
            "org/chromium/chrome/browser/omnibox/suggestions/editurl/LuxxleEditUrlSuggestionProcessorBase";

    public LuxxleEditUrlSuggestionProcessorBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sEditUrlSuggestionProcessor, sLuxxleEditUrlSuggestionProcessorBase);

        changeMethodOwner(
                sEditUrlSuggestionProcessor, "onCopyLink", sLuxxleEditUrlSuggestionProcessorBase);
    }
}
