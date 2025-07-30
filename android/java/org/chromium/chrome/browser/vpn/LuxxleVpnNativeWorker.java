/**
 * Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn;

import org.jni_zero.CalledByNative;
import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;

import java.util.ArrayList;
import java.util.List;

@JNINamespace("chrome::android")
public class LuxxleVpnNativeWorker {
    private long mNativeLuxxleVpnNativeWorker;
    private static final Object sLock = new Object();
    private static LuxxleVpnNativeWorker sInstance;

    private List<LuxxleVpnObserver> mObservers;

    public static LuxxleVpnNativeWorker getInstance() {
        synchronized (sLock) {
            if (sInstance == null) {
                sInstance = new LuxxleVpnNativeWorker();
                sInstance.init();
            }
        }
        return sInstance;
    }

    private LuxxleVpnNativeWorker() {
        mObservers = new ArrayList<LuxxleVpnObserver>();
    }

    private void init() {
        if (mNativeLuxxleVpnNativeWorker == 0) {
            LuxxleVpnNativeWorkerJni.get().init(this);
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
        if (mNativeLuxxleVpnNativeWorker != 0) {
            LuxxleVpnNativeWorkerJni.get().destroy(mNativeLuxxleVpnNativeWorker, this);
            mNativeLuxxleVpnNativeWorker = 0;
        }
    }

    public void addObserver(LuxxleVpnObserver observer) {
        synchronized (sLock) {
            mObservers.add(observer);
        }
    }

    public void removeObserver(LuxxleVpnObserver observer) {
        synchronized (sLock) {
            mObservers.remove(observer);
        }
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeLuxxleVpnNativeWorker == 0;
        mNativeLuxxleVpnNativeWorker = nativePtr;
    }

    @CalledByNative
    public void onGetTimezonesForRegions(String jsonTimezones, boolean isSuccess) {
        for (LuxxleVpnObserver observer : mObservers) {
            observer.onGetTimezonesForRegions(jsonTimezones, isSuccess);
        }
    }

    @CalledByNative
    public void onGetHostnamesForRegion(String jsonHostnames, boolean isSuccess) {
        for (LuxxleVpnObserver observer : mObservers) {
            observer.onGetHostnamesForRegion(jsonHostnames, isSuccess);
        }
    }

    @CalledByNative
    public void onGetWireguardProfileCredentials(
            String jsonWireguardProfileCredentials, boolean isSuccess) {
        for (LuxxleVpnObserver observer : mObservers) {
            observer.onGetWireguardProfileCredentials(jsonWireguardProfileCredentials, isSuccess);
        }
    }

    @CalledByNative
    public void onVerifyCredentials(String jsonVerifyCredentials, boolean isSuccess) {
        for (LuxxleVpnObserver observer : mObservers) {
            observer.onVerifyCredentials(jsonVerifyCredentials, isSuccess);
        }
    }

    @CalledByNative
    public void onInvalidateCredentials(String jsonInvalidateCredentials, boolean isSuccess) {
        for (LuxxleVpnObserver observer : mObservers) {
            observer.onInvalidateCredentials(jsonInvalidateCredentials, isSuccess);
        }
    }

    @CalledByNative
    public void onGetSubscriberCredential(String subscriberCredential, boolean isSuccess) {
        for (LuxxleVpnObserver observer : mObservers) {
            observer.onGetSubscriberCredential(subscriberCredential, isSuccess);
        }
    }

    @CalledByNative
    public void onVerifyPurchaseToken(
            String jsonResponse, String purchaseToken, String productId, boolean isSuccess) {
        for (LuxxleVpnObserver observer : mObservers) {
            observer.onVerifyPurchaseToken(jsonResponse, purchaseToken, productId, isSuccess);
        }
    }

    public void getTimezonesForRegions() {
        LuxxleVpnNativeWorkerJni.get().getTimezonesForRegions(mNativeLuxxleVpnNativeWorker);
    }

    public void getHostnamesForRegion(String region, String regionPrecision) {
        LuxxleVpnNativeWorkerJni.get()
                .getHostnamesForRegion(mNativeLuxxleVpnNativeWorker, region, regionPrecision);
    }

    public void getWireguardProfileCredentials(
            String subscriberCredential, String publicKey, String hostname) {
        LuxxleVpnNativeWorkerJni.get().getWireguardProfileCredentials(
                mNativeLuxxleVpnNativeWorker, subscriberCredential, publicKey, hostname);
    }

    public void verifyCredentials(
            String hostname, String clientId, String subscriberCredential, String apiAuthToken) {
        LuxxleVpnNativeWorkerJni.get().verifyCredentials(mNativeLuxxleVpnNativeWorker, hostname,
                clientId, subscriberCredential, apiAuthToken);
    }

    public void invalidateCredentials(
            String hostname, String clientId, String subscriberCredential, String apiAuthToken) {
        LuxxleVpnNativeWorkerJni.get().invalidateCredentials(mNativeLuxxleVpnNativeWorker, hostname,
                clientId, subscriberCredential, apiAuthToken);
    }

    public void getSubscriberCredential(String productType, String productId,
            String validationMethod, String purchaseToken, String packageName) {
        LuxxleVpnNativeWorkerJni.get().getSubscriberCredential(mNativeLuxxleVpnNativeWorker,
                productType, productId, validationMethod, purchaseToken, packageName);
    }

    public void verifyPurchaseToken(
            String purchaseToken, String productId, String productType, String packageName) {
        LuxxleVpnNativeWorkerJni.get().verifyPurchaseToken(
                mNativeLuxxleVpnNativeWorker, purchaseToken, productId, productType, packageName);
    }

    // Desktop purchase methods
    public void reloadPurchasedState() {
        LuxxleVpnNativeWorkerJni.get().reloadPurchasedState(mNativeLuxxleVpnNativeWorker);
    }

    public boolean isPurchasedUser() {
        return LuxxleVpnNativeWorkerJni.get().isPurchasedUser(mNativeLuxxleVpnNativeWorker);
    }

    public void getSubscriberCredentialV12() {
        LuxxleVpnNativeWorkerJni.get().getSubscriberCredentialV12(mNativeLuxxleVpnNativeWorker);
    }

    public void reportBackgroundP3A(long sessionStartTimeMs, long sessionEndTimeMs) {
        LuxxleVpnNativeWorkerJni.get().reportBackgroundP3A(
                mNativeLuxxleVpnNativeWorker, sessionStartTimeMs, sessionEndTimeMs);
    }

    public void reportForegroundP3A() {
        LuxxleVpnNativeWorkerJni.get().reportForegroundP3A(mNativeLuxxleVpnNativeWorker);
    }

    @NativeMethods
    interface Natives {
        void init(LuxxleVpnNativeWorker caller);

        void destroy(long nativeLuxxleVpnNativeWorker, LuxxleVpnNativeWorker caller);

        void getTimezonesForRegions(long nativeLuxxleVpnNativeWorker);

        void getHostnamesForRegion(
                long nativeLuxxleVpnNativeWorker, String region, String regionPrecision);

        void getWireguardProfileCredentials(
                long nativeLuxxleVpnNativeWorker,
                String subscriberCredential,
                String publicKey,
                String hostname);

        void verifyCredentials(
                long nativeLuxxleVpnNativeWorker,
                String hostname,
                String clientId,
                String subscriberCredential,
                String apiAuthToken);

        void invalidateCredentials(
                long nativeLuxxleVpnNativeWorker,
                String hostname,
                String clientId,
                String subscriberCredential,
                String apiAuthToken);

        void getSubscriberCredential(
                long nativeLuxxleVpnNativeWorker,
                String productType,
                String productId,
                String validationMethod,
                String purchaseToken,
                String packageName);

        void verifyPurchaseToken(
                long nativeLuxxleVpnNativeWorker,
                String purchaseToken,
                String productId,
                String productType,
                String packageName);

        void reloadPurchasedState(long nativeLuxxleVpnNativeWorker);

        boolean isPurchasedUser(long nativeLuxxleVpnNativeWorker);

        void getSubscriberCredentialV12(long nativeLuxxleVpnNativeWorker);

        void reportBackgroundP3A(
                long nativeLuxxleVpnNativeWorker, long sessionStartTimeMs, long sessionEndTimeMs);

        void reportForegroundP3A(long nativeLuxxleVpnNativeWorker);
    }
}
