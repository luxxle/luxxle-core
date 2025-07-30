/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.luxxle.bytecode;

import org.objectweb.asm.ClassVisitor;

public class LuxxleDownloadMessageUiControllerImplClassAdapter extends LuxxleClassVisitor {
    static String sDownloadMessageUiControllerImpl =
            "org/chromium/chrome/browser/download/DownloadMessageUiControllerImpl";

    static String sLuxxleDownloadMessageUiControllerImpl =
            "org/chromium/chrome/browser/download/LuxxleDownloadMessageUiControllerImpl";

    public LuxxleDownloadMessageUiControllerImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeSuperName(sDownloadMessageUiControllerImpl, sLuxxleDownloadMessageUiControllerImpl);
        changeMethodOwner(sDownloadMessageUiControllerImpl, "isVisibleToUser",
                sLuxxleDownloadMessageUiControllerImpl);
    }
}
