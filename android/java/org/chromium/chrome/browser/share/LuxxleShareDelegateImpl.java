/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.share;

import android.content.Context;

import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.base.supplier.Supplier;
import org.chromium.build.annotations.NullMarked;
import org.chromium.chrome.browser.data_sharing.DataSharingTabManager;
import org.chromium.chrome.browser.lifecycle.ActivityLifecycleDispatcher;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tabmodel.TabModelSelector;
import org.chromium.components.browser_ui.bottomsheet.BottomSheetController;

@NullMarked
public class LuxxleShareDelegateImpl extends ShareDelegateImpl {
    public LuxxleShareDelegateImpl(
            Context context,
            BottomSheetController controller,
            ActivityLifecycleDispatcher lifecycleDispatcher,
            Supplier<Tab> tabProvider,
            Supplier<TabModelSelector> tabModelSelectorProvider,
            Supplier<Profile> profileSupplier,
            ShareSheetDelegate delegate,
            boolean isCustomTab,
            DataSharingTabManager dataSharingTabManager) {
        super(
                context,
                controller,
                lifecycleDispatcher,
                tabProvider,
                tabModelSelectorProvider,
                profileSupplier,
                delegate,
                isCustomTab,
                dataSharingTabManager);
    }

    @Override
    public boolean isSharingHubEnabled() {
        return super.isSharingHubEnabled()
                && !ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePreferenceKeys.LUXXLE_DISABLE_SHARING_HUB, false);
    }
}
