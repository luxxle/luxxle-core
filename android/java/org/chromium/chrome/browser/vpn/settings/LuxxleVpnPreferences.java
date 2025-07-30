/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.vpn.settings;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Pair;

import androidx.appcompat.app.AlertDialog;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.preference.Preference;
import androidx.preference.PreferenceCategory;

import com.wireguard.android.backend.GoBackend;
import com.wireguard.crypto.KeyPair;

import org.chromium.base.LuxxleFeatureList;
import org.chromium.base.Log;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.InternetConnection;
import org.chromium.chrome.browser.billing.InAppPurchaseWrapper;
import org.chromium.chrome.browser.billing.LinkSubscriptionUtils;
import org.chromium.chrome.browser.billing.PurchaseModel;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.settings.LuxxlePreferenceFragment;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.chrome.browser.util.TabUtils;
import org.chromium.chrome.browser.vpn.LuxxleVpnNativeWorker;
import org.chromium.chrome.browser.vpn.LuxxleVpnObserver;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnPrefModel;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnWireguardProfileCredentials;
import org.chromium.chrome.browser.vpn.timer.TimerDialogFragment;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnApiResponseUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnProfileUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnUtils;
import org.chromium.chrome.browser.vpn.wireguard.WireguardConfigUtils;
import org.chromium.components.browser_ui.settings.ChromeBasePreference;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.ui.widget.Toast;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

public class LuxxleVpnPreferences extends LuxxlePreferenceFragment implements LuxxleVpnObserver {
    private static final String TAG = "LuxxleVPN";
    public static final String PREF_VPN_SWITCH = "vpn_switch";
    public static final String PREF_SUBSCRIPTION_MANAGE = "subscription_manage";
    public static final String PREF_LINK_SUBSCRIPTION = "link_subscription";
    public static final String PREF_SUBSCRIPTION_STATUS = "subscription_status";
    public static final String PREF_SUBSCRIPTION_EXPIRES = "subscription_expires";
    public static final String PREF_SERVER_CHANGE_LOCATION = "server_change_location";
    public static final String PREF_SUPPORT_TECHNICAL = "support_technical";
    public static final String PREF_SUPPORT_VPN = "support_vpn";
    public static final String PREF_SERVER_RESET_CONFIGURATION = "server_reset_configuration";
    private static final String PREF_SPLIT_TUNNELING = "split_tunneling";
    private static final String PREF_AUTO_RECONNECT_VPN = "auto_reconnect_vpn";
    private static final String PREF_LUXXLE_VPN_SUBSCRIPTION_SECTION =
            "luxxle_vpn_subscription_section";

    private static final int INVALIDATE_CREDENTIAL_TIMER_COUNT = 5000;

    private static final String VPN_SUPPORT_PAGE =
            "https://support.luxxle.com/hc/en-us/articles/4410838268429";

    private static final String DATE_FORMAT = "dd/MM/yyyy";

    private ChromeSwitchPreference mVpnSwitch;
    private ChromeBasePreference mSubscriptionStatus;
    private ChromeBasePreference mSubscriptionExpires;
    private ChromeBasePreference mLinkSubscriptionPreference;
    private LuxxleVpnPrefModel mLuxxleVpnPrefModel;
    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        mPageTitle.set(getString(R.string.luxxle_firewall_vpn));
        SettingsUtils.addPreferencesFromResource(this, R.xml.luxxle_vpn_preferences);

        mVpnSwitch = (ChromeSwitchPreference) findPreference(PREF_VPN_SWITCH);
        mVpnSwitch.setChecked(
                LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(getActivity()));
        mVpnSwitch.setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
            @Override
            public boolean onPreferenceClick(Preference preference) {
                if (mVpnSwitch != null) {
                    mVpnSwitch.setChecked(
                            LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(getActivity()));
                }
                if (LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(getActivity())) {
                    TimerDialogFragment timerDialogFragment = new TimerDialogFragment();
                    timerDialogFragment.show(
                            getActivity().getSupportFragmentManager(), TimerDialogFragment.TAG);
                } else {
                    if (LuxxleVpnNativeWorker.getInstance().isPurchasedUser()) {
                        LuxxleVpnPrefUtils.setSubscriptionPurchase(true);
                        if (WireguardConfigUtils.isConfigExist(getActivity())) {
                            LuxxleVpnProfileUtils.getInstance().startVpn(getActivity());
                        } else {
                            LuxxleVpnUtils.openLuxxleVpnProfileActivity(getActivity());
                        }
                    } else {
                        LuxxleVpnUtils.showProgressDialog(
                                getActivity(), getResources().getString(R.string.vpn_connect_text));
                        if (LuxxleVpnPrefUtils.isSubscriptionPurchase()) {
                            verifyPurchase(true);
                        } else {
                            LuxxleVpnUtils.openLuxxleVpnPlansActivity(getActivity());
                            LuxxleVpnUtils.dismissProgressDialog();
                        }
                    }
                }
                return false;
            }
        });

        mSubscriptionStatus = (ChromeBasePreference) findPreference(PREF_SUBSCRIPTION_STATUS);
        mSubscriptionExpires = (ChromeBasePreference) findPreference(PREF_SUBSCRIPTION_EXPIRES);

        findPreference(PREF_SUPPORT_TECHNICAL)
                .setOnPreferenceClickListener(
                        new Preference.OnPreferenceClickListener() {
                            @Override
                            public boolean onPreferenceClick(Preference preference) {
                                LuxxleVpnUtils.openLuxxleVpnSupportActivity(getActivity());
                                return true;
                            }
                        });

        findPreference(PREF_SUPPORT_VPN)
                .setOnPreferenceClickListener(
                        new Preference.OnPreferenceClickListener() {
                            @Override
                            public boolean onPreferenceClick(Preference preference) {
                                CustomTabActivity.showInfoPage(getActivity(), VPN_SUPPORT_PAGE);
                                return true;
                            }
                        });

        findPreference(PREF_SUBSCRIPTION_MANAGE)
                .setOnPreferenceClickListener(
                        new Preference.OnPreferenceClickListener() {
                            @Override
                            public boolean onPreferenceClick(Preference preference) {
                                Intent browserIntent =
                                        new Intent(
                                                Intent.ACTION_VIEW,
                                                Uri.parse(
                                                        InAppPurchaseWrapper
                                                                .MANAGE_SUBSCRIPTION_PAGE));
                                getActivity().startActivity(browserIntent);
                                return true;
                            }
                        });

        findPreference(PREF_SERVER_RESET_CONFIGURATION)
                .setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        showConfirmDialog();
                        return true;
                    }
                });

        findPreference(PREF_SPLIT_TUNNELING)
                .setOnPreferenceClickListener(
                        new Preference.OnPreferenceClickListener() {
                            @Override
                            public boolean onPreferenceClick(Preference preference) {
                                LuxxleVpnUtils.openSplitTunnelActivity(getActivity());
                                return true;
                            }
                        });
        findPreference(PREF_AUTO_RECONNECT_VPN)
                .setOnPreferenceClickListener(
                        new Preference.OnPreferenceClickListener() {
                            @Override
                            public boolean onPreferenceClick(Preference preference) {
                                LuxxleVpnUtils.openAutoReconnectVpnActivity(getActivity());
                                return true;
                            }
                        });
        mLinkSubscriptionPreference = new ChromeBasePreference(getActivity());
        mLinkSubscriptionPreference.setTitle(
                getResources().getString(R.string.link_subscription_title));
        mLinkSubscriptionPreference.setSummary(
                getResources().getString(R.string.link_subscription_text));
        mLinkSubscriptionPreference.setKey(PREF_LINK_SUBSCRIPTION);
        mLinkSubscriptionPreference.setVisible(
                ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_VPN_LINK_SUBSCRIPTION_ANDROID_UI)
                && LuxxleVpnPrefUtils.isSubscriptionPurchase());
        mLinkSubscriptionPreference.setOnPreferenceClickListener(
                new Preference.OnPreferenceClickListener() {
                    @Override
                    public boolean onPreferenceClick(Preference preference) {
                        TabUtils.openURLWithLuxxleActivity(
                                LinkSubscriptionUtils.getLuxxleAccountLinkUrl(
                                        InAppPurchaseWrapper.SubscriptionProduct.VPN));
                        return true;
                    }
                });
        PreferenceCategory preferenceCategory =
                (PreferenceCategory) findPreference(PREF_LUXXLE_VPN_SUBSCRIPTION_SECTION);
        preferenceCategory.addPreference(mLinkSubscriptionPreference);
        preferenceCategory.setVisible(!LuxxleVpnNativeWorker.getInstance().isPurchasedUser());
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
        if (context != null) {
            ConnectivityManager connectivityManager =
                    (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkRequest networkRequest =
                    new NetworkRequest.Builder()
                            .addTransportType(NetworkCapabilities.TRANSPORT_VPN)
                            .removeCapability(NetworkCapabilities.NET_CAPABILITY_NOT_VPN)
                            .build();
            connectivityManager.registerNetworkCallback(networkRequest, mNetworkCallback);
        }
        if (!InternetConnection.isNetworkAvailable(getActivity())) {
            Toast.makeText(getActivity(), R.string.no_internet, Toast.LENGTH_SHORT).show();
            getActivity().finish();
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        if (LuxxleVpnUtils.mUpdateProfileAfterSplitTunnel) {
            LuxxleVpnUtils.mUpdateProfileAfterSplitTunnel = false;
            LuxxleVpnUtils.showProgressDialog(
                    getActivity(), getResources().getString(R.string.updating_vpn_profile));
            LuxxleVpnUtils.updateProfileConfiguration(getActivity());
        } else {
            LuxxleVpnUtils.dismissProgressDialog();
        }
        if (mLinkSubscriptionPreference != null) {
            mLinkSubscriptionPreference.setVisible(
                    ChromeFeatureList.isEnabled(
                            LuxxleFeatureList.LUXXLE_VPN_LINK_SUBSCRIPTION_ANDROID_UI)
                    && LuxxleVpnPrefUtils.isSubscriptionPurchase());
        }
        new Handler().post(() -> updateSummaries());
    }

    private void updateSummary(String preferenceString, String summary) {
        Preference p = findPreference(preferenceString);
        p.setSummary(summary);
    }

    private void updateSummaries() {
        if (getActivity() == null) {
            return;
        }
        if (!LuxxleVpnPrefUtils.getProductId().isEmpty()) {
            String subscriptionStatus =
                    String.format(
                            InAppPurchaseWrapper.getInstance()
                                            .isMonthlySubscription(LuxxleVpnPrefUtils.getProductId())
                                    ? getActivity()
                                            .getResources()
                                            .getString(R.string.monthly_subscription)
                                    : getActivity()
                                            .getResources()
                                            .getString(R.string.yearly_subscription),
                            (LuxxleVpnPrefUtils.isTrialSubscription()
                                    ? getActivity().getResources().getString(R.string.trial)
                                    : ""));
            updateSummary(PREF_SUBSCRIPTION_STATUS, subscriptionStatus);
        }

        if (LuxxleVpnPrefUtils.getPurchaseExpiry() > 0) {
            long expiresInMillis = LuxxleVpnPrefUtils.getPurchaseExpiry();
            SimpleDateFormat formatter = new SimpleDateFormat(DATE_FORMAT, Locale.getDefault());
            updateSummary(PREF_SUBSCRIPTION_EXPIRES, formatter.format(new Date(expiresInMillis)));
        }
        if (mVpnSwitch != null) {
            mVpnSwitch.setChecked(
                    LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(getActivity()));
        }
        new Thread() {
            @Override
            public void run() {
                if (getActivity() != null) {
                    getActivity()
                            .runOnUiThread(
                                    new Runnable() {
                                        @Override
                                        public void run() {
                                            findPreference(PREF_SERVER_CHANGE_LOCATION)
                                                    .setVisible(
                                                            LuxxleVpnPrefUtils
                                                                    .isSubscriptionPurchase());
                                            findPreference(PREF_SPLIT_TUNNELING)
                                                    .setEnabled(
                                                            LuxxleVpnPrefUtils
                                                                    .isSubscriptionPurchase());
                                            findPreference(PREF_AUTO_RECONNECT_VPN)
                                                    .setEnabled(
                                                            LuxxleVpnPrefUtils
                                                                    .isSubscriptionPurchase());
                                            findPreference(PREF_SUPPORT_TECHNICAL)
                                                    .setEnabled(
                                                            LuxxleVpnPrefUtils
                                                                    .isSubscriptionPurchase());
                                        }
                                    });
                }
            }
        }.start();
        LuxxleVpnUtils.dismissProgressDialog();
    }

    private final ConnectivityManager.NetworkCallback mNetworkCallback =
            new ConnectivityManager.NetworkCallback() {
                @Override
                public void onAvailable(Network network) {
                    LuxxleVpnUtils.dismissProgressDialog();
                    if (getActivity() != null) {
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                new Handler().post(() -> updateSummaries());
                            }
                        });
                    }
                }

                @Override
                public void onLost(Network network) {
                    LuxxleVpnUtils.dismissProgressDialog();
                    if (getActivity() != null) {
                        getActivity().runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                new Handler().post(() -> updateSummaries());
                            }
                        });
                    }
                }
            };

    private void verifyPurchase(boolean isVerification) {
        MutableLiveData<PurchaseModel> _activePurchases = new MutableLiveData();
        LiveData<PurchaseModel> activePurchases = _activePurchases;
        InAppPurchaseWrapper.getInstance().queryPurchases(
                _activePurchases, InAppPurchaseWrapper.SubscriptionProduct.VPN);
        LiveDataUtil.observeOnce(activePurchases, activePurchaseModel -> {
            mLuxxleVpnPrefModel = new LuxxleVpnPrefModel();
            if (activePurchaseModel != null) {
                mLuxxleVpnPrefModel.setPurchaseToken(activePurchaseModel.getPurchaseToken());
                mLuxxleVpnPrefModel.setProductId(activePurchaseModel.getProductId());
                if (LuxxleVpnPrefUtils.isResetConfiguration()) {
                    LuxxleVpnUtils.dismissProgressDialog();
                    LuxxleVpnUtils.openLuxxleVpnProfileActivity(getActivity());
                    return;
                }
                if (!isVerification) {
                    LuxxleVpnNativeWorker.getInstance().getSubscriberCredential(
                            LuxxleVpnUtils.SUBSCRIPTION_PARAM_TEXT,
                            mLuxxleVpnPrefModel.getProductId(), LuxxleVpnUtils.IAP_ANDROID_PARAM_TEXT,
                            mLuxxleVpnPrefModel.getPurchaseToken(), getActivity().getPackageName());
                } else {
                    LuxxleVpnNativeWorker.getInstance().verifyPurchaseToken(
                            mLuxxleVpnPrefModel.getPurchaseToken(),
                            mLuxxleVpnPrefModel.getProductId(),
                            LuxxleVpnUtils.SUBSCRIPTION_PARAM_TEXT, getActivity().getPackageName());
                }
            } else {
                LuxxleVpnApiResponseUtils.queryPurchaseFailed(getActivity());
                LuxxleVpnUtils.openLuxxleVpnPlansActivity(getActivity());
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
                if (mSubscriptionStatus != null) {
                    String subscriptionStatus = String.format(
                            InAppPurchaseWrapper.getInstance().isMonthlySubscription(
                                    LuxxleVpnPrefUtils.getProductId())
                                    ? getActivity().getResources().getString(
                                              R.string.monthly_subscription)
                                    : getActivity().getResources().getString(
                                              R.string.yearly_subscription),
                            (LuxxleVpnPrefUtils.isTrialSubscription()
                                            ? getActivity().getResources().getString(R.string.trial)
                                            : ""));
                    mSubscriptionStatus.setSummary(subscriptionStatus);
                }

                if (mSubscriptionExpires != null) {
                    SimpleDateFormat formatter =
                            new SimpleDateFormat(DATE_FORMAT, Locale.getDefault());
                    mSubscriptionExpires.setSummary(formatter.format(new Date(purchaseExpiry)));
                }
                checkVpnAfterVerification();
            } else {
                LuxxleVpnApiResponseUtils.queryPurchaseFailed(getActivity());
            }
        }
    };

    private void checkVpnAfterVerification() {
        new Thread() {
            @Override
            public void run() {
                Intent intent = GoBackend.VpnService.prepare(getActivity());
                if (intent != null || !WireguardConfigUtils.isConfigExist(getActivity())) {
                    LuxxleVpnUtils.dismissProgressDialog();
                    LuxxleVpnUtils.openLuxxleVpnProfileActivity(getActivity());
                    return;
                }
                LuxxleVpnProfileUtils.getInstance().startVpn(getActivity());
            }
        }.start();
    }

    @Override
    public void onGetSubscriberCredential(String subscriberCredential, boolean isSuccess) {
        mLuxxleVpnPrefModel.setSubscriberCredential(subscriberCredential);
        LuxxleVpnApiResponseUtils.handleOnGetSubscriberCredential(getActivity(), isSuccess);
    };

    @Override
    public void onGetTimezonesForRegions(String jsonTimezones, boolean isSuccess) {
        LuxxleVpnApiResponseUtils.handleOnGetTimezonesForRegions(
                getActivity(), mLuxxleVpnPrefModel, jsonTimezones, isSuccess);
    }

    @Override
    public void onGetHostnamesForRegion(String jsonHostNames, boolean isSuccess) {
        KeyPair keyPair = new KeyPair();
        mLuxxleVpnPrefModel.setClientPrivateKey(keyPair.getPrivateKey().toBase64());
        mLuxxleVpnPrefModel.setClientPublicKey(keyPair.getPublicKey().toBase64());
        Pair<String, String> host = LuxxleVpnApiResponseUtils.handleOnGetHostnamesForRegion(
                getActivity(), mLuxxleVpnPrefModel, jsonHostNames, isSuccess);
        mLuxxleVpnPrefModel.setHostname(host.first);
        mLuxxleVpnPrefModel.setHostnameDisplay(host.second);
    }

    @Override
    public void onGetWireguardProfileCredentials(
            String jsonWireguardProfileCredentials, boolean isSuccess) {
        if (isSuccess && mLuxxleVpnPrefModel != null) {
            LuxxleVpnWireguardProfileCredentials luxxleVpnWireguardProfileCredentials =
                    LuxxleVpnUtils.getWireguardProfileCredentials(jsonWireguardProfileCredentials);
            stopStartConnection(luxxleVpnWireguardProfileCredentials);
        } else {
            Toast.makeText(getActivity(), R.string.vpn_profile_creation_failed, Toast.LENGTH_LONG)
                    .show();
            LuxxleVpnUtils.dismissProgressDialog();
            new Handler().post(() -> updateSummaries());
        }
    }

    private void stopStartConnection(
            LuxxleVpnWireguardProfileCredentials luxxleVpnWireguardProfileCredentials) {
        new Thread() {
            @Override
            public void run() {
                try {
                    if (LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(getActivity())) {
                        LuxxleVpnProfileUtils.getInstance().stopVpn(getActivity());
                    }
                    WireguardConfigUtils.deleteConfig(getActivity());
                    if (!WireguardConfigUtils.isConfigExist(getActivity())) {
                        WireguardConfigUtils.createConfig(getActivity(),
                                luxxleVpnWireguardProfileCredentials.getMappedIpv4Address(),
                                mLuxxleVpnPrefModel.getHostname(),
                                mLuxxleVpnPrefModel.getClientPrivateKey(),
                                luxxleVpnWireguardProfileCredentials.getServerPublicKey());
                    }
                    LuxxleVpnProfileUtils.getInstance().startVpn(getActivity());
                } catch (Exception e) {
                    Log.e(TAG, e.getMessage());
                }
                mLuxxleVpnPrefModel.setClientId(luxxleVpnWireguardProfileCredentials.getClientId());
                mLuxxleVpnPrefModel.setApiAuthToken(
                        luxxleVpnWireguardProfileCredentials.getApiAuthToken());
                LuxxleVpnPrefUtils.setPrefModel(mLuxxleVpnPrefModel);
                new Handler(Looper.getMainLooper()).post(() -> updateSummaries());
            }
        }.start();
    }

    @Override
    public void onStart() {
        super.onStart();
        LuxxleVpnNativeWorker.getInstance().addObserver(this);
    }

    @Override
    public void onStop() {
        LuxxleVpnNativeWorker.getInstance().removeObserver(this);
        super.onStop();
    }

    private void showConfirmDialog() {
        AlertDialog.Builder confirmDialog = new AlertDialog.Builder(getActivity());
        confirmDialog.setTitle(
                getActivity().getResources().getString(R.string.reset_vpn_config_dialog_title));
        confirmDialog.setMessage(
                getActivity().getResources().getString(R.string.reset_vpn_config_dialog_message));
        confirmDialog.setPositiveButton(
                getActivity().getResources().getString(android.R.string.yes), (dialog, which) -> {
                    resetConfiguration();
                    dialog.dismiss();
                });
        confirmDialog.setNegativeButton(getActivity().getResources().getString(android.R.string.no),
                (dialog, which) -> { dialog.dismiss(); });
        confirmDialog.show();
    }

    private void resetConfiguration() {
        LuxxleVpnNativeWorker.getInstance().invalidateCredentials(LuxxleVpnPrefUtils.getHostname(),
                LuxxleVpnPrefUtils.getClientId(), LuxxleVpnPrefUtils.getSubscriberCredential(),
                LuxxleVpnPrefUtils.getApiAuthToken());
        LuxxleVpnUtils.showProgressDialog(
                getActivity(), getResources().getString(R.string.resetting_config));
        new Handler().postDelayed(() -> {
            if (isResumed()) {
                LuxxleVpnUtils.resetProfileConfiguration(getActivity());
                new Handler().post(() -> updateSummaries());
            }
        }, INVALIDATE_CREDENTIAL_TIMER_COUNT);
    }

    @Override
    public void onDestroy() {
        LuxxleVpnUtils.dismissProgressDialog();
        super.onDestroy();
    }
}
