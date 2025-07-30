/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import org.jni_zero.CalledByNative;
import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;

import org.chromium.base.Callback;

import java.time.LocalDateTime;
import java.time.ZoneOffset;

@JNINamespace("chrome::android")
public class LuxxleSyncWorker {
    private static final String TAG = "SYNC";

    private long mNativeLuxxleSyncWorker;

    private static LuxxleSyncWorker sLuxxleSyncWorker;
    private static boolean sInitialized;

    public static LuxxleSyncWorker get() {
        if (!sInitialized) {
            sLuxxleSyncWorker = new LuxxleSyncWorker();
            sInitialized = true;
        }
        return sLuxxleSyncWorker;
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeLuxxleSyncWorker == 0;
        mNativeLuxxleSyncWorker = nativePtr;
    }

    private void init() {
        if (mNativeLuxxleSyncWorker == 0) {
            LuxxleSyncWorkerJni.get().init(LuxxleSyncWorker.this);
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
        if (mNativeLuxxleSyncWorker != 0) {
            LuxxleSyncWorkerJni.get().destroy(mNativeLuxxleSyncWorker);
            mNativeLuxxleSyncWorker = 0;
        }
    }

    public LuxxleSyncWorker() {
        init();
    }

    public String getPureWords() {
        return LuxxleSyncWorkerJni.get().getSyncCodeWords(mNativeLuxxleSyncWorker);
    }

    public String getTimeLimitedWordsFromPure(String pureWords) {
        return LuxxleSyncWorkerJni.get().getTimeLimitedWordsFromPure(pureWords);
    }

    public void saveCodephrase(String codephrase) {
        LuxxleSyncWorkerJni.get().saveCodeWords(mNativeLuxxleSyncWorker, codephrase);
    }

    public String getSeedHexFromWords(String codephrase) {
        return LuxxleSyncWorkerJni.get().getSeedHexFromWords(codephrase);
    }

    public String getWordsFromSeedHex(String seedHex) {
        return LuxxleSyncWorkerJni.get().getWordsFromSeedHex(seedHex);
    }

    public String getQrDataJson(String seedHex) {
        return LuxxleSyncWorkerJni.get().getQrDataJson(seedHex);
    }

    public int getQrCodeValidationResult(String jsonQr) {
        return LuxxleSyncWorkerJni.get().getQrCodeValidationResult(jsonQr);
    }

    public String getSeedHexFromQrJson(String jsonQr) {
        return LuxxleSyncWorkerJni.get().getSeedHexFromQrJson(jsonQr);
    }

    public int getWordsValidationResult(String timeLimitedWords) {
        return LuxxleSyncWorkerJni.get().getWordsValidationResult(timeLimitedWords);
    }

    public String getPureWordsFromTimeLimited(String timeLimitedWords) {
        return LuxxleSyncWorkerJni.get().getPureWordsFromTimeLimited(timeLimitedWords);
    }

    public LocalDateTime getNotAfterFromFromTimeLimitedWords(String timeLimitedWords) {
        long unixTime =
                LuxxleSyncWorkerJni.get().getNotAfterFromFromTimeLimitedWords(timeLimitedWords);
        LocalDateTime notAfter = LocalDateTime.ofEpochSecond(unixTime, 0, ZoneOffset.UTC);
        return notAfter;
    }

    public String getFormattedTimeDelta(long seconds) {
        return LuxxleSyncWorkerJni.get().getFormattedTimeDelta(seconds);
    }

    public void requestSync() {
        LuxxleSyncWorkerJni.get().requestSync(mNativeLuxxleSyncWorker);
    }

    public boolean isInitialSyncFeatureSetupComplete() {
        return LuxxleSyncWorkerJni.get().isInitialSyncFeatureSetupComplete(mNativeLuxxleSyncWorker);
    }

    public void finalizeSyncSetup() {
        LuxxleSyncWorkerJni.get().finalizeSyncSetup(mNativeLuxxleSyncWorker);
    }

    public void resetSync() {
        LuxxleSyncWorkerJni.get().resetSync(mNativeLuxxleSyncWorker);
    }

    @CalledByNative
    private static void onPermanentlyDeleteAccountResult(Callback<String> callback, String result) {
        callback.onResult(result);
    }

    public void permanentlyDeleteAccount(Callback<String> callback) {
        LuxxleSyncWorkerJni.get().permanentlyDeleteAccount(mNativeLuxxleSyncWorker, callback);
    }

    public void clearAccountDeletedNoticePending() {
        LuxxleSyncWorkerJni.get().clearAccountDeletedNoticePending(mNativeLuxxleSyncWorker);
    }

    public boolean isAccountDeletedNoticePending() {
        return LuxxleSyncWorkerJni.get().isAccountDeletedNoticePending(mNativeLuxxleSyncWorker);
    }

    @CalledByNative
    private static void onJoinSyncChainResult(Callback<Boolean> callback, Boolean result) {
        callback.onResult(result);
    }

    public void setJoinSyncChainCallback(Callback<Boolean> callback) {
        LuxxleSyncWorkerJni.get().setJoinSyncChainCallback(mNativeLuxxleSyncWorker, callback);
    }

    public int getWordsCount(String words) {
        return LuxxleSyncWorkerJni.get().getWordsCount(words);
    }

    @NativeMethods
    interface Natives {
        void init(LuxxleSyncWorker caller);

        void destroy(long nativeLuxxleSyncWorker);

        String getSyncCodeWords(long nativeLuxxleSyncWorker);

        void requestSync(long nativeLuxxleSyncWorker);

        String getSeedHexFromWords(String passphrase);

        String getWordsFromSeedHex(String seedHex);

        String getQrDataJson(String seedHex);

        int getQrCodeValidationResult(String jsonQr);

        String getSeedHexFromQrJson(String jsonQr);

        int getWordsValidationResult(String timeLimitedWords);

        String getPureWordsFromTimeLimited(String timeLimitedWords);

        String getTimeLimitedWordsFromPure(String pureWords);

        long getNotAfterFromFromTimeLimitedWords(String pureWords);

        String getFormattedTimeDelta(long seconds);

        void saveCodeWords(long nativeLuxxleSyncWorker, String passphrase);

        int getWordsCount(String words);

        void finalizeSyncSetup(long nativeLuxxleSyncWorker);

        boolean isInitialSyncFeatureSetupComplete(long nativeLuxxleSyncWorker);

        void resetSync(long nativeLuxxleSyncWorker);

        void permanentlyDeleteAccount(long nativeLuxxleSyncWorker, Callback<String> callback);
        void clearAccountDeletedNoticePending(long nativeLuxxleSyncWorker);
        boolean isAccountDeletedNoticePending(long nativeLuxxleSyncWorker);
        void setJoinSyncChainCallback(long nativeLuxxleSyncWorker, Callback<Boolean> callback);
    }
}
