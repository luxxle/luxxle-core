/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.Toast;

import androidx.preference.Preference;
import androidx.preference.Preference.OnPreferenceChangeListener;

import org.chromium.base.ContextUtils;
import org.chromium.base.ThreadUtils;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.local_database.DatabaseHelper;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;

/**
 * Fragment to keep track of all the display related preferences.
 */
public class LuxxleStatsPreferences extends LuxxlePreferenceFragment
    implements OnPreferenceChangeListener {

    // deprecated preferences from browser-android-tabs
    public static final String PREF_LUXXLE_STATS = "luxxle_stats";
    public static final String PREF_LUXXLE_STATS_NOTIFICATION = "luxxle_stats_notification";
    public static final String PREF_CLEAR_LUXXLE_STATS = "clear_luxxle_stats";

    private ChromeSwitchPreference mLuxxleStatsPref;
    private ChromeSwitchPreference mLuxxleStatsNotificationPref;

    private DatabaseHelper mDatabaseHelper = DatabaseHelper.getInstance();

    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    public static int getPreferenceSummary() {
        return OnboardingPrefManager.getInstance().isLuxxleStatsEnabled()
                ? R.string.text_on
                : R.string.text_off;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mPageTitle.set(getString(R.string.luxxle_stats));
        SettingsUtils.addPreferencesFromResource(this, R.xml.luxxle_stats_preferences);
        findPreference(PREF_CLEAR_LUXXLE_STATS)
                .setOnPreferenceClickListener(
                        new Preference.OnPreferenceClickListener() {
                            @Override
                            public boolean onPreferenceClick(Preference preference) {
                                new AsyncTask<Void>() {
                                    @Override
                                    protected Void doInBackground() {
                                        try {
                                            mDatabaseHelper.clearStatsTable();
                                            mDatabaseHelper.clearSavedBandwidthTable();
                                        } catch (Exception e) {
                                            // Do nothing if url is invalid.
                                            // Just return w/o showing shields popup.
                                            return null;
                                        }
                                        return null;
                                    }

                                    @Override
                                    protected void onPostExecute(Void result) {
                                        assert ThreadUtils.runningOnUiThread();
                                        if (isCancelled()) return;
                                        Toast.makeText(
                                                        ContextUtils.getApplicationContext(),
                                                        ContextUtils.getApplicationContext()
                                                                .getResources()
                                                                .getString(
                                                                        R.string
                                                                                .data_has_been_cleared),
                                                        Toast.LENGTH_SHORT)
                                                .show();
                                    }
                                }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
                                return true;
                            }
                        });
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        mLuxxleStatsPref = (ChromeSwitchPreference) findPreference(PREF_LUXXLE_STATS);
        if (mLuxxleStatsPref != null) {
            mLuxxleStatsPref.setEnabled(true);
            mLuxxleStatsPref.setChecked(OnboardingPrefManager.getInstance().isLuxxleStatsEnabled());
            mLuxxleStatsPref.setOnPreferenceChangeListener(this);
        }
        mLuxxleStatsNotificationPref =
                (ChromeSwitchPreference) findPreference(PREF_LUXXLE_STATS_NOTIFICATION);
        if (mLuxxleStatsNotificationPref != null) {
            mLuxxleStatsNotificationPref.setEnabled(true);
            mLuxxleStatsNotificationPref.setChecked(
                    OnboardingPrefManager.getInstance().isLuxxleStatsNotificationEnabled());
            mLuxxleStatsNotificationPref.setOnPreferenceChangeListener(this);
        }
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        setOnPreferenceValue(preference.getKey(), (boolean)newValue);
        return true;
    }

    public static void setOnPreferenceValue(String preferenceName, boolean newValue) {
        if (PREF_LUXXLE_STATS.equals(preferenceName)) {
            OnboardingPrefManager.getInstance().setLuxxleStatsEnabled((boolean)newValue);
        } else if (PREF_LUXXLE_STATS_NOTIFICATION.equals(preferenceName)) {
            OnboardingPrefManager.getInstance().setLuxxleStatsNotificationEnabled((boolean)newValue);
        } else {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(preferenceName, newValue);
            sharedPreferencesEditor.apply();
        }
    }

    public static void setOnPreferenceValue(String preferenceName, int newValue) {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
        sharedPreferencesEditor.putInt(preferenceName, newValue);
        sharedPreferencesEditor.apply();
    }
}
