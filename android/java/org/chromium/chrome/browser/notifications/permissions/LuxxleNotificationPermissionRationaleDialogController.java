/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.notifications.permissions;

import android.content.Context;

import org.chromium.base.Callback;
import org.chromium.ui.modaldialog.ModalDialogManager;

/**
 *
 * This is override chromium android 13 default dialog
 * */
public class LuxxleNotificationPermissionRationaleDialogController
        extends NotificationPermissionRationaleDialogController {
    public LuxxleNotificationPermissionRationaleDialogController(
            Context context, ModalDialogManager modalDialogManager) {
        super(context, modalDialogManager);
    }

    /** This is empty override method, to block default android 13 chromium dialog */
    @Override
    public void showRationaleUi(Callback<Integer> rationaleCallback) {}
}
