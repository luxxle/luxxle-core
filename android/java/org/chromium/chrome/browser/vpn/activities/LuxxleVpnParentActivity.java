/**
 * Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn.activities;

import android.content.Intent;
import android.os.Handler;
import android.util.Pair;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;

import com.wireguard.android.backend.GoBackend;
import com.wireguard.crypto.KeyPair;

import org.chromium.base.Log;
import org.chromium.base.supplier.OneshotSupplier;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.billing.InAppPurchaseWrapper;
import org.chromium.chrome.browser.billing.PurchaseModel;
import org.chromium.chrome.browser.init.ActivityProfileProvider;
import org.chromium.chrome.browser.init.AsyncInitializationActivity;
import org.chromium.chrome.browser.profiles.ProfileProvider;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.chrome.browser.util.TabUtils;
import org.chromium.chrome.browser.vpn.LuxxleVpnNativeWorker;
import org.chromium.chrome.browser.vpn.LuxxleVpnObserver;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnPrefModel;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnWireguardProfileCredentials;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnApiResponseUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnProfileUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnUtils;
import org.chromium.chrome.browser.vpn.wireguard.WireguardConfigUtils;

public abstract class LuxxleVpnParentActivity
        extends AsyncInitializationActivity implements LuxxleVpnObserver {
    private static final String TAG = "LuxxleVPN";
    public boolean mIsVerification;
    protected LuxxleVpnPrefModel mLuxxleVpnPrefModel;
    protected boolean mIsServerLocationChanged;

    private static final int INVALIDATE_CREDENTIAL_TIMER_COUNT = 5000;

    abstract void showRestoreMenu(boolean shouldShowRestore);
    abstract void updateProfileView();

    // Pass @{code ActivityResultRegistry} reference explicitly to avoid crash
    // https://github.com/luxxle/luxxle-browser/issues/31882
    ActivityResultLauncher<Intent> mIntentActivityResultLauncher =
            registerForActivityResult(
                    new ActivityResultContracts.StartActivityForResult(),
                    getActivityResultRegistry(),
                    result -> {
                        LuxxleVpnUtils.dismissProgressDialog();
                        if (result.getResultCode() == RESULT_OK) {
                            LuxxleVpnProfileUtils.getInstance()
                                    .startVpn(LuxxleVpnParentActivity.this);
                            LuxxleVpnUtils.showVpnConfirmDialog(this);
                        } else if (result.getResultCode() == RESULT_CANCELED) {
                            if (LuxxleVpnProfileUtils.getInstance().isVPNRunning(this)) {
                                LuxxleVpnUtils.showVpnAlwaysOnErrorDialog(this);
                            } else {
                                updateProfileView();
                            }
                            LuxxleVpnUtils.showToast(
                                    getResources().getString(R.string.permission_was_cancelled));
                        }
                    });

    @Override
    public void onResumeWithNative() {
        super.onResumeWithNative();
        LuxxleVpnNativeWorker.getInstance().addObserver(this);
    }

    @Override
    public void onPauseWithNative() {
        LuxxleVpnNativeWorker.getInstance().removeObserver(this);
        super.onPauseWithNative();
    }

    @Override
    public void finishNativeInitialization() {
        super.finishNativeInitialization();
    }

    @Override
    protected void onDestroy() {
        LuxxleVpnUtils.dismissProgressDialog();
        super.onDestroy();
    }

    protected void verifySubscription() {
        mLuxxleVpnPrefModel = new LuxxleVpnPrefModel();
        MutableLiveData<PurchaseModel> _activePurchases = new MutableLiveData();
        LiveData<PurchaseModel> activePurchases = _activePurchases;
        InAppPurchaseWrapper.getInstance()
                .queryPurchases(_activePurchases, InAppPurchaseWrapper.SubscriptionProduct.VPN);
        LiveDataUtil.observeOnce(
                activePurchases,
                activePurchaseModel -> {
                    if (activePurchaseModel != null) {
                        mLuxxleVpnPrefModel.setPurchaseToken(activePurchaseModel.getPurchaseToken());
                        mLuxxleVpnPrefModel.setProductId(activePurchaseModel.getProductId());
                        LuxxleVpnNativeWorker.getInstance()
                                .verifyPurchaseToken(
                                        mLuxxleVpnPrefModel.getPurchaseToken(),
                                        mLuxxleVpnPrefModel.getProductId(),
                                        LuxxleVpnUtils.SUBSCRIPTION_PARAM_TEXT,
                                        getPackageName());
                    } else {
                        if (!mIsVerification) {
                            LuxxleVpnApiResponseUtils.queryPurchaseFailed(
                                    LuxxleVpnParentActivity.this);
                            mIsServerLocationChanged = false;
                        } else {
                            showRestoreMenu(false);
                        }
                        LuxxleVpnUtils.dismissProgressDialog();
                    }
                });
    }

    @Override
    public void onVerifyPurchaseToken(
            String jsonResponse, String purchaseToken, String productId, boolean isSuccess) {
        if (isSuccess && mLuxxleVpnPrefModel != null) {
            Long purchaseExpiry = LuxxleVpnUtils.getPurchaseExpiryDate(jsonResponse);
            int paymentState = LuxxleVpnUtils.getPaymentState(jsonResponse);
            if (purchaseExpiry > 0 && purchaseExpiry >= System.currentTimeMillis()) {
                LuxxleVpnPrefUtils.setPurchaseToken(purchaseToken);
                LuxxleVpnPrefUtils.setProductId(productId);
                LuxxleVpnPrefUtils.setPurchaseExpiry(purchaseExpiry);
                LuxxleVpnPrefUtils.setSubscriptionPurchase(true);
                LuxxleVpnPrefUtils.setPaymentState(paymentState);
                if (!mIsVerification || LuxxleVpnPrefUtils.isResetConfiguration()) {
                    LuxxleVpnNativeWorker.getInstance()
                            .getSubscriberCredential(
                                    LuxxleVpnUtils.SUBSCRIPTION_PARAM_TEXT,
                                    mLuxxleVpnPrefModel.getProductId(),
                                    LuxxleVpnUtils.IAP_ANDROID_PARAM_TEXT,
                                    mLuxxleVpnPrefModel.getPurchaseToken(),
                                    getPackageName());
                } else {
                    mIsVerification = false;
                    showRestoreMenu(true);
                    LuxxleVpnUtils.showToast(getResources().getString(R.string.already_subscribed));
                    LuxxleVpnUtils.dismissProgressDialog();
                }
            } else {
                LuxxleVpnApiResponseUtils.queryPurchaseFailed(LuxxleVpnParentActivity.this);
                mIsServerLocationChanged = false;
                if (mIsVerification) {
                    mIsVerification = false;
                    showRestoreMenu(false);
                    LuxxleVpnUtils.dismissProgressDialog();
                } else {
                    LuxxleVpnUtils.openLuxxleVpnPlansActivity(LuxxleVpnParentActivity.this);
                }
            }
        } else {
            LuxxleVpnUtils.dismissProgressDialog();
        }
    };

    @Override
    public void onGetSubscriberCredential(String subscriberCredential, boolean isSuccess) {
        mLuxxleVpnPrefModel.setSubscriberCredential(subscriberCredential);
        LuxxleVpnApiResponseUtils.handleOnGetSubscriberCredential(
                LuxxleVpnParentActivity.this, isSuccess);
    };

    @Override
    public void onGetTimezonesForRegions(String jsonTimezones, boolean isSuccess) {
        LuxxleVpnApiResponseUtils.handleOnGetTimezonesForRegions(
                LuxxleVpnParentActivity.this, mLuxxleVpnPrefModel, jsonTimezones, isSuccess);
    }

    @Override
    public void onGetHostnamesForRegion(String jsonHostNames, boolean isSuccess) {
        KeyPair keyPair = new KeyPair();
        mLuxxleVpnPrefModel.setClientPrivateKey(keyPair.getPrivateKey().toBase64());
        mLuxxleVpnPrefModel.setClientPublicKey(keyPair.getPublicKey().toBase64());
        Pair<String, String> host = LuxxleVpnApiResponseUtils.handleOnGetHostnamesForRegion(
                LuxxleVpnParentActivity.this, mLuxxleVpnPrefModel, jsonHostNames, isSuccess);
        mLuxxleVpnPrefModel.setHostname(host.first);
        mLuxxleVpnPrefModel.setHostnameDisplay(host.second);
    }

    @Override
    public void onGetWireguardProfileCredentials(
            String jsonWireguardProfileCredentials, boolean isSuccess) {
        if (isSuccess && mLuxxleVpnPrefModel != null) {
            LuxxleVpnWireguardProfileCredentials luxxleVpnWireguardProfileCredentials =
                    LuxxleVpnUtils.getWireguardProfileCredentials(jsonWireguardProfileCredentials);

            int timerCount = 0;
            if (mIsServerLocationChanged) {
                timerCount = INVALIDATE_CREDENTIAL_TIMER_COUNT;
                mIsServerLocationChanged = false;
                try {
                    LuxxleVpnNativeWorker.getInstance()
                            .invalidateCredentials(
                                    LuxxleVpnPrefUtils.getHostname(),
                                    LuxxleVpnPrefUtils.getClientId(),
                                    LuxxleVpnPrefUtils.getSubscriberCredential(),
                                    LuxxleVpnPrefUtils.getApiAuthToken());
                } catch (Exception ex) {
                    Log.e(TAG, ex.getMessage());
                }
            }

            new Handler()
                    .postDelayed(
                            () -> {
                                checkForVpn(luxxleVpnWireguardProfileCredentials);
                            },
                            timerCount);
        } else {
            LuxxleVpnUtils.showToast(getResources().getString(R.string.vpn_profile_creation_failed));
            LuxxleVpnUtils.dismissProgressDialog();
        }
    }

    public void changeServerRegion() {
        mIsServerLocationChanged = true;
        LuxxleVpnUtils.showProgressDialog(
                LuxxleVpnParentActivity.this, getResources().getString(R.string.vpn_connect_text));
        if (LuxxleVpnNativeWorker.getInstance().isPurchasedUser()) {
            mLuxxleVpnPrefModel = new LuxxleVpnPrefModel();
            LuxxleVpnNativeWorker.getInstance().getSubscriberCredentialV12();
        } else {
            verifySubscription();
        }
    }

    private void checkForVpn(
            LuxxleVpnWireguardProfileCredentials luxxleVpnWireguardProfileCredentials) {
        new Thread() {
            @Override
            public void run() {
                try {
                    if (LuxxleVpnProfileUtils.getInstance()
                            .isLuxxleVPNConnected(LuxxleVpnParentActivity.this)) {
                        LuxxleVpnProfileUtils.getInstance().stopVpn(LuxxleVpnParentActivity.this);
                    }
                    if (WireguardConfigUtils.isConfigExist(getApplicationContext())) {
                        WireguardConfigUtils.deleteConfig(getApplicationContext());
                    }
                    WireguardConfigUtils.createConfig(
                            getApplicationContext(),
                            luxxleVpnWireguardProfileCredentials.getMappedIpv4Address(),
                            mLuxxleVpnPrefModel.getHostname(),
                            mLuxxleVpnPrefModel.getClientPrivateKey(),
                            luxxleVpnWireguardProfileCredentials.getServerPublicKey());

                    mLuxxleVpnPrefModel.setClientId(
                            luxxleVpnWireguardProfileCredentials.getClientId());
                    mLuxxleVpnPrefModel.setApiAuthToken(
                            luxxleVpnWireguardProfileCredentials.getApiAuthToken());
                    LuxxleVpnPrefUtils.setPrefModel(mLuxxleVpnPrefModel);

                    LuxxleVpnUtils.dismissProgressDialog();
                    Intent intent = GoBackend.VpnService.prepare(LuxxleVpnParentActivity.this);
                    if (intent != null) {
                        mIntentActivityResultLauncher.launch(intent);
                        return;
                    }
                    LuxxleVpnProfileUtils.getInstance().startVpn(LuxxleVpnParentActivity.this);
                    TabUtils.bringChromeTabbedActivityToTheTop(LuxxleVpnParentActivity.this);
                } catch (Exception e) {
                    LuxxleVpnUtils.dismissProgressDialog();
                    Log.e(TAG, e.getMessage());
                }
            }
        }.start();
    }

    @Override
    protected OneshotSupplier<ProfileProvider> createProfileProvider() {
        return new ActivityProfileProvider(getLifecycleDispatcher());
    }
}
