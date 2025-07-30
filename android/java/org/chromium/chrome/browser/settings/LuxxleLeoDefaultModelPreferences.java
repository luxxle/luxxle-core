/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;

import androidx.annotation.Nullable;

import org.chromium.ai_chat.mojom.ModelWithSubtitle;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.luxxle_leo.LuxxleLeoMojomHelper;
import org.chromium.components.browser_ui.settings.SettingsUtils;

public class LuxxleLeoDefaultModelPreferences extends LuxxlePreferenceFragment
        implements LuxxleLeoRadioButtonGroupDefaultModelPreference.RadioButtonsDelegate {
    private static final String PREF_DEFAULT_MODEL_GROUP = "default_model";
    private LuxxleLeoRadioButtonGroupDefaultModelPreference mRadioButtons;
    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    @Override
    public void onCreatePreferences(@Nullable Bundle savedInstanceState, String rootKey) {
        SettingsUtils.addPreferencesFromResource(this, R.xml.luxxle_leo_default_model_preferences);
        mPageTitle.set(getString(R.string.leo_default_model_pref_screen));
        mRadioButtons =
                (LuxxleLeoRadioButtonGroupDefaultModelPreference)
                        findPreference(PREF_DEFAULT_MODEL_GROUP);
        mRadioButtons.initialize(this);
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    @Override
    public void fetchModels() {
        LuxxleLeoMojomHelper.getInstance(getProfile())
                .getModels(
                        models -> {
                            fetchDefaultModelInitRadioButtons(models);
                        });
    }

    private void fetchDefaultModelInitRadioButtons(ModelWithSubtitle[] models) {
        LuxxleLeoMojomHelper.getInstance(getProfile())
                .getDefaultModelKey(
                        defaultModel -> {
                            mRadioButtons.initializeModels(models, defaultModel);
                        });
    }

    @Override
    public void setDefaultModel(String key) {
        LuxxleLeoMojomHelper.getInstance(getProfile()).setDefaultModelKey(key);
    }
}
