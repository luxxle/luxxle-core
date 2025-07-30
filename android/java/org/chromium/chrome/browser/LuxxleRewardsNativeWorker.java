/**
 * Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser;

import android.os.Handler;

import androidx.annotation.Nullable;

import org.jni_zero.CalledByNative;
import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;
import org.json.JSONException;

import org.chromium.luxxle_rewards.mojom.PublisherStatus;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.components.embedder_support.util.UrlConstants;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class LuxxleRewardsNativeWorker {
    /** Allows to monitor a front tab publisher changes. */
    public interface PublisherObserver {
        void onFrontTabPublisherChanged(boolean verified, String publisherId);
    }

    // Rewards notifications
    // Taken from components/luxxle_rewards/content/rewards_notification_service.h
    public static final int REWARDS_NOTIFICATION_INVALID = 0;
    public static final int REWARDS_NOTIFICATION_AUTO_CONTRIBUTE = 1;
    public static final int REWARDS_NOTIFICATION_FAILED_CONTRIBUTION = 4;
    public static final int REWARDS_NOTIFICATION_IMPENDING_CONTRIBUTION = 5;
    public static final int REWARDS_NOTIFICATION_TIPS_PROCESSED = 8;
    public static final int REWARDS_NOTIFICATION_ADS_ONBOARDING = 9;
    public static final int REWARDS_NOTIFICATION_VERIFIED_PUBLISHER = 10;
    public static final int REWARDS_NOTIFICATION_PENDING_NOT_ENOUGH_FUNDS = 11;
    public static final int REWARDS_NOTIFICATION_GENERAL = 12;

    public static final int OK = 0;
    public static final int FAILED = 1;
    public static final int BAT_NOT_ALLOWED = 25;
    public static final int SAFETYNET_ATTESTATION_FAILED = 27;

    private String mFrontTabUrl;
    private static final Handler sHandler = new Handler();

    private List<LuxxleRewardsObserver> mObservers;
    private List<PublisherObserver> mFrontTabPublisherObservers;
    private long mNativeLuxxleRewardsNativeWorker;

    private static LuxxleRewardsNativeWorker sInstance;
    private static final Object sLock = new Object();

    public static LuxxleRewardsNativeWorker getInstance() {
        synchronized (sLock) {
            if (sInstance == null) {
                sInstance = new LuxxleRewardsNativeWorker();
                sInstance.init();
          }
        }
        return sInstance;
    }

    private LuxxleRewardsNativeWorker() {
        mObservers = new ArrayList<LuxxleRewardsObserver>();
        mFrontTabPublisherObservers = new ArrayList<PublisherObserver>();
    }

    private void init() {
      if (mNativeLuxxleRewardsNativeWorker == 0) {
          LuxxleRewardsNativeWorkerJni.get().init(LuxxleRewardsNativeWorker.this);
      }
    }

    /**
     * A finalizer is required to ensure that the native object associated with this descriptor gets
     * torn down, otherwise there would be a memory leak.
     */
    @SuppressWarnings("Finalize")
    @Override
    protected void finalize() {
        destroy();
    }

    private void destroy() {
        if (mNativeLuxxleRewardsNativeWorker != 0) {
            LuxxleRewardsNativeWorkerJni.get().destroy(mNativeLuxxleRewardsNativeWorker);
            mNativeLuxxleRewardsNativeWorker = 0;
        }
    }

    public void addObserver(LuxxleRewardsObserver observer) {
        synchronized (sLock) {
            mObservers.add(observer);
        }
    }

    public void removeObserver(LuxxleRewardsObserver observer) {
        synchronized (sLock) {
            mObservers.remove(observer);
        }
    }

    public void addPublisherObserver(PublisherObserver observer) {
        synchronized (sLock) {
            mFrontTabPublisherObservers.add(observer);
        }
    }

    public void removePublisherObserver(PublisherObserver observer) {
        synchronized (sLock) {
            mFrontTabPublisherObservers.remove(observer);
        }
    }

    public void onNotifyFrontTabUrlChanged(int tabId, String url) {
        boolean chromeUrl = url.startsWith(UrlConstants.CHROME_SCHEME);
        boolean newUrl = (mFrontTabUrl == null || !mFrontTabUrl.equals(url));
        if (chromeUrl) {
            // Don't query 'GetPublisherInfo' and post response now.
            sHandler.post(
                    new Runnable() {
                        @Override
                        public void run() {
                            notifyPublisherObservers(false, "");
                        }
                    });
        } else if (newUrl) {
            getPublisherInfo(tabId, url);
        }

        mFrontTabUrl = url;
    }

    private void notifyPublisherObservers(boolean verified, String publisherId) {
        for (PublisherObserver observer : mFrontTabPublisherObservers) {
            observer.onFrontTabPublisherChanged(verified, publisherId);
        }
    }

    public void triggerOnNotifyFrontTabUrlChanged() {
        // Clear mFrontTabUrl so that all observers are updated.
        mFrontTabUrl = "";
        sHandler.post(
                new Runnable() {
                    @Override
                    public void run() {
                        Tab tab = LuxxleRewardsHelper.currentActiveChromeTabbedActivityTab();
                        if (tab != null && !tab.isIncognito()) {
                            onNotifyFrontTabUrlChanged(tab.getId(), tab.getUrl().getSpec());
                        }
                    }
                });
    }

    public boolean isSupported() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().isSupported(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public boolean isSupportedSkipRegionCheck() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().isSupportedSkipRegionCheck(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public boolean isRewardsEnabled() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().isRewardsEnabled(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public boolean shouldShowSelfCustodyInvite() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get()
                    .shouldShowSelfCustodyInvite(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void createRewardsWallet(String countryCode) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().createRewardsWallet(
                    mNativeLuxxleRewardsNativeWorker, countryCode);
        }
    }

    public void getRewardsParameters() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getRewardsParameters(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public double getVbatDeadline() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getVbatDeadline(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void getUserType() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getUserType(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void fetchBalance() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().fetchBalance(mNativeLuxxleRewardsNativeWorker);
        }
    }

    @Nullable
    public LuxxleRewardsBalance getWalletBalance() {
        synchronized (sLock) {
            String json = LuxxleRewardsNativeWorkerJni.get().getWalletBalance(
                    mNativeLuxxleRewardsNativeWorker);
            LuxxleRewardsBalance balance = null;
            try{
                balance = new LuxxleRewardsBalance(json);
            }
            catch (JSONException e) {
                balance = null;
            }
            return balance;
        }
    }

    public String getExternalWalletType() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getExternalWalletType(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public boolean canConnectAccount() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().canConnectAccount(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public double[] getTipChoices() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getTipChoices(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public double getWalletRate() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getWalletRate(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void getPublisherInfo(int tabId, String host) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getPublisherInfo(
                    mNativeLuxxleRewardsNativeWorker, tabId, host);
        }
    }

    public String getPublisherURL(int tabId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPublisherURL(
                    mNativeLuxxleRewardsNativeWorker, tabId);
        }
    }

    public String getCaptchaSolutionURL(String paymentId, String captchaId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getCaptchaSolutionURL(
                    mNativeLuxxleRewardsNativeWorker, paymentId, captchaId);
        }
    }

    public String getAttestationURL() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getAttestationURL(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public String getAttestationURLWithPaymentId(String paymentId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getAttestationURLWithPaymentId(
                    mNativeLuxxleRewardsNativeWorker, paymentId);
        }
    }

    public String getPublisherFavIconURL(int tabId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPublisherFavIconURL(
                    mNativeLuxxleRewardsNativeWorker, tabId);
        }
    }

    public String getPublisherName(int tabId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPublisherName(
                    mNativeLuxxleRewardsNativeWorker, tabId);
        }
    }

    public String getPublisherId(int tabId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPublisherId(
                    mNativeLuxxleRewardsNativeWorker, tabId);
        }
    }

    public int getPublisherPercent(int tabId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPublisherPercent(
                    mNativeLuxxleRewardsNativeWorker, tabId);
        }
    }

    public boolean getPublisherExcluded(int tabId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPublisherExcluded(
                    mNativeLuxxleRewardsNativeWorker, tabId);
        }
    }

    public int getPublisherStatus(int tabId) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPublisherStatus(
                    mNativeLuxxleRewardsNativeWorker, tabId);
        }
    }

    public void removePublisherFromMap(int tabId) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().removePublisherFromMap(
                    mNativeLuxxleRewardsNativeWorker, tabId);
        }
    }

    public void getCurrentBalanceReport() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getCurrentBalanceReport(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void donate(String publisherKey, double amount, boolean recurring) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get()
                    .donate(mNativeLuxxleRewardsNativeWorker, publisherKey, amount, recurring);
        }
    }

    public void getAllNotifications() {
        sHandler.post(
                new Runnable() {
                    @Override
                    public void run() {
                        synchronized (sLock) {
                            LuxxleRewardsNativeWorkerJni.get()
                                    .getAllNotifications(mNativeLuxxleRewardsNativeWorker);
                        }
                    }
                });
    }

    public void deleteNotification(String notificationId) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get()
                    .deleteNotification(mNativeLuxxleRewardsNativeWorker, notificationId);
        }
    }

    public void getRecurringDonations() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getRecurringDonations(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public boolean isCurrentPublisherInRecurrentDonations(String publisher) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().isCurrentPublisherInRecurrentDonations(
                    mNativeLuxxleRewardsNativeWorker, publisher);
        }
    }

    public double getPublisherRecurrentDonationAmount(String publisher) {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPublisherRecurrentDonationAmount(
                    mNativeLuxxleRewardsNativeWorker, publisher);
        }
    }

    public void getReconcileStamp() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getReconcileStamp(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void removeRecurring(String publisher) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().removeRecurring(
                    mNativeLuxxleRewardsNativeWorker, publisher);
        }
    }

    public void resetTheWholeState() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().resetTheWholeState(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public int getAdsPerHour() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getAdsPerHour(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void setAdsPerHour(int value) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().setAdsPerHour(mNativeLuxxleRewardsNativeWorker, value);
        }
    }

    public void getExternalWallet() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getExternalWallet(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public boolean isTermsOfServiceUpdateRequired() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get()
                    .isTermsOfServiceUpdateRequired(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void acceptTermsOfServiceUpdate() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get()
                    .acceptTermsOfServiceUpdate(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public String getCountryCode() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getCountryCode(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void getAvailableCountries() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getAvailableCountries(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void getPublisherBanner(String publisherKey) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get()
                    .getPublisherBanner(mNativeLuxxleRewardsNativeWorker, publisherKey);
        }
    }

    public void getPublishersVisitedCount() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getPublishersVisitedCount(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    @CalledByNative
    public void onGetPublishersVisitedCount(int count) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onGetPublishersVisitedCount(count);
        }
    }

    public void disconnectWallet() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().disconnectWallet(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public void getAdsAccountStatement() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().getAdsAccountStatement(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    @CalledByNative
    public void onCreateRewardsWallet(String result) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onCreateRewardsWallet(result);
        }
    }

    public void refreshPublisher(String publisherKey) {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().refreshPublisher(
                    mNativeLuxxleRewardsNativeWorker, publisherKey);
        }
    }

    public void recordPanelTrigger() {
        synchronized (sLock) {
            LuxxleRewardsNativeWorkerJni.get().recordPanelTrigger(mNativeLuxxleRewardsNativeWorker);
        }
    }

    public String getPayoutStatus() {
        synchronized (sLock) {
            return LuxxleRewardsNativeWorkerJni.get().getPayoutStatus(
                    mNativeLuxxleRewardsNativeWorker);
        }
    }

    @CalledByNative
    public void onRefreshPublisher(int status, String publisherKey) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onRefreshPublisher(status, publisherKey);
        }
    }

    @CalledByNative
    public void onRewardsParameters() {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onRewardsParameters();
        }
    }

    @CalledByNative
    public void onTermsOfServiceUpdateAccepted() {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onTermsOfServiceUpdateAccepted();
        }
    }

    @CalledByNative
    public void onBalance(boolean success) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onBalance(success);
        }
    }

    @CalledByNative
    public void onGetCurrentBalanceReport(double[] report) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onGetCurrentBalanceReport(report);
        }
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeLuxxleRewardsNativeWorker == 0;
        mNativeLuxxleRewardsNativeWorker = nativePtr;
    }

    @CalledByNative
    public void onPublisherInfo(int tabId, String publisherId) {
        int pubStatus = getPublisherStatus(tabId);
        boolean verified = pubStatus != PublisherStatus.NOT_VERIFIED;
        notifyPublisherObservers(verified, publisherId);

        // Notify LuxxleRewardsObserver (panel).
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onPublisherInfo(tabId);
        }
    }

    @CalledByNative
    public void onNotificationAdded(String id, int type, long timestamp, String[] args) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onNotificationAdded(id, type, timestamp, args);
        }
    }

    @CalledByNative
    public void onNotificationsCount(int count) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onNotificationsCount(count);
        }
    }

    @CalledByNative
    public void onGetLatestNotification(String id, int type, long timestamp, String[] args) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onGetLatestNotification(id, type, timestamp, args);
        }
    }

    @CalledByNative
    public void onNotificationDeleted(String id) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onNotificationDeleted(id);
        }
    }

    @CalledByNative
    public void onGetReconcileStamp(long timestamp) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onGetReconcileStamp(timestamp);
        }
    }

    @CalledByNative
    public void onRecurringDonationUpdated() {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onRecurringDonationUpdated();
        }
    }

    @CalledByNative
    public void onCompleteReset(boolean success) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onCompleteReset(success);
        }
    }

    @CalledByNative
    public void onResetTheWholeState(boolean success) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onResetTheWholeState(success);
        }
    }

    @CalledByNative
    public void onGetExternalWallet(String externalWallet) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onGetExternalWallet(externalWallet);
        }
    }

    @CalledByNative
    public void onGetAvailableCountries(String[] countries) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onGetAvailableCountries(countries);
        }
    }

    @CalledByNative
    public void onGetAdsAccountStatement(
            boolean success,
            double nextPaymentDate,
            int adsReceivedThisMonth,
            double minEarningsThisMonth,
            double maxEarningsThisMonth,
            double minEarningsLastMonth,
            double maxEarningsLastMonth) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onGetAdsAccountStatement(
                    success,
                    nextPaymentDate,
                    adsReceivedThisMonth,
                    minEarningsThisMonth,
                    maxEarningsThisMonth,
                    minEarningsLastMonth,
                    maxEarningsLastMonth);
        }
    }

    @CalledByNative
    public void onExternalWalletConnected() {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onExternalWalletConnected();
        }
    }

    @CalledByNative
    public void onExternalWalletLoggedOut() {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onExternalWalletLoggedOut();
        }
    }

    @CalledByNative
    public void onExternalWalletReconnected() {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onExternalWalletReconnected();
        }
    }

    @CalledByNative
    public void onSendContribution(boolean result) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onSendContribution(result);
        }
    }

    @CalledByNative
    public void onReconcileComplete(int resultCode, int rewardsType, double amount) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onReconcileComplete(resultCode, rewardsType, amount);
        }
    }

    @CalledByNative
    public void onPublisherBanner(String jsonBannerInfo) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onPublisherBanner(jsonBannerInfo);
        }
    }

    @CalledByNative
    public void onGetUserType(int userType) {
        for (LuxxleRewardsObserver observer : mObservers) {
            observer.onGetUserType(userType);
        }
    }

    @NativeMethods
    interface Natives {
        void init(LuxxleRewardsNativeWorker caller);
        void destroy(long nativeLuxxleRewardsNativeWorker);
        boolean isSupported(long nativeLuxxleRewardsNativeWorker);
        boolean isSupportedSkipRegionCheck(long nativeLuxxleRewardsNativeWorker);
        boolean isRewardsEnabled(long nativeLuxxleRewardsNativeWorker);

        boolean shouldShowSelfCustodyInvite(long nativeLuxxleRewardsNativeWorker);

        String getWalletBalance(long nativeLuxxleRewardsNativeWorker);
        String getExternalWalletType(long nativeLuxxleRewardsNativeWorker);

        void getPublisherBanner(long nativeLuxxleRewardsNativeWorker, String publisherKey);

        void getPublishersVisitedCount(long nativeLuxxleRewardsNativeWorker);
        boolean canConnectAccount(long nativeLuxxleRewardsNativeWorker);

        double[] getTipChoices(long nativeLuxxleRewardsNativeWorker);

        double getWalletRate(long nativeLuxxleRewardsNativeWorker);

        void getPublisherInfo(long nativeLuxxleRewardsNativeWorker, int tabId, String host);

        String getPublisherURL(long nativeLuxxleRewardsNativeWorker, int tabId);

        String getCaptchaSolutionURL(
                long nativeLuxxleRewardsNativeWorker, String paymentId, String captchaId);

        String getAttestationURL(long nativeLuxxleRewardsNativeWorker);

        String getAttestationURLWithPaymentId(
                long nativeLuxxleRewardsNativeWorker, String paymentId);

        String getPublisherFavIconURL(long nativeLuxxleRewardsNativeWorker, int tabId);

        String getPublisherName(long nativeLuxxleRewardsNativeWorker, int tabId);

        String getPublisherId(long nativeLuxxleRewardsNativeWorker, int tabId);

        int getPublisherPercent(long nativeLuxxleRewardsNativeWorker, int tabId);

        boolean getPublisherExcluded(long nativeLuxxleRewardsNativeWorker, int tabId);

        int getPublisherStatus(long nativeLuxxleRewardsNativeWorker, int tabId);

        void removePublisherFromMap(long nativeLuxxleRewardsNativeWorker, int tabId);

        void getCurrentBalanceReport(long nativeLuxxleRewardsNativeWorker);

        void donate(
                long nativeLuxxleRewardsNativeWorker,
                String publisherKey,
                double amount,
                boolean recurring);

        void getAllNotifications(long nativeLuxxleRewardsNativeWorker);

        void deleteNotification(long nativeLuxxleRewardsNativeWorker, String notificationId);

        void getRecurringDonations(long nativeLuxxleRewardsNativeWorker);

        boolean isCurrentPublisherInRecurrentDonations(
                long nativeLuxxleRewardsNativeWorker, String publisher);

        void getReconcileStamp(long nativeLuxxleRewardsNativeWorker);

        double getPublisherRecurrentDonationAmount(
                long nativeLuxxleRewardsNativeWorker, String publisher);

        void removeRecurring(long nativeLuxxleRewardsNativeWorker, String publisher);

        void resetTheWholeState(long nativeLuxxleRewardsNativeWorker);

        int getAdsPerHour(long nativeLuxxleRewardsNativeWorker);

        void setAdsPerHour(long nativeLuxxleRewardsNativeWorker, int value);

        void getExternalWallet(long nativeLuxxleRewardsNativeWorker);

        boolean isTermsOfServiceUpdateRequired(long nativeLuxxleRewardsNativeWorker);

        void acceptTermsOfServiceUpdate(long nativeLuxxleRewardsNativeWorker);

        String getCountryCode(long nativeLuxxleRewardsNativeWorker);
        void getAvailableCountries(long nativeLuxxleRewardsNativeWorker);
        void disconnectWallet(long nativeLuxxleRewardsNativeWorker);
        void refreshPublisher(long nativeLuxxleRewardsNativeWorker, String publisherKey);

        void recordPanelTrigger(long nativeLuxxleRewardsNativeWorker);

        void createRewardsWallet(long nativeLuxxleRewardsNativeWorker, String countryCode);

        void getRewardsParameters(long nativeLuxxleRewardsNativeWorker);

        double getVbatDeadline(long nativeLuxxleRewardsNativeWorker);

        void getUserType(long nativeLuxxleRewardsNativeWorker);

        void fetchBalance(long nativeLuxxleRewardsNativeWorker);

        void getAdsAccountStatement(long nativeLuxxleRewardsNativeWorker);

        String getPayoutStatus(long nativeLuxxleRewardsNativeWorker);
    }
}
