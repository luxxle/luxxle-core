/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.informers;

import org.jni_zero.CalledByNative;

import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleSyncWorker;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.infobar.LuxxleInfoBarIdentifier;
import org.chromium.chrome.browser.settings.LuxxleSyncScreensPreference;
import org.chromium.chrome.browser.settings.SettingsNavigationFactory;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.ui.messages.infobar.LuxxleSimpleConfirmInfoBarBuilder;
import org.chromium.chrome.browser.ui.messages.infobar.SimpleConfirmInfoBarBuilder;
import org.chromium.components.browser_ui.settings.SettingsNavigation;

public class LuxxleSyncAccountDeletedInformer {
    private static final String TAG = "SyncAccountDeleted";

    @CalledByNative
    public static void show() {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();

            Tab tab = activity.getActivityTabProvider().get();
            if (tab == null) return;

            if (!LuxxleSyncWorker.get().isAccountDeletedNoticePending()) {
                return;
            }

            LuxxleSimpleConfirmInfoBarBuilder.createInfobarWithDrawable(
                    tab.getWebContents(),
                    new SimpleConfirmInfoBarBuilder.Listener() {
                        @Override
                        public void onInfoBarDismissed() {
                            // Pressing cross
                            // In any way don't show the informer again
                            disableInformer();
                        }

                        @Override
                        public boolean onInfoBarButtonClicked(boolean isPrimary) {
                            assert isPrimary : "We don't have secondary button";
                            // Pressing `OK`
                            // Don't show the informer again
                            disableInformer();
                            return false;
                        }

                        @Override
                        public boolean onInfoBarLinkClicked() {
                            // Pressing link `re-create the account`
                            // Don't show the informer again
                            disableInformer();
                            SettingsNavigation settingsLauncher =
                                    SettingsNavigationFactory.createSettingsNavigation();
                            settingsLauncher.startSettings(
                                    ContextUtils.getApplicationContext(),
                                    LuxxleSyncScreensPreference.class);
                            return false;
                        }
                    },
                    LuxxleInfoBarIdentifier.LUXXLE_SYNC_ACCOUNT_DELETED_INFOBAR,
                    activity,
                    R.drawable.ic_warning_circle,
                    // See comment at |LuxxleSyncAccountDeletedInfoBarDelegate::GetMessageText|
                    // for the informer text and link test placeholder empty substitution
                    activity.getString(R.string.luxxle_sync_account_deleted_infobar_message, ""),
                    activity.getString(R.string.ok),
                    "",
                    activity.getString(R.string.luxxle_sync_account_deleted_infobar_link_text, ""),
                    false);
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "show " + e);
        }
    }

    private static void disableInformer() {
        LuxxleSyncWorker.get().clearAccountDeletedNoticePending();
    }
}
