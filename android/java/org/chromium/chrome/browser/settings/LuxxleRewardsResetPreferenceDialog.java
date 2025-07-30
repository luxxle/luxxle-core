/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;
import android.view.View;

import androidx.preference.PreferenceDialogFragmentCompat;

import org.chromium.chrome.browser.LuxxleRewardsNativeWorker;

/**
 * The dialog used to reset Luxxle Rewards.
 */
public class LuxxleRewardsResetPreferenceDialog extends PreferenceDialogFragmentCompat {
    public static final String TAG = "LuxxleRewardsResetPreferenceDialog";

    public static LuxxleRewardsResetPreferenceDialog newInstance(
            LuxxleRewardsResetPreference preference) {
        LuxxleRewardsResetPreferenceDialog fragment = new LuxxleRewardsResetPreferenceDialog();
        Bundle bundle = new Bundle(1);
        bundle.putString(PreferenceDialogFragmentCompat.ARG_KEY, preference.getKey());
        fragment.setArguments(bundle);
        return fragment;
    }

    @Override
    protected void onBindDialogView(View view) {
        super.onBindDialogView(view);
    }

    @Override
    public void onDialogClosed(boolean positive) {
        if (positive) {
            LuxxleRewardsNativeWorker.getInstance().resetTheWholeState();
        }
    }
}
