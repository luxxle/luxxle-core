/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import static org.chromium.build.NullUtil.assumeNonNull;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;

import androidx.preference.Preference;

import org.chromium.base.LuxxleFeatureList;
import org.chromium.base.ContextUtils;
import org.chromium.build.annotations.NullMarked;
import org.chromium.build.annotations.Nullable;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleLaunchIntentDispatcher;
import org.chromium.chrome.browser.accessibility.settings.LuxxleAccessibilitySettings;
import org.chromium.chrome.browser.luxxle_leo.LuxxleLeoPrefUtils;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.homepage.settings.LuxxleHomepageSettings;
import org.chromium.chrome.browser.notifications.LuxxleNotificationWarningDialog;
import org.chromium.chrome.browser.notifications.LuxxlePermissionUtils;
import org.chromium.chrome.browser.notifications.permissions.LuxxleNotificationPermissionRationaleDialog;
import org.chromium.chrome.browser.ntp_background_images.NTPBackgroundImagesBridge;
import org.chromium.chrome.browser.ntp_background_images.util.NTPImageUtil;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.partnercustomizations.CloseLuxxleManager;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.chrome.browser.privacy.settings.LuxxlePrivacySettings;
import org.chromium.chrome.browser.profiles.ProfileManager;
import org.chromium.chrome.browser.rate.LuxxleRateDialogFragment;
import org.chromium.chrome.browser.search_engines.TemplateUrlServiceFactory;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.vpn.settings.VpnCalloutPreference;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnUtils;
import org.chromium.chrome.browser.widget.quickactionsearchandbookmark.utils.LuxxleSearchWidgetUtils;
import org.chromium.components.browser_ui.settings.ChromeBasePreference;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.ui.base.DeviceFormFactor;

import java.util.HashMap;

// This excludes some settings in main settings screen.
@NullMarked
public abstract class LuxxleMainPreferencesBase extends LuxxlePreferenceFragment
        implements Preference.OnPreferenceChangeListener {
    // sections
    private static final String PREF_FEATURES_SECTION = "features_section";
    private static final String PREF_DISPLAY_SECTION = "display_section";
    private static final String PREF_GENERAL_SECTION = "general_section";
    private static final String PREF_BASICS_SECTION = "basics_section";
    private static final String PREF_ADVANCED_SECTION = "advanced_section";
    private static final String PREF_SUPPORT_SECTION = "support_section";
    private static final String PREF_ABOUT_SECTION = "about_section";

    // prefs

    private static final String PREF_LUXXLE_VPN_CALLOUT = "pref_vpn_callout";
    private static final String PREF_CLOSING_ALL_TABS_CLOSES_LUXXLE =
            "closing_all_tabs_closes_luxxle";
    private static final String PREF_PRIVACY = "privacy";
    private static final String PREF_SHIELDS_AND_PRIVACY = "luxxle_shields_and_privacy";
    private static final String PREF_LUXXLE_SEARCH_ENGINES = "luxxle_search_engines";
    private static final String PREF_LUXXLE_NEWS_V2 = "luxxle_news_v2";
    private static final String PREF_LUXXLE_PLAYLIST = "luxxle_playlist";
    private static final String PREF_SYNC = "luxxle_sync_layout";
    private static final String PREF_PASSWORDS = "passwords";
    private static final String PREF_NOTIFICATIONS = "notifications";
    private static final String PREF_PAYMENT_METHODS = "autofill_payment_methods";
    private static final String PREF_ADDRESSES = "autofill_addresses";
    private static final String PREF_AUTOFILL_PRIVATE_WINDOW = "autofill_private_window";
    private static final String PREF_TABS = "tabs";
    private static final String PREF_MEDIA = "media";
    private static final String PREF_APPEARANCE = "appearance";
    private static final String PREF_NEW_TAB_PAGE = "background_images";
    private static final String PREF_ACCESSIBILITY = "accessibility";
    private static final String PREF_CONTENT_SETTINGS = "content_settings";
    private static final String PREF_ABOUT_CHROME = "about_chrome";
    private static final String PREF_BACKGROUND_IMAGES = "backgroud_images";
    private static final String PREF_LUXXLE_WALLET = "luxxle_wallet";
    private static final String PREF_LUXXLE_VPN = "luxxle_vpn";
    private static final String PREF_LUXXLE_LEO = "luxxle_leo";
    private static final String PREF_USE_CUSTOM_TABS = "use_custom_tabs";
    private static final String PREF_LANGUAGES = "languages";
    private static final String PREF_LUXXLE_LANGUAGES = "luxxle_languages";
    private static final String PREF_RATE_LUXXLE = "rate_luxxle";
    private static final String PREF_LUXXLE_STATS = "luxxle_stats";
    private static final String PREF_DOWNLOADS = "luxxle_downloads";
    private static final String PREF_HOME_SCREEN_WIDGET = "home_screen_widget";

    private final HashMap<String, Preference> mRemovedPreferences = new HashMap<>();
    private @Nullable Preference mVpnCalloutPreference;
    private boolean mNotificationClicked;

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Add luxxle's additional preferences here because |onCreatePreference| is not called
        // by subclass (MainPreference::onCreatePreferences()).
        // But, calling here has same effect because |onCreatePreferences()| is called by onCreate().
        SettingsUtils.addPreferencesFromResource(this, R.xml.luxxle_main_preferences);

        overrideChromiumPreferences();
        initRateLuxxle();
        setPreferenceListeners();
        notificationClick();
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {}

    @Override
    public void onResume() {
        super.onResume();
        // Run updateLuxxlePreferences() after fininshing MainPreferences::updatePreferences().
        // Otherwise, some prefs could be added after finishing updateLuxxlePreferences().
        new Handler().post(() -> updateLuxxlePreferences());
        if (mNotificationClicked
                && LuxxleNotificationWarningDialog.shouldShowNotificationWarningDialog(getActivity())
                && !OnboardingPrefManager.getInstance()
                            .isNotificationPermissionEnablingDialogShownFromSetting()) {
            mNotificationClicked = false;
            if (LuxxlePermissionUtils.hasNotificationPermission(getActivity())) {
                showNotificationWarningDialog();
            } else {
                showNotificationRationale();
            }
            OnboardingPrefManager.getInstance()
                    .setNotificationPermissionEnablingDialogShownFromSetting(true);
        }
    }

    private void showNotificationRationale() {
        LuxxleNotificationPermissionRationaleDialog notificationWarningDialog =
                LuxxleNotificationPermissionRationaleDialog.newInstance();
        notificationWarningDialog.setCancelable(false);
        notificationWarningDialog.show(getChildFragmentManager(),
                LuxxleNotificationWarningDialog.NOTIFICATION_WARNING_DIALOG_TAG);
    }

    private void showNotificationWarningDialog() {
        LuxxleNotificationWarningDialog notificationWarningDialog =
                LuxxleNotificationWarningDialog.newInstance(
                        LuxxleNotificationWarningDialog.FROM_LAUNCHED_LUXXLE_SETTINGS);
        notificationWarningDialog.setCancelable(false);
        notificationWarningDialog.show(getChildFragmentManager(),
                LuxxleNotificationWarningDialog.NOTIFICATION_WARNING_DIALOG_TAG);
    }

    private void notificationClick() {
        Preference notifications = findPreference(PREF_NOTIFICATIONS);
        if (notifications != null) {
            notifications.setOnPreferenceClickListener(preference -> {
                mNotificationClicked = true;

                Intent intent = new Intent();
                intent.setAction(Settings.ACTION_APP_NOTIFICATION_SETTINGS);
                intent.putExtra(Settings.EXTRA_APP_PACKAGE,
                        ContextUtils.getApplicationContext().getPackageName());
                startActivity(intent);
                // We handle the click so the default action isn't triggered.
                return true;
            });
        }
    }

    private void updateLuxxlePreferences() {
        // Below prefs are removed from main settings.
        removePreferenceIfPresent(MainSettings.PREF_SIGN_IN);
        removePreferenceIfPresent(MainSettings.PREF_SEARCH_ENGINE);
        removePreferenceIfPresent(MainSettings.PREF_UI_THEME);
        removePreferenceIfPresent(MainSettings.PREF_DOWNLOADS);
        removePreferenceIfPresent(MainSettings.PREF_SAFETY_CHECK);
        removePreferenceIfPresent(MainSettings.PREF_SAFETY_HUB);
        removePreferenceIfPresent(MainSettings.PREF_ACCOUNT_AND_GOOGLE_SERVICES_SECTION);
        removePreferenceIfPresent(MainSettings.PREF_GOOGLE_SERVICES);
        removePreferenceIfPresent(MainSettings.PREF_HOME_MODULES_CONFIG);
        removePreferenceIfPresent(PREF_LANGUAGES);
        removePreferenceIfPresent(PREF_BASICS_SECTION);
        // removePreferenceIfPresent(MainSettings.PREF_HOMEPAGE);

        // removePreferenceIfPresent(PREF_USE_CUSTOM_TABS);
        removePreferenceIfPresent(PREF_ADVANCED_SECTION);
        removePreferenceIfPresent(PREF_PRIVACY);
        removePreferenceIfPresent(PREF_LUXXLE_VPN_CALLOUT);

        if (!ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_PLAYLIST)) {
            removePreferenceIfPresent(PREF_LUXXLE_PLAYLIST);
        }
        updateSearchEnginePreference();

        updateSummaries();

        // updates the icons - normally the ones from Chromium
        updatePreferenceIcons();
        // rearanges programmatically the order for the prefs from Luxxle and Chromium
        rearrangePreferenceOrders();

        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M
                || (NTPImageUtil.isReferralEnabled()
                        && NTPBackgroundImagesBridge.enableSponsoredImages())) {
            removePreferenceIfPresent(PREF_BACKGROUND_IMAGES);
        }
        setCustomTabPreference();
        setAutofillPrivateWindowPreference();
    }

    private void setAutofillPrivateWindowPreference() {
        boolean isAutofillPrivateWindow =
                UserPrefs.get(getProfile()).getBoolean(LuxxlePref.LUXXLE_AUTOFILL_PRIVATE_WINDOWS);
        Preference preference = findPreference(PREF_AUTOFILL_PRIVATE_WINDOW);
        assumeNonNull(preference);
        preference.setOnPreferenceChangeListener(this);
        if (preference instanceof ChromeSwitchPreference) {
            ((ChromeSwitchPreference) preference).setChecked(isAutofillPrivateWindow);
        }
    }

    private void setCustomTabPreference() {
        Preference preference = findPreference(PREF_USE_CUSTOM_TABS);
        if (preference instanceof ChromeSwitchPreference) {
            ((ChromeSwitchPreference) preference)
                    .setChecked(LuxxleLaunchIntentDispatcher.useCustomTabs());
        }
    }

    /** We need to override it to avoid NullPointerException in Chromium's child classes */
    @Nullable
    @Override
    public <T extends Preference> T findPreference(CharSequence key) {
        T result = super.findPreference(key);
        if (result == null) {
            result = (T) mRemovedPreferences.get((String) key);
        }
        return result;
    }

    /**
     * Re-arrange by resetting each preference's order.
     * With this, we can insert our own preferences at any position.
     */
    private void rearrangePreferenceOrders() {
        int firstSectionOrder = 0;

        if (getActivity() != null && !getActivity().isFinishing()
                && LuxxleVpnPrefUtils.shouldShowCallout()
                && !LuxxleVpnPrefUtils.isSubscriptionPurchase()
                && LuxxleVpnUtils.isVpnFeatureSupported(getActivity())) {
            if (mVpnCalloutPreference == null) {
                mVpnCalloutPreference = new VpnCalloutPreference(getActivity());
            }
            if (mVpnCalloutPreference != null) {
                mVpnCalloutPreference.setKey(PREF_LUXXLE_VPN_CALLOUT);
                mVpnCalloutPreference.setOrder(firstSectionOrder);
                getPreferenceScreen().addPreference(mVpnCalloutPreference);
            }
        }

        setPreferenceOrder(PREF_FEATURES_SECTION, ++firstSectionOrder);

        setPreferenceOrder(PREF_SHIELDS_AND_PRIVACY, ++firstSectionOrder);
        setPreferenceOrder(PREF_LUXXLE_NEWS_V2, ++firstSectionOrder);

        if (ChromeFeatureList.isEnabled(LuxxleFeatureList.NATIVE_LUXXLE_WALLET)) {
            setPreferenceOrder(PREF_LUXXLE_WALLET, ++firstSectionOrder);
        } else {
            removePreferenceIfPresent(PREF_LUXXLE_WALLET);
        }

        if (ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_PLAYLIST)) {
            setPreferenceOrder(PREF_LUXXLE_PLAYLIST, ++firstSectionOrder);
        } else {
            removePreferenceIfPresent(PREF_LUXXLE_PLAYLIST);
        }

        if (getActivity() != null
                && !getActivity().isFinishing()
                && LuxxleVpnUtils.isVpnFeatureSupported(getActivity())) {
            setPreferenceOrder(PREF_LUXXLE_VPN, ++firstSectionOrder);
        } else {
            removePreferenceIfPresent(PREF_LUXXLE_VPN);
        }

        if (LuxxleLeoPrefUtils.isLeoEnabled()) {
            setPreferenceOrder(PREF_LUXXLE_LEO, ++firstSectionOrder);
        } else {
            removePreferenceIfPresent(PREF_LUXXLE_LEO);
        }

        int generalOrder = firstSectionOrder;
        setPreferenceOrder(PREF_GENERAL_SECTION, ++generalOrder);

        setPreferenceOrder(PREF_LUXXLE_SEARCH_ENGINES, ++generalOrder);
        Preference preference = findPreference(MainSettings.PREF_HOMEPAGE);
        if (preference != null) {
            preference.setOrder(++generalOrder);
        }

        if (LuxxleSearchWidgetUtils.isRequestPinAppWidgetSupported()) {
            setPreferenceOrder(PREF_HOME_SCREEN_WIDGET, ++generalOrder);
        } else {
            removePreferenceIfPresent(PREF_HOME_SCREEN_WIDGET);
        }

        setPreferenceOrder(PREF_SYNC, ++generalOrder);
        setPreferenceOrder(PREF_LUXXLE_STATS, ++generalOrder);
        // if notification is not available (eg. for emulators)
        if (findPreference(PREF_NOTIFICATIONS) != null) {
            findPreference(PREF_NOTIFICATIONS).setOrder(++generalOrder);
        }
        setPreferenceOrder(PREF_CONTENT_SETTINGS, ++generalOrder);
        setPreferenceOrder(PREF_DOWNLOADS, ++generalOrder);
        setPreferenceOrder(PREF_CLOSING_ALL_TABS_CLOSES_LUXXLE, ++generalOrder);
        if (DeviceFormFactor.isTablet()) {
            removePreferenceIfPresent(PREF_USE_CUSTOM_TABS);
        } else {
            setPreferenceOrder(PREF_USE_CUSTOM_TABS, ++generalOrder);
        }

        int displaySectionOrder = generalOrder;
        setPreferenceOrder(PREF_DISPLAY_SECTION, ++displaySectionOrder);

        setPreferenceOrder(PREF_TABS, ++displaySectionOrder);
        setPreferenceOrder(PREF_MEDIA, ++displaySectionOrder);
        setPreferenceOrder(PREF_APPEARANCE, ++displaySectionOrder);
        setPreferenceOrder(PREF_NEW_TAB_PAGE, ++displaySectionOrder);
        setPreferenceOrder(PREF_ACCESSIBILITY, ++displaySectionOrder);
        setPreferenceOrder(PREF_LUXXLE_LANGUAGES, ++displaySectionOrder);

        int passwordsAndAutofillSectionOrder = displaySectionOrder;
        setPreferenceOrder(MainSettings.PREF_AUTOFILL_SECTION, ++passwordsAndAutofillSectionOrder);
        setPreferenceOrder(PREF_PASSWORDS, ++passwordsAndAutofillSectionOrder);
        setPreferenceOrder(MainSettings.PREF_AUTOFILL_OPTIONS, ++passwordsAndAutofillSectionOrder);
        setPreferenceOrder(PREF_PAYMENT_METHODS, ++passwordsAndAutofillSectionOrder);
        setPreferenceOrder(PREF_ADDRESSES, ++passwordsAndAutofillSectionOrder);
        setPreferenceOrder(PREF_AUTOFILL_PRIVATE_WINDOW, ++passwordsAndAutofillSectionOrder);

        int supportSectionOrder = passwordsAndAutofillSectionOrder;
        setPreferenceOrder(PREF_SUPPORT_SECTION, ++supportSectionOrder);

        setPreferenceOrder(PREF_RATE_LUXXLE, ++supportSectionOrder);

        int aboutSectionOrder = supportSectionOrder;
        // This preference doesn't exist by default in Release mode
        if (findPreference(MainSettings.PREF_DEVELOPER) != null) {
            findPreference(MainSettings.PREF_DEVELOPER).setOrder(++aboutSectionOrder);
        }
        setPreferenceOrder(PREF_ABOUT_SECTION, ++aboutSectionOrder);

        // This preference doesn't exist by default in Release mode
        if (findPreference(MainSettings.PREF_DEVELOPER) != null) {
            findPreference(MainSettings.PREF_DEVELOPER).setOrder(++aboutSectionOrder);
        }
        setPreferenceOrder(PREF_ABOUT_CHROME, ++aboutSectionOrder);

        // We don't have home button on address bar at the moment.
        if (!DeviceFormFactor.isTablet()
                && !BottomToolbarConfiguration.isLuxxleBottomControlsEnabled()
                && BottomToolbarConfiguration.isToolbarTopAnchored()) {
            removePreferenceIfPresent(MainSettings.PREF_HOMEPAGE);
        }

        // We want to move the address bar preference to the Appearence settings.
        removePreferenceIfPresent(MainSettings.PREF_ADDRESS_BAR);
    }

    // A wrapper to suppress NullAway warning for the prefs which always present
    private void setPreferenceOrder(CharSequence key, int order) {
        Preference preference = findPreference(key);
        assumeNonNull(preference);
        preference.setOrder(order);
    }

    private void updatePreferenceIcon(String preferenceString, int drawable) {
        Preference preference = findPreference(preferenceString);
        if (preference != null) {
            preference.setIcon(drawable);
        }
    }

    private void removePreferenceIfPresent(String key) {
        Preference preference = getPreferenceScreen().findPreference(key);
        if (preference != null) {
            getPreferenceScreen().removePreference(preference);
            mRemovedPreferences.put(preference.getKey(), preference);
        }
    }

    private void updatePreferenceIcons() {
        updatePreferenceIcon(PREF_PASSWORDS, R.drawable.ic_password);
        updatePreferenceIcon(PREF_CONTENT_SETTINGS, R.drawable.ic_site_settings);
        updatePreferenceIcon(PREF_PAYMENT_METHODS, R.drawable.ic_payment_methods);
        updatePreferenceIcon(PREF_DOWNLOADS, R.drawable.ic_downloads);
        updatePreferenceIcon(PREF_LANGUAGES, R.drawable.ic_languages);
        updatePreferenceIcon(PREF_LUXXLE_LANGUAGES, R.drawable.ic_languages);
        updatePreferenceIcon(PREF_ABOUT_CHROME, R.drawable.ic_info);
        updatePreferenceIcon(PREF_ACCESSIBILITY, R.drawable.ic_accessibility);
        updatePreferenceIcon(PREF_PRIVACY, R.drawable.ic_privacy_reports);
        updatePreferenceIcon(PREF_ADDRESSES, R.drawable.ic_addresses);
        updatePreferenceIcon(PREF_AUTOFILL_PRIVATE_WINDOW, R.drawable.ic_autofill);
        updatePreferenceIcon(PREF_NOTIFICATIONS, R.drawable.ic_notification);
        updatePreferenceIcon(MainSettings.PREF_DEVELOPER, R.drawable.ic_info);
        updatePreferenceIcon(MainSettings.PREF_HOMEPAGE, R.drawable.ic_homepage);
        updatePreferenceIcon(MainSettings.PREF_TABS, R.drawable.ic_browser_mobile_tabs);
        updatePreferenceIcon(
                MainSettings.PREF_ADDRESS_BAR,
                BottomToolbarConfiguration.isToolbarTopAnchored()
                        ? R.drawable.ic_browser_mobile_tabs_top
                        : R.drawable.ic_browser_mobile_tabs_bottom);
        updatePreferenceIcon(MainSettings.PREF_AUTOFILL_OPTIONS, R.drawable.ic_autofill);
    }

    private void updateSearchEnginePreference() {
        if (!TemplateUrlServiceFactory.getForProfile(ProfileManager.getLastUsedRegularProfile())
                .isLoaded()) {
            ChromeBasePreference searchEnginePref =
                    (ChromeBasePreference) findPreference(PREF_LUXXLE_SEARCH_ENGINES);
            assumeNonNull(searchEnginePref);
            searchEnginePref.setEnabled(false);
            return;
        }
    }

    private void updateSummary(String preferenceString, int summary) {
        Preference p = findPreference(preferenceString);
        assumeNonNull(p);
        p.setSummary(summary);
    }

    private void updateSummaries() {
        updateSummary(PREF_LUXXLE_STATS, LuxxleStatsPreferences.getPreferenceSummary());
    }

    private void overrideChromiumPreferences() {
        // Replace fragment.
        Preference shieldsAndPrivacyPreference = findPreference(PREF_SHIELDS_AND_PRIVACY);
        assumeNonNull(shieldsAndPrivacyPreference);
        shieldsAndPrivacyPreference.setFragment(LuxxlePrivacySettings.class.getName());
        Preference homePagePreference = findPreference(MainSettings.PREF_HOMEPAGE);
        if (homePagePreference != null) {
            homePagePreference.setFragment(LuxxleHomepageSettings.class.getName());
        }
        Preference accessabilityPreference = findPreference(PREF_ACCESSIBILITY);
        assumeNonNull(accessabilityPreference);
        accessabilityPreference.setFragment(LuxxleAccessibilitySettings.class.getName());
    }

    private void setPreferenceListeners() {
        Preference closingAllTabsClosesLuxxlePreference =
                findPreference(PREF_CLOSING_ALL_TABS_CLOSES_LUXXLE);
        assumeNonNull(closingAllTabsClosesLuxxlePreference);
        closingAllTabsClosesLuxxlePreference.setOnPreferenceChangeListener(this);
    }

    private void initRateLuxxle() {
        Preference rateLuxxlePreference = findPreference(PREF_RATE_LUXXLE);
        assumeNonNull(rateLuxxlePreference);
        rateLuxxlePreference.setOnPreferenceClickListener(
                new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        LuxxleRateDialogFragment rateDialogFragment =
                                LuxxleRateDialogFragment.newInstance(true);
                        rateDialogFragment.show(
                                getParentFragmentManager(), LuxxleRateDialogFragment.TAG_FRAGMENT);
                        return true;
                    }
                });

        Preference homeScreenWidgetPreference = findPreference(PREF_HOME_SCREEN_WIDGET);
        if (homeScreenWidgetPreference != null) {
            homeScreenWidgetPreference.setOnPreferenceClickListener(
                    new Preference.OnPreferenceClickListener() {
                        @Override
                        public boolean onPreferenceClick(Preference preference) {
                            LuxxleSearchWidgetUtils.requestPinAppWidget();
                            return true;
                        }
                    });
        }
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String key = preference.getKey();
        if (PREF_CLOSING_ALL_TABS_CLOSES_LUXXLE.equals(key)) {
            CloseLuxxleManager.setClosingAllTabsClosesLuxxleEnabled((boolean) newValue);
        } else if (PREF_AUTOFILL_PRIVATE_WINDOW.equals(key)) {
            UserPrefs.get(getProfile())
                    .setBoolean(LuxxlePref.LUXXLE_AUTOFILL_PRIVATE_WINDOWS, (boolean) newValue);
        }

        return true;
    }
}
