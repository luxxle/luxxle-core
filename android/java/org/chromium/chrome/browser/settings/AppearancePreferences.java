/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import static org.chromium.chrome.browser.settings.MainSettings.PREF_UI_THEME;

import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.preference.Preference;

import org.chromium.base.LuxxleFeatureList;
import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.base.ContextUtils;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleFeatureUtil;
import org.chromium.chrome.browser.LuxxleRelaunchUtils;
import org.chromium.chrome.browser.LuxxleRewardsNativeWorker;
import org.chromium.chrome.browser.LuxxleRewardsObserver;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.multiwindow.LuxxleMultiWindowDialogFragment;
import org.chromium.chrome.browser.multiwindow.LuxxleMultiWindowUtils;
import org.chromium.chrome.browser.multiwindow.MultiWindowUtils;
import org.chromium.chrome.browser.night_mode.NightModeUtils;
import org.chromium.chrome.browser.ntp.NtpUtil;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.profiles.ProfileManager;
import org.chromium.chrome.browser.tasks.tab_management.LuxxleTabUiFeatureUtilities;
import org.chromium.chrome.browser.toolbar.ToolbarPositionController;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.toolbar.settings.AddressBarSettingsFragment;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.ui.base.DeviceFormFactor;

public class AppearancePreferences extends LuxxlePreferenceFragment
        implements Preference.OnPreferenceChangeListener, LuxxleRewardsObserver {
    public static final String PREF_HIDE_LUXXLE_REWARDS_ICON = "hide_luxxle_rewards_icon";
    public static final String PREF_HIDE_LUXXLE_REWARDS_ICON_MIGRATION =
            "hide_luxxle_rewards_icon_migration";
    public static final String PREF_SHOW_LUXXLE_REWARDS_ICON = "show_luxxle_rewards_icon";
    public static final String PREF_ADS_SWITCH = "ads_switch";
    public static final String PREF_LUXXLE_NIGHT_MODE_ENABLED = "luxxle_night_mode_enabled_key";
    public static final String PREF_LUXXLE_DISABLE_SHARING_HUB = "luxxle_disable_sharing_hub";
    public static final String PREF_LUXXLE_ENABLE_TAB_GROUPS = "luxxle_enable_tab_groups";
    public static final String PREF_LUXXLE_ENABLE_SPEEDREADER = "luxxle_enable_speedreader";
    public static final String PREF_ENABLE_MULTI_WINDOWS = "enable_multi_windows";
    public static final String PREF_SHOW_UNDO_WHEN_TABS_CLOSED = "show_undo_when_tabs_closed";
    public static final String PREF_ADDRESS_BAR = "address_bar";

    private LuxxleRewardsNativeWorker mLuxxleRewardsNativeWorker;

    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mPageTitle.set(getString(R.string.prefs_appearance));
        SettingsUtils.addPreferencesFromResource(this, R.xml.luxxle_appearance_preferences);
        boolean isTablet =
                DeviceFormFactor.isNonMultiDisplayContextOnTablet(
                        ContextUtils.getApplicationContext());
        if (isTablet) {
            removePreferenceIfPresent(LuxxlePreferenceKeys.LUXXLE_BOTTOM_TOOLBAR_ENABLED_KEY);
        }

        if (!NightModeUtils.isNightModeSupported()) {
            removePreferenceIfPresent(PREF_UI_THEME);
        }

        mLuxxleRewardsNativeWorker = LuxxleRewardsNativeWorker.getInstance();
        if (mLuxxleRewardsNativeWorker == null || !mLuxxleRewardsNativeWorker.isSupported()) {
            removePreferenceIfPresent(PREF_SHOW_LUXXLE_REWARDS_ICON);
        }

        if (!ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_SPEEDREADER)) {
            removePreferenceIfPresent(PREF_LUXXLE_ENABLE_SPEEDREADER);
        }
        if (!new LuxxleMultiWindowUtils().shouldShowEnableWindow(getActivity())) {
            removePreferenceIfPresent(PREF_ENABLE_MULTI_WINDOWS);
        }

        if (!ToolbarPositionController.isToolbarPositionCustomizationEnabled(getContext(), false)) {
            removePreferenceIfPresent(PREF_ADDRESS_BAR);
        }
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {}

    private void removePreferenceIfPresent(String key) {
        Preference preference = getPreferenceScreen().findPreference(key);
        if (preference != null) getPreferenceScreen().removePreference(preference);
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        ChromeSwitchPreference showLuxxleRewardsIconPref =
                (ChromeSwitchPreference) findPreference(PREF_SHOW_LUXXLE_REWARDS_ICON);
        if (showLuxxleRewardsIconPref != null) {
            showLuxxleRewardsIconPref.setChecked(NtpUtil.shouldShowRewardsIcon());
            showLuxxleRewardsIconPref.setOnPreferenceChangeListener(this);
        }

        ChromeSwitchPreference adsSwitchPref =
                (ChromeSwitchPreference) findPreference(PREF_ADS_SWITCH);
        if (adsSwitchPref != null) {
            adsSwitchPref.setChecked(getPrefAdsInBackgroundEnabled());
            adsSwitchPref.setOnPreferenceChangeListener(this);
        }

        Preference nightModeEnabled = findPreference(PREF_LUXXLE_NIGHT_MODE_ENABLED);
        nightModeEnabled.setOnPreferenceChangeListener(this);
        if (nightModeEnabled instanceof ChromeSwitchPreference) {
            ((ChromeSwitchPreference) nightModeEnabled)
                    .setChecked(ChromeFeatureList.isEnabled(
                            LuxxleFeatureList.FORCE_WEB_CONTENTS_DARK_MODE));
        }

        Preference enableBottomToolbar =
                findPreference(LuxxlePreferenceKeys.LUXXLE_BOTTOM_TOOLBAR_ENABLED_KEY);
        if (enableBottomToolbar != null) {
            enableBottomToolbar.setOnPreferenceChangeListener(this);
        }

        Preference disableSharingHub = findPreference(PREF_LUXXLE_DISABLE_SHARING_HUB);
        if (disableSharingHub != null) {
            disableSharingHub.setOnPreferenceChangeListener(this);
            if (disableSharingHub instanceof ChromeSwitchPreference) {
                ((ChromeSwitchPreference) disableSharingHub)
                        .setChecked(
                                ChromeSharedPreferences.getInstance()
                                        .readBoolean(
                                                LuxxlePreferenceKeys.LUXXLE_DISABLE_SHARING_HUB,
                                                false));
            }
        }

        Preference enableTabGroups = findPreference(PREF_LUXXLE_ENABLE_TAB_GROUPS);
        if (enableTabGroups != null) {
            enableTabGroups.setOnPreferenceChangeListener(this);
            if (enableTabGroups instanceof ChromeSwitchPreference) {
                ((ChromeSwitchPreference) enableTabGroups)
                        .setChecked(LuxxleTabUiFeatureUtilities.isLuxxleTabGroupsEnabled());
            }
        }

        Preference enableSpeedreader = findPreference(PREF_LUXXLE_ENABLE_SPEEDREADER);
        if (enableSpeedreader != null) {
            enableSpeedreader.setOnPreferenceChangeListener(this);
            if (enableSpeedreader instanceof ChromeSwitchPreference) {
                ((ChromeSwitchPreference) enableSpeedreader)
                        .setChecked(
                                UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                                        .getBoolean(LuxxlePref.SPEEDREADER_PREF_ENABLED));
            }
        }

        Preference enableMultiWindow = findPreference(PREF_ENABLE_MULTI_WINDOWS);
        if (enableMultiWindow != null) {
            enableMultiWindow.setOnPreferenceChangeListener(this);
            if (enableMultiWindow instanceof ChromeSwitchPreference) {
                ((ChromeSwitchPreference) enableMultiWindow)
                        .setChecked(LuxxleMultiWindowUtils.shouldEnableMultiWindows());
            }
        }

        ChromeSwitchPreference showUndoButtonOnTabClosed =
                (ChromeSwitchPreference) findPreference(PREF_SHOW_UNDO_WHEN_TABS_CLOSED);
        if (showUndoButtonOnTabClosed != null) {
            showUndoButtonOnTabClosed.setOnPreferenceChangeListener(this);
            ((ChromeSwitchPreference) showUndoButtonOnTabClosed)
                    .setChecked(
                            ChromeSharedPreferences.getInstance()
                                    .readBoolean(
                                            LuxxlePreferenceKeys.SHOW_UNDO_WHEN_TABS_CLOSED, true));
        }
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    @Override
    public void onStart() {
        if (mLuxxleRewardsNativeWorker != null) {
            mLuxxleRewardsNativeWorker.addObserver(this);
        }
        super.onStart();

        if (ToolbarPositionController.isToolbarPositionCustomizationEnabled(getContext(), false)) {
            updatePreferenceTitle(
                    PREF_ADDRESS_BAR, AddressBarSettingsFragment.getTitle(getContext()));
            updatePreferenceIcon(
                    PREF_ADDRESS_BAR,
                    BottomToolbarConfiguration.isToolbarTopAnchored()
                            ? R.drawable.ic_browser_mobile_tabs_top
                            : R.drawable.ic_browser_mobile_tabs_bottom);
        }

        Preference enableBottomToolbar =
                findPreference(LuxxlePreferenceKeys.LUXXLE_BOTTOM_TOOLBAR_ENABLED_KEY);
        if (enableBottomToolbar instanceof ChromeSwitchPreference) {
            if (BottomToolbarConfiguration.isToolbarTopAnchored()) {
                boolean isTablet =
                        DeviceFormFactor.isNonMultiDisplayContextOnTablet(
                                ContextUtils.getApplicationContext());
                ((ChromeSwitchPreference) enableBottomToolbar)
                        .setChecked(
                                !isTablet
                                        && BottomToolbarConfiguration
                                                .isLuxxleBottomControlsEnabled());
            }
            if (BottomToolbarConfiguration.isToolbarBottomAnchored()) {
                updatePreferenceSummary(
                        LuxxlePreferenceKeys.LUXXLE_BOTTOM_TOOLBAR_ENABLED_KEY,
                        R.string.luxxle_bottom_navigation_toolbar_disabled_summary);
            } else {
                updatePreferenceSummary(
                        LuxxlePreferenceKeys.LUXXLE_BOTTOM_TOOLBAR_ENABLED_KEY,
                        ((ChromeSwitchPreference) enableBottomToolbar).isChecked()
                                ? R.string.text_on
                                : R.string.text_off);
            }
            ((ChromeSwitchPreference) enableBottomToolbar)
                    .setEnabled(BottomToolbarConfiguration.isToolbarTopAnchored());
        }
    }

    @Override
    public void onStop() {
        if (mLuxxleRewardsNativeWorker != null) {
            mLuxxleRewardsNativeWorker.removeObserver(this);
        }
        super.onStop();
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String key = preference.getKey();
        boolean shouldRelaunch = false;
        if (LuxxlePreferenceKeys.LUXXLE_BOTTOM_TOOLBAR_ENABLED_KEY.equals(key)) {
            SharedPreferences prefs = ContextUtils.getAppSharedPreferences();
            Boolean originalStatus = BottomToolbarConfiguration.isLuxxleBottomControlsEnabled();
            updatePreferenceSummary(
                    LuxxlePreferenceKeys.LUXXLE_BOTTOM_TOOLBAR_ENABLED_KEY,
                    !originalStatus ? R.string.text_on : R.string.text_off);
            prefs.edit()
                    .putBoolean(
                            LuxxlePreferenceKeys.LUXXLE_BOTTOM_TOOLBAR_ENABLED_KEY, !originalStatus)
                    .apply();
            shouldRelaunch = true;
        } else if (PREF_SHOW_LUXXLE_REWARDS_ICON.equals(key)) {
            SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
            SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
            sharedPreferencesEditor.putBoolean(PREF_SHOW_LUXXLE_REWARDS_ICON, !(boolean) newValue);
            sharedPreferencesEditor.apply();
            shouldRelaunch = true;
        } else if (PREF_ADS_SWITCH.equals(key)) {
            setPrefAdsInBackgroundEnabled((boolean) newValue);
        } else if (PREF_LUXXLE_NIGHT_MODE_ENABLED.equals(key)) {
            LuxxleFeatureUtil.enableFeature(
                    LuxxleFeatureList.ENABLE_FORCE_DARK, (boolean) newValue, true);
            shouldRelaunch = true;
        } else if (PREF_LUXXLE_DISABLE_SHARING_HUB.equals(key)) {
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(
                            LuxxlePreferenceKeys.LUXXLE_DISABLE_SHARING_HUB, (boolean) newValue);
        } else if (PREF_LUXXLE_ENABLE_TAB_GROUPS.equals(key)) {
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(LuxxlePreferenceKeys.LUXXLE_TAB_GROUPS_ENABLED, (boolean) newValue);
        } else if (PREF_LUXXLE_ENABLE_SPEEDREADER.equals(key)) {
            UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                    .setBoolean(LuxxlePref.SPEEDREADER_PREF_ENABLED, (boolean) newValue);
            shouldRelaunch = true;
        } else if (PREF_ENABLE_MULTI_WINDOWS.equals(key)) {
            if (!(boolean) newValue) {
                if (MultiWindowUtils.getInstanceCount() > 1) {
                    LuxxleMultiWindowDialogFragment dialogFragment =
                            LuxxleMultiWindowDialogFragment.newInstance();
                    LuxxleMultiWindowDialogFragment.DismissListener dismissListener =
                            new LuxxleMultiWindowDialogFragment.DismissListener() {
                                @Override
                                public void onDismiss() {
                                    if (MultiWindowUtils.getInstanceCount() == 1) {
                                        if (preference instanceof ChromeSwitchPreference) {
                                            ((ChromeSwitchPreference) preference).setChecked(false);
                                            LuxxleMultiWindowUtils.updateEnableMultiWindows(false);
                                        }
                                    }
                                }
                            };
                    dialogFragment.setDismissListener(dismissListener);

                    dialogFragment.show(
                            getActivity().getSupportFragmentManager(),
                            "LuxxleMultiWindowDialogFragment");

                    return false;
                }
            }
            LuxxleMultiWindowUtils.updateEnableMultiWindows((boolean) newValue);
        } else if (PREF_SHOW_UNDO_WHEN_TABS_CLOSED.equals(key)) {
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(
                            LuxxlePreferenceKeys.SHOW_UNDO_WHEN_TABS_CLOSED, (boolean) newValue);
        }
        if (shouldRelaunch) {
            LuxxleRelaunchUtils.askForRelaunch(getActivity());
        }

        return true;
    }

    /** Returns the user preference for whether the luxxle ads in background is enabled. */
    public static boolean getPrefAdsInBackgroundEnabled() {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        return sharedPreferences.getBoolean(PREF_ADS_SWITCH, false);
    }

    /** Sets the user preference for whether the luxxle ads in background is enabled. */
    public void setPrefAdsInBackgroundEnabled(boolean enabled) {
        SharedPreferences sharedPreferences = ContextUtils.getAppSharedPreferences();
        SharedPreferences.Editor sharedPreferencesEditor = sharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(PREF_ADS_SWITCH, enabled);
        sharedPreferencesEditor.apply();
    }

    private void updatePreferenceIcon(String preferenceString, int drawable) {
        Preference preference = findPreference(preferenceString);
        if (preference != null) {
            preference.setIcon(drawable);
        }
    }

    private void updatePreferenceTitle(String preferenceString, CharSequence title) {
        Preference preference = findPreference(preferenceString);
        if (preference != null) {
            preference.setTitle(title);
        }
    }

    private void updatePreferenceSummary(String preferenceString, int summaryId) {
        Preference preference = findPreference(preferenceString);
        if (preference != null) {
            preference.setSummary(summaryId);
        }
    }
}
