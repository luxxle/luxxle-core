/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.decentralized_dns.settings;

import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.preference.PreferenceFragmentCompat;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleLocalState;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.components.browser_ui.settings.SettingsUtils;

public class ENSSettingsFragment extends PreferenceFragmentCompat {
    static final String PREF_ENS_RESOLVE_METHOD = "ens_resolve_method";
    static final String PREF_ENS_OFFCHAIN_LOOKUP_METHOD = "ens_offchain_lookup";

    @Override
    public void onCreatePreferences(@Nullable Bundle savedInstanceState, String rootKey) {
        getActivity().setTitle(R.string.ens_title);
        SettingsUtils.addPreferencesFromResource(this, R.xml.ens_preferences);

        RadioButtonGroupDDnsResolveMethodPreference radioButtonGroupDDnsResolveMethodPreference =
                (RadioButtonGroupDDnsResolveMethodPreference) findPreference(
                        PREF_ENS_RESOLVE_METHOD);
        radioButtonGroupDDnsResolveMethodPreference.initialize(
                LuxxleLocalState.get().getInteger(LuxxlePref.ENS_RESOLVE_METHOD));
        radioButtonGroupDDnsResolveMethodPreference.setOnPreferenceChangeListener(
                (preference, newValue) -> {
                    LuxxleLocalState.get().setInteger(LuxxlePref.ENS_RESOLVE_METHOD, (int) newValue);
                    return true;
                });

        RadioButtonGroupEnsOffchainResolveMethodPreference
                radioButtonGroupEnsOffchainResolveMethodPreference =
                        (RadioButtonGroupEnsOffchainResolveMethodPreference) findPreference(
                                PREF_ENS_OFFCHAIN_LOOKUP_METHOD);
        radioButtonGroupEnsOffchainResolveMethodPreference.initialize(
                LuxxleLocalState.get().getInteger(LuxxlePref.ENS_OFFCHAIN_RESOLVE_METHOD));
        radioButtonGroupEnsOffchainResolveMethodPreference.setOnPreferenceChangeListener(
                (preference, newValue) -> {
                    LuxxleLocalState.get().setInteger(
                            LuxxlePref.ENS_OFFCHAIN_RESOLVE_METHOD, (int) newValue);
                    return true;
                });
    }
}
