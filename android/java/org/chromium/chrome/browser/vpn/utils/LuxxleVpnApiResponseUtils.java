/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.vpn.utils;

import android.app.Activity;
import android.util.Pair;

import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import org.chromium.luxxle_vpn.mojom.LuxxleVpnConstants;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.billing.InAppPurchaseWrapper;
import org.chromium.chrome.browser.billing.PurchaseModel;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.chrome.browser.vpn.LuxxleVpnNativeWorker;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnPrefModel;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnServerRegion;

import java.util.TimeZone;

public class LuxxleVpnApiResponseUtils {
    public static void queryPurchaseFailed(Activity activity) {
        LuxxleVpnPrefUtils.setProductId("");
        LuxxleVpnPrefUtils.setPurchaseExpiry(0L);
        LuxxleVpnPrefUtils.setSubscriptionPurchase(false);
        LuxxleVpnPrefUtils.setPaymentState(0);
        if (LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(activity)) {
            LuxxleVpnProfileUtils.getInstance().stopVpn(activity);
        }
        LuxxleVpnUtils.showToast(
                activity.getResources().getString(R.string.purchase_token_verification_failed));
        LuxxleVpnUtils.dismissProgressDialog();
    }

    public static void handleOnGetSubscriberCredential(Activity activity, boolean isSuccess) {
        if (isSuccess) {
            if (!LuxxleVpnNativeWorker.getInstance().isPurchasedUser()) {
                MutableLiveData<PurchaseModel> _activePurchases = new MutableLiveData();
                LiveData<PurchaseModel> activePurchases = _activePurchases;
                InAppPurchaseWrapper.getInstance()
                        .queryPurchases(
                                _activePurchases, InAppPurchaseWrapper.SubscriptionProduct.VPN);
                LiveDataUtil.observeOnce(activePurchases, activePurchaseModel -> {
                    InAppPurchaseWrapper.getInstance().processPurchases(
                            activity, activePurchaseModel.getPurchase());
                });
            }
            LuxxleVpnNativeWorker.getInstance().getTimezonesForRegions();
        } else {
            LuxxleVpnUtils.showToast(
                    activity.getResources().getString(R.string.vpn_profile_creation_failed));
            LuxxleVpnUtils.dismissProgressDialog();
        }
    }

    public static void handleOnGetTimezonesForRegions(Activity activity,
            LuxxleVpnPrefModel luxxleVpnPrefModel, String jsonTimezones, boolean isSuccess) {
        if (isSuccess) {
            LuxxleVpnServerRegion luxxleVpnServerRegion =
                    LuxxleVpnUtils.getServerRegionForTimeZone(
                            jsonTimezones, TimeZone.getDefault().getID());
            if (luxxleVpnServerRegion == null || luxxleVpnServerRegion.getRegionName().isEmpty()) {
                LuxxleVpnUtils.showToast(
                        String.format(
                                activity.getResources()
                                        .getString(R.string.couldnt_get_matching_timezone),
                                TimeZone.getDefault().getID()));
                LuxxleVpnUtils.dismissProgressDialog();
                return;
            }
            String regionFromTimeZone = luxxleVpnServerRegion.getRegionName();
            String regionForHostName = regionFromTimeZone;
            String regionPrecision = luxxleVpnServerRegion.getRegionPrecision();

            // Determine the region for host name and precision
            if (LuxxleVpnUtils.selectedServerRegion != null) {
                if (!LuxxleVpnUtils.selectedServerRegion
                        .getRegionName()
                        .equals(LuxxleVpnPrefUtils.PREF_LUXXLE_VPN_AUTOMATIC)) {
                    regionForHostName = LuxxleVpnUtils.selectedServerRegion.getRegionName();
                    luxxleVpnServerRegion = LuxxleVpnUtils.selectedServerRegion;
                    regionPrecision = luxxleVpnServerRegion.getRegionPrecision();
                } else {
                    regionPrecision = LuxxleVpnConstants.REGION_PRECISION_DEFAULT;
                }
            } else {
                String serverRegion = LuxxleVpnPrefUtils.getRegionName();
                if (serverRegion.equals(LuxxleVpnPrefUtils.PREF_LUXXLE_VPN_AUTOMATIC)) {
                    regionPrecision = LuxxleVpnConstants.REGION_PRECISION_DEFAULT;
                } else {
                    regionForHostName = serverRegion;
                }
            }
            LuxxleVpnNativeWorker.getInstance()
                    .getHostnamesForRegion(regionForHostName, regionPrecision);
            luxxleVpnPrefModel.setServerRegion(luxxleVpnServerRegion);
        } else {
            LuxxleVpnUtils.showToast(
                    activity.getResources().getString(R.string.vpn_profile_creation_failed));
            LuxxleVpnUtils.dismissProgressDialog();
        }
    }

    public static Pair<String, String> handleOnGetHostnamesForRegion(Activity activity,
            LuxxleVpnPrefModel luxxleVpnPrefModel, String jsonHostNames, boolean isSuccess) {
        Pair<String, String> host = new Pair<String, String>("", "");
        if (isSuccess && luxxleVpnPrefModel != null) {
            host = LuxxleVpnUtils.getHostnameForRegion(jsonHostNames);
            LuxxleVpnNativeWorker.getInstance().getWireguardProfileCredentials(
                    luxxleVpnPrefModel.getSubscriberCredential(),
                    luxxleVpnPrefModel.getClientPublicKey(), host.first);
        } else {
            LuxxleVpnUtils.showToast(
                    activity.getResources().getString(R.string.vpn_profile_creation_failed));
            LuxxleVpnUtils.dismissProgressDialog();
        }
        return host;
    }
}
