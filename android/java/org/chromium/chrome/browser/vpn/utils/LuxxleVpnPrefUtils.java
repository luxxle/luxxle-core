/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.vpn.utils;

import org.chromium.base.ContextUtils;
import org.chromium.luxxle_vpn.mojom.LuxxleVpnConstants;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.profiles.ProfileManager;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnPrefModel;
import org.chromium.components.user_prefs.UserPrefs;

import java.util.Collections;
import java.util.Set;

public class LuxxleVpnPrefUtils {
    private static final String PREF_LUXXLE_VPN_CALLOUT = "luxxle_vpn_callout";
    private static final String PREF_LUXXLE_VPN_SUBSCRIPTION_PURCHASE =
            "luxxle_vpn_subscription_purchase";
    private static final String PREF_LUXXLE_VPN_PAYMENT_STATE = "luxxle_vpn_payment_state";
    private static final String PREF_LUXXLE_VPN_HOSTNAME = "luxxle_vpn_hostname";
    private static final String PREF_LUXXLE_VPN_HOSTNAME_DISPLAY = "luxxle_vpn_hostname_display";
    private static final String PREF_LUXXLE_VPN_PURCHASE_TOKEN = "luxxle_vpn_purchase_token";
    private static final String PREF_LUXXLE_VPN_PRODUCT_ID = "luxxle_vpn_product_id";
    private static final String PREF_LUXXLE_VPN_PURCHASE_EXPIRY = "luxxle_vpn_purchase_expiry";
    private static final String PREF_LUXXLE_VPN_SERVER_CHANGE_LOCATION = "server_change_location";
    private static final String PREF_LUXXLE_VPN_SERVER_COUNTRY = "server_country";
    private static final String PREF_LUXXLE_VPN_SERVER_CONTINENT = "server_continent";
    private static final String PREF_LUXXLE_VPN_SERVER_ISO_CODE = "server_iso_code";
    private static final String PREF_LUXXLE_VPN_SERVER_NAME_PRETTY = "server_name_pretty";
    private static final String PREF_LUXXLE_VPN_SERVER_PRECISION = "server_precision";
    private static final String VPN_AUTOMATIC_SERVER_SELECTION = "vpn_automatic_server_selection";

    private static final String PREF_LUXXLE_VPN_RESET_CONFIGURATION =
            "luxxle_vpn_reset_configuration";
    private static final String PREF_EXCLUDED_PACKAGES = "excluded_packages";

    public static final String PREF_LUXXLE_VPN_AUTOMATIC = "automatic";
    public static final String PREF_LUXXLE_VPN_START = "luxxle_vpn_start";

    public static final String PREF_LUXXLE_VPN_API_AUTH_TOKEN = "luxxle_vpn_api_auth_token";
    public static final String PREF_LUXXLE_VPN_SUBSCRIBER_CREDENTIAL =
            "luxxle_vpn_subscriber_credential";
    public static final String PREF_LUXXLE_VPN_CLIENT_ID = "luxxle_vpn_client_id";
    public static final String PREF_LUXXLE_VPN_SERVER_PUBLIC_KEY = "luxxle_vpn_server_public_key";
    public static final String PREF_LUXXLE_VPN_IP_ADDRESS = "luxxle_vpn_ip_address";
    public static final String PREF_LUXXLE_VPN_CLIENT_PRIVATE_KEY = "luxxle_vpn_client_private_key";
    public static final String PREF_SESSION_START_TIME = "luxxle_vpn_session_start_time";
    public static final String PREF_SESSION_END_TIME = "luxxle_vpn_session_end_time";
    private static final String PREF_LINK_SUBSCRIPTION_DIALOG = "link_subscription_dialog";

    public static boolean isLinkSubscriptionDialogShown() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(PREF_LINK_SUBSCRIPTION_DIALOG, false);
    }

    public static void setLinkSubscriptionDialogShown(boolean newValue) {
        ChromeSharedPreferences.getInstance().writeBoolean(PREF_LINK_SUBSCRIPTION_DIALOG, newValue);
    }

    public static boolean shouldShowCallout() {
        return ChromeSharedPreferences.getInstance().readBoolean(PREF_LUXXLE_VPN_CALLOUT, true);
    }

    public static void setCallout(boolean newValue) {
        ChromeSharedPreferences.getInstance().writeBoolean(PREF_LUXXLE_VPN_CALLOUT, newValue);
    }

    public static boolean isSubscriptionPurchase() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(PREF_LUXXLE_VPN_SUBSCRIPTION_PURCHASE, false);
    }

    public static void setSubscriptionPurchase(boolean newValue) {
        ChromeSharedPreferences.getInstance()
                .writeBoolean(PREF_LUXXLE_VPN_SUBSCRIPTION_PURCHASE, newValue);
    }

    public static void setPaymentState(int newValue) {
        ChromeSharedPreferences.getInstance().writeInt(PREF_LUXXLE_VPN_PAYMENT_STATE, newValue);
    }

    public static boolean isTrialSubscription() {
        return ChromeSharedPreferences.getInstance().readInt(PREF_LUXXLE_VPN_PAYMENT_STATE, 0) == 2;
    }

    public static boolean isResetConfiguration() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(PREF_LUXXLE_VPN_RESET_CONFIGURATION, false);
    }

    public static void setResetConfiguration(boolean newValue) {
        ChromeSharedPreferences.getInstance()
                .writeBoolean(PREF_LUXXLE_VPN_RESET_CONFIGURATION, newValue);
    }

    public static boolean isAutomaticServerSelection() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(VPN_AUTOMATIC_SERVER_SELECTION, false);
    }

    public static void setAutomaticServerSelection(boolean newValue) {
        ChromeSharedPreferences.getInstance()
                .writeBoolean(VPN_AUTOMATIC_SERVER_SELECTION, newValue);
    }

    public static void setHostname(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_HOSTNAME, value);
    }

    public static String getHostname() {
        return ChromeSharedPreferences.getInstance().readString(PREF_LUXXLE_VPN_HOSTNAME, "");
    }

    public static void setHostnameDisplay(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_HOSTNAME_DISPLAY, value);
    }

    public static String getHostnameDisplay() {
        return ChromeSharedPreferences.getInstance()
                .readString(PREF_LUXXLE_VPN_HOSTNAME_DISPLAY, "");
    }

    public static void setPurchaseToken(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_PURCHASE_TOKEN, value);
        UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                .setString(LuxxlePref.LUXXLE_VPN_PURCHASE_TOKEN_ANDROID, value);
        UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                .setString(
                        LuxxlePref.LUXXLE_VPN_PACKAGE_ANDROID,
                        ContextUtils.getApplicationContext().getPackageName());
    }

    public static String getPurchaseToken() {
        return ChromeSharedPreferences.getInstance().readString(PREF_LUXXLE_VPN_PURCHASE_TOKEN, "");
    }

    public static void setProductId(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_PRODUCT_ID, value);
        UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                .setString(LuxxlePref.LUXXLE_VPN_PRODUCT_ID_ANDROID, value);
    }

    public static String getProductId() {
        return ChromeSharedPreferences.getInstance().readString(PREF_LUXXLE_VPN_PRODUCT_ID, "");
    }

    public static void setPurchaseExpiry(Long value) {
        ChromeSharedPreferences.getInstance().writeLong(PREF_LUXXLE_VPN_PURCHASE_EXPIRY, value);
    }

    public static Long getPurchaseExpiry() {
        return ChromeSharedPreferences.getInstance().readLong(PREF_LUXXLE_VPN_PURCHASE_EXPIRY, 0);
    }

    public static String getRegionName() {
        return ChromeSharedPreferences.getInstance()
                .readString(PREF_LUXXLE_VPN_SERVER_CHANGE_LOCATION, PREF_LUXXLE_VPN_AUTOMATIC);
    }

    private static void setRegionName(String newValue) {
        ChromeSharedPreferences.getInstance()
                .writeString(PREF_LUXXLE_VPN_SERVER_CHANGE_LOCATION, newValue);
    }

    public static String getRegionCountry() {
        String code =
                ChromeSharedPreferences.getInstance().readString(PREF_LUXXLE_VPN_SERVER_COUNTRY, "");
        return code;
    }

    public static void setRegionCountry(String newValue) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_SERVER_COUNTRY, newValue);
    }

    public static String getRegionContinent() {
        String code =
                ChromeSharedPreferences.getInstance()
                        .readString(PREF_LUXXLE_VPN_SERVER_CONTINENT, "");
        return code;
    }

    public static void setRegionContinent(String newValue) {
        ChromeSharedPreferences.getInstance()
                .writeString(PREF_LUXXLE_VPN_SERVER_CONTINENT, newValue);
    }

    public static String getRegionIsoCode() {
        String code =
                ChromeSharedPreferences.getInstance()
                        .readString(PREF_LUXXLE_VPN_SERVER_ISO_CODE, "");
        return code;
    }

    public static void setRegionIsoCode(String newValue) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_SERVER_ISO_CODE, newValue);
    }

    public static String getRegionNamePretty() {
        return ChromeSharedPreferences.getInstance()
                .readString(PREF_LUXXLE_VPN_SERVER_NAME_PRETTY, "");
    }

    public static void setRegionNamePretty(String newValue) {
        ChromeSharedPreferences.getInstance()
                .writeString(PREF_LUXXLE_VPN_SERVER_NAME_PRETTY, newValue);
    }

    public static String getRegionPrecision() {
        return ChromeSharedPreferences.getInstance()
                .readString(
                        PREF_LUXXLE_VPN_SERVER_PRECISION,
                        LuxxleVpnConstants.REGION_PRECISION_COUNTRY);
    }

    public static void setRegionPrecision(String newValue) {
        ChromeSharedPreferences.getInstance()
                .writeString(PREF_LUXXLE_VPN_SERVER_PRECISION, newValue);
    }

    public static void setApiAuthToken(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_API_AUTH_TOKEN, value);
    }

    public static String getApiAuthToken() {
        return ChromeSharedPreferences.getInstance().readString(PREF_LUXXLE_VPN_API_AUTH_TOKEN, "");
    }

    public static void setSubscriberCredential(String value) {
        ChromeSharedPreferences.getInstance()
                .writeString(PREF_LUXXLE_VPN_SUBSCRIBER_CREDENTIAL, value);
    }

    public static String getSubscriberCredential() {
        return ChromeSharedPreferences.getInstance()
                .readString(PREF_LUXXLE_VPN_SUBSCRIBER_CREDENTIAL, "");
    }

    public static void setClientId(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_CLIENT_ID, value);
    }

    public static String getClientId() {
        return ChromeSharedPreferences.getInstance().readString(PREF_LUXXLE_VPN_CLIENT_ID, "");
    }

    public static void setIpAddress(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_IP_ADDRESS, value);
    }

    public static String getIpAddress() {
        return ChromeSharedPreferences.getInstance().readString(PREF_LUXXLE_VPN_IP_ADDRESS, "");
    }

    public static void setServerPublicKey(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_SERVER_PUBLIC_KEY, value);
    }

    public static String getServerPublicKey() {
        return ChromeSharedPreferences.getInstance()
                .readString(PREF_LUXXLE_VPN_SERVER_PUBLIC_KEY, "");
    }

    public static void setClientPrivateKey(String value) {
        ChromeSharedPreferences.getInstance().writeString(PREF_LUXXLE_VPN_CLIENT_PRIVATE_KEY, value);
    }

    public static String getClientPrivateKey() {
        return ChromeSharedPreferences.getInstance()
                .readString(PREF_LUXXLE_VPN_CLIENT_PRIVATE_KEY, "");
    }

    public static void setPrefModel(LuxxleVpnPrefModel luxxleVpnPrefModel) {
        setHostname(luxxleVpnPrefModel.getHostname());
        setHostnameDisplay(luxxleVpnPrefModel.getHostnameDisplay());
        setRegionCountry(luxxleVpnPrefModel.getServerRegion().getCountry());
        setRegionContinent(luxxleVpnPrefModel.getServerRegion().getContinent());
        setRegionIsoCode(luxxleVpnPrefModel.getServerRegion().getCountryIsoCode());
        setRegionName(luxxleVpnPrefModel.getServerRegion().getRegionName());
        setRegionNamePretty(luxxleVpnPrefModel.getServerRegion().getRegionNamePretty());
        setRegionPrecision(luxxleVpnPrefModel.getServerRegion().getRegionPrecision());
        setAutomaticServerSelection(luxxleVpnPrefModel.getServerRegion().isAutoSelected());
        setPurchaseToken(luxxleVpnPrefModel.getPurchaseToken());
        setProductId(luxxleVpnPrefModel.getProductId());
        setSubscriberCredential(luxxleVpnPrefModel.getSubscriberCredential());
        setClientId(luxxleVpnPrefModel.getClientId());
        setApiAuthToken(luxxleVpnPrefModel.getApiAuthToken());
        setResetConfiguration(false);
    }

    public static Set<String> getExcludedPackages() {
        return ChromeSharedPreferences.getInstance()
                .readStringSet(PREF_EXCLUDED_PACKAGES, Collections.emptySet());
    }

    public static void setExcludedPackages(Set<String> packages) {
        ChromeSharedPreferences.getInstance().writeStringSet(PREF_EXCLUDED_PACKAGES, packages);
    }

    public static void setSessionEndTimeMs(long timeMs) {
        ChromeSharedPreferences.getInstance().writeLong(PREF_SESSION_END_TIME, timeMs);
    }

    public static long getSessionEndTimeMs() {
        return ChromeSharedPreferences.getInstance().readLong(PREF_SESSION_END_TIME, -1);
    }

    public static void setSessionStartTimeMs(long timeMs) {
        ChromeSharedPreferences.getInstance().writeLong(PREF_SESSION_START_TIME, timeMs);
    }

    public static long getSessionStartTimeMs() {
        return ChromeSharedPreferences.getInstance().readLong(PREF_SESSION_START_TIME, -1);
    }
}
