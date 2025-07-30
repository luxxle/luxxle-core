/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleUndoBarControllerBaseClassAdapter extends LuxxleClassVisitor {
    static String sUndoBarController =
            "org/chromium/chrome/browser/undo_tab_close_snackbar/UndoBarController";
    static String sLuxxleUndoBarControllerBase =
            "org/chromium/chrome/browser/undo_tab_close_snackbar/LuxxleUndoBarControllerBase";

    public LuxxleUndoBarControllerBaseClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sUndoBarController, sLuxxleUndoBarControllerBase);

        changeMethodOwner(sUndoBarController, "showUndoBar", sLuxxleUndoBarControllerBase);
    }
}
