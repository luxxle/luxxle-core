/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.bookmarks;

import android.content.ComponentName;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager;
import org.chromium.chrome.browser.ui.native_page.NativePageHost;

public class LuxxleBookmarkPage extends BookmarkPage {
    // Overridden Chromium's BookmarkPage.mManager
    private BookmarkManagerCoordinator mBookmarkManagerCoordinator;

    public LuxxleBookmarkPage(
            @NonNull SnackbarManager snackbarManager,
            @NonNull Profile profile,
            @NonNull NativePageHost host,
            @Nullable ComponentName componentName) {
        super(snackbarManager, profile, host, componentName);

        if (mBookmarkManagerCoordinator instanceof LuxxleBookmarkManagerCoordinator
                && LuxxleActivity.getChromeTabbedActivity() != null) {
            ((LuxxleBookmarkManagerCoordinator) mBookmarkManagerCoordinator)
                    .setWindow(LuxxleActivity.getChromeTabbedActivity().getWindowAndroid());
        }
    }
}
