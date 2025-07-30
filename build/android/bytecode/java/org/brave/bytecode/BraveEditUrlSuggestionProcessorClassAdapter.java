/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleEditUrlSuggestionProcessorClassAdapter extends LuxxleClassVisitor {
    static String sEditUrlSuggestionProcessorName =
            "org/chromium/chrome/browser/omnibox/suggestions/editurl/EditUrlSuggestionProcessor";
    static String sLuxxleEditUrlSuggestionProcessorName =
            "org/chromium/chrome/browser/omnibox/suggestions/editurl/LuxxleEditUrlSuggestionProcessor";

    public LuxxleEditUrlSuggestionProcessorClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(sEditUrlSuggestionProcessorName, sLuxxleEditUrlSuggestionProcessorName);

        addMethodAnnotation(
                sLuxxleEditUrlSuggestionProcessorName,
                "maybeUpdateSuggestionForCopyLink",
                "Ljava/lang/Override;");
    }
}
