/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.billing;

import org.chromium.chrome.browser.billing.InAppPurchaseWrapper.SubscriptionProduct;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;

import java.util.Locale;

public class LinkSubscriptionUtils {
    private static final String LUXXLE_ACCOUNT_URL_STAGING = "account.luxxlesoftware.com";
    private static final String LUXXLE_ACCOUNT_URL = "account.luxxle.com";

    public static final String PREF_LINK_SUBSCRIPTION_ON_STAGING = "link_subscription_on_staging";

    public static String getLuxxleAccountLinkUrl(SubscriptionProduct subscriptionProduct) {
        String luxxleAccountUrl =
                isLinkSubscriptionOnStaging() ? LUXXLE_ACCOUNT_URL_STAGING : LUXXLE_ACCOUNT_URL;
        String linkType =
                (SubscriptionProduct.LEO == subscriptionProduct) ? "link-order" : "connect-receipt";
        String baseUrl = "https://%s?intent=%s&product=%s";
        return String.format(
                baseUrl,
                luxxleAccountUrl,
                linkType,
                subscriptionProduct.name().toLowerCase(Locale.ROOT));
    }

    public static String getLuxxleAccountRecoverUrl(SubscriptionProduct subscriptionProduct) {
        String luxxleAccountUrl =
                isLinkSubscriptionOnStaging() ? LUXXLE_ACCOUNT_URL_STAGING : LUXXLE_ACCOUNT_URL;
        String baseUrl = "https://%s?intent=recover&product=%s&ux=mobile";
        return String.format(
                baseUrl, luxxleAccountUrl, subscriptionProduct.name().toLowerCase(Locale.ROOT));
    }

    public static boolean isLinkSubscriptionOnStaging() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(PREF_LINK_SUBSCRIPTION_ON_STAGING, false);
    }
}
