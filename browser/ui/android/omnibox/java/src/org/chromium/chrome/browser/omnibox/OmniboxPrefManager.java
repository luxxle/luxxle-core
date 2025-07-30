/**
 * Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.omnibox;

import android.content.SharedPreferences;

import org.chromium.base.ContextUtils;

import java.util.Calendar;
import java.util.Date;

public class OmniboxPrefManager {
    private static final String LUXXLE_SEARCH_PROMO_BANNER_EXPIRED_DATE =
            "luxxle_search_promo_banner_expired_date";
    private static final String LUXXLE_SEARCH_PROMO_BANNER_MAYBE_LATER =
            "luxxle_search_promo_banner_maybe_later";
    private static final String LUXXLE_SEARCH_PROMO_BANNER_DISMISSED =
            "luxxle_search_promo_banner_dismissed";

    private static OmniboxPrefManager sInstance;
    private final SharedPreferences mSharedPreferences;

    private boolean isLuxxleSearchPromoBannerDismissedCurrentSession;

    private OmniboxPrefManager() {
        mSharedPreferences = ContextUtils.getAppSharedPreferences();
    }

    /**
     * Returns the singleton instance of OmniboxPrefManager, creating it if needed.
     */
    public static OmniboxPrefManager getInstance() {
        if (sInstance == null) {
            sInstance = new OmniboxPrefManager();
        }
        return sInstance;
    }

    public long getLuxxleSearchPromoBannerExpiredDate() {
        return mSharedPreferences.getLong(LUXXLE_SEARCH_PROMO_BANNER_EXPIRED_DATE, 0);
    }

    public void setLuxxleSearchPromoBannerExpiredDate() {
        Calendar calender = Calendar.getInstance();
        calender.setTime(new Date());
        calender.add(Calendar.DATE, 14);

        SharedPreferences.Editor sharedPreferencesEditor = mSharedPreferences.edit();
        sharedPreferencesEditor.putLong(
                LUXXLE_SEARCH_PROMO_BANNER_EXPIRED_DATE, calender.getTimeInMillis());
        sharedPreferencesEditor.apply();
    }

    public boolean isLuxxleSearchPromoBannerMaybeLater() {
        return mSharedPreferences.getBoolean(LUXXLE_SEARCH_PROMO_BANNER_MAYBE_LATER, false);
    }

    public void setLuxxleSearchPromoBannerMaybeLater() {
        isLuxxleSearchPromoBannerDismissedCurrentSession = true;

        SharedPreferences.Editor sharedPreferencesEditor = mSharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(LUXXLE_SEARCH_PROMO_BANNER_MAYBE_LATER, true);
        sharedPreferencesEditor.apply();
    }

    public boolean isLuxxleSearchPromoBannerDismissed() {
        return mSharedPreferences.getBoolean(LUXXLE_SEARCH_PROMO_BANNER_DISMISSED, false);
    }

    public void setLuxxleSearchPromoBannerDismissed() {
        SharedPreferences.Editor sharedPreferencesEditor = mSharedPreferences.edit();
        sharedPreferencesEditor.putBoolean(LUXXLE_SEARCH_PROMO_BANNER_DISMISSED, true);
        sharedPreferencesEditor.apply();
    }

    public boolean isLuxxleSearchPromoBannerDismissedCurrentSession() {
        return isLuxxleSearchPromoBannerDismissedCurrentSession;
    }
}
