/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.luxxle_leo;

import org.chromium.base.LuxxleFeatureList;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.settings.LuxxleLeoPreferences;
import org.chromium.components.prefs.PrefService;
import org.chromium.components.user_prefs.UserPrefs;

public class LuxxleLeoPrefUtils {
    private static final String TAG = "LuxxleLeoPrefUtils";

    private static Profile getProfile() {
        Profile profile = null;
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            profile = activity.getCurrentProfile();
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "get LuxxleActivity exception", e);
        }
        if (profile == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.getProfile profile is null");
        }

        return profile;
    }

    public static void setIsSubscriptionActive(boolean value) {
        Profile profileToUse = LuxxleLeoPrefUtils.getProfile();
        if (profileToUse == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.setIsSubscriptionActive profile is null");
            return;
        }
        UserPrefs.get(profileToUse)
                .setBoolean(LuxxlePref.LUXXLE_CHAT_SUBSCRIPTION_ACTIVE_ANDROID, value);
    }

    public static boolean getIsSubscriptionActive(Profile profile) {
        Profile profileToUse = profile == null ? LuxxleLeoPrefUtils.getProfile() : profile;
        if (profileToUse == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.getIsSubscriptionActive profile is null");
            return false;
        }
        return UserPrefs.get(profileToUse)
                .getBoolean(LuxxlePref.LUXXLE_CHAT_SUBSCRIPTION_ACTIVE_ANDROID);
    }

    public static void setChatPurchaseToken(String token) {
        Profile profileToUse = LuxxleLeoPrefUtils.getProfile();
        if (profileToUse == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.setChatPurchaseToken profile is null");
            return;
        }
        PrefService prefService = UserPrefs.get(profileToUse);
        if (prefService.getString(LuxxlePref.LUXXLE_CHAT_PURCHASE_TOKEN_ANDROID).equals(token)
                && !prefService.getString(LuxxlePref.LUXXLE_CHAT_ORDER_ID_ANDROID).isEmpty()) {
            return;
        }
        // It means we don't have a Play Store subscription anymore or
        // we have a new one.
        LuxxleLeoPrefUtils.resetSubscriptionLinkedStatus(profileToUse);
        prefService.setString(LuxxlePref.LUXXLE_CHAT_ORDER_ID_ANDROID, "");
        prefService.setString(LuxxlePref.LUXXLE_CHAT_PURCHASE_TOKEN_ANDROID, token);
        if (!token.isEmpty()) {
            createFetchOrder(profileToUse);
        }
    }

    public static boolean getIsHistoryEnabled() {
        Profile profileToUse = LuxxleLeoPrefUtils.getProfile();
        if (profileToUse == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.getIsHistoryEnabled profile is null");
            return false;
        }
        return UserPrefs.get(profileToUse).getBoolean(LuxxlePref.LUXXLE_CHAT_STORAGE_ENABLED);
    }

    public static void setIsHistoryEnabled(boolean isEnabled) {
        Profile profileToUse = LuxxleLeoPrefUtils.getProfile();
        if (profileToUse == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.getIsHistoryEnabled profile is null");
            return;
        }
        UserPrefs.get(profileToUse).setBoolean(LuxxlePref.LUXXLE_CHAT_STORAGE_ENABLED, isEnabled);
    }

    private static void createFetchOrder(Profile profileToUse) {
        LuxxleLeoMojomHelper.getInstance(profileToUse)
                .createOrderId(
                        orderId -> {
                            fetchOrder(profileToUse, orderId);
                        });
    }

    private static void fetchOrder(Profile profileToUse, String orderId) {
        LuxxleLeoMojomHelper.getInstance(profileToUse)
                .fetchOrderCredentials(
                        orderId,
                        response -> {
                            if (!response.isEmpty()) {
                                return;
                            }
                            UserPrefs.get(profileToUse)
                                    .setString(LuxxlePref.LUXXLE_CHAT_ORDER_ID_ANDROID, orderId);
                        });
    }

    public static void setChatPackageName() {
        Profile profileToUse = LuxxleLeoPrefUtils.getProfile();
        if (profileToUse == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.setChatPackageName profile is null");
            return;
        }
        UserPrefs.get(profileToUse)
                .setString(
                        LuxxlePref.LUXXLE_CHAT_PACKAGE_NAME_ANDROID,
                        ContextUtils.getApplicationContext().getPackageName());
    }

    public static void setChatProductId(String productId) {
        Profile profileToUse = LuxxleLeoPrefUtils.getProfile();
        if (profileToUse == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.setChatProductId profile is null");
            return;
        }
        UserPrefs.get(profileToUse).setString(LuxxlePref.LUXXLE_CHAT_PRODUCT_ID_ANDROID, productId);
    }

    public static boolean isLeoEnabled() {
        if (!ChromeFeatureList.isEnabled(LuxxleFeatureList.AI_CHAT)) {
            return false;
        }

        return true;
    }

    public static boolean isSubscriptionLinked() {
        Profile profileToUse = LuxxleLeoPrefUtils.getProfile();
        if (profileToUse == null) {
            Log.e(TAG, "LuxxleLeoPrefUtils.isSubscriptionLinked profile is null");
            return false;
        }

        return UserPrefs.get(profileToUse)
                        .getInteger(LuxxlePref.LUXXLE_CHAT_SUBSCRIPTION_LINK_STATUS_ANDROID)
                != 0;
    }

    private static void resetSubscriptionLinkedStatus(Profile profile) {
        UserPrefs.get(profile).setInteger(LuxxlePref.LUXXLE_CHAT_SUBSCRIPTION_LINK_STATUS_ANDROID, 0);
    }

    public static boolean shouldShowLeoQuickSearchEngine() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(LuxxleLeoPreferences.PREF_LEO_QUICK_SEARCH_ENGINE, true);
    }
}
