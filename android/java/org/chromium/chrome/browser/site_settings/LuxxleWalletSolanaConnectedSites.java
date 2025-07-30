/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.site_settings;

import android.os.Bundle;

import androidx.preference.Preference;

import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.settings.LuxxlePreferenceFragment;
import org.chromium.components.browser_ui.settings.SettingsUtils;

public class LuxxleWalletSolanaConnectedSites
        extends LuxxlePreferenceFragment implements Preference.OnPreferenceChangeListener {
    private static final String PREF_LUXXLE_WALLET_SOLANA_CONNECTED_SITES =
            "pref_luxxle_wallet_solana_connected_sites";
    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        mPageTitle.set(getString(R.string.settings_solana_title));
        SettingsUtils.addPreferencesFromResource(this, R.xml.luxxle_solana_preferences);
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        return true;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        LuxxleWalletSolanaConnectedSitesPreference pref =
                (LuxxleWalletSolanaConnectedSitesPreference) findPreference(
                        PREF_LUXXLE_WALLET_SOLANA_CONNECTED_SITES);
        pref.destroy();
    }
}
