/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.luxxle_news;

import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;

import org.chromium.base.Promise;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskRunner;
import org.chromium.base.task.TaskTraits;
import org.chromium.luxxle_news.mojom.LuxxleNewsController;
import org.chromium.build.annotations.NullMarked;
import org.chromium.build.annotations.Nullable;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.bindings.Interface;
import org.chromium.mojo.bindings.Interface.Proxy.Handler;
import org.chromium.mojo.system.MessagePipeHandle;
import org.chromium.mojo.system.impl.CoreImpl;

@JNINamespace("chrome::android")
@NullMarked
public class LuxxleNewsControllerFactory {
    private static final Object sLock = new Object();
    private static @Nullable LuxxleNewsControllerFactory sInstance;
    private final TaskRunner mTaskRunner;

    public static LuxxleNewsControllerFactory getInstance() {
        synchronized (sLock) {
            if (sInstance == null) {
                sInstance = new LuxxleNewsControllerFactory();
            }
        }
        return sInstance;
    }

    private LuxxleNewsControllerFactory() {
        mTaskRunner = PostTask.createSequencedTaskRunner(TaskTraits.UI_DEFAULT);
    }

    public Promise<@Nullable LuxxleNewsController> getForProfile(
            Profile profile, @Nullable ConnectionErrorHandler connectionErrorHandler) {
        final Promise<@Nullable LuxxleNewsController> promise = new Promise<>();

        mTaskRunner.execute(
                () -> {
                    long nativeHandle =
                            LuxxleNewsControllerFactoryJni.get()
                                    .getInterfaceToLuxxleNewsController(profile);
                    MessagePipeHandle handle = wrapNativeHandle(nativeHandle);
                    if (!handle.isValid()) {
                        promise.fulfill(null);
                        return;
                    }
                    LuxxleNewsController luxxleNewsController =
                            LuxxleNewsController.MANAGER.attachProxy(handle, 0);
                    if (connectionErrorHandler != null) {
                        Handler handler = ((Interface.Proxy) luxxleNewsController).getProxyHandler();
                        handler.setErrorHandler(connectionErrorHandler);
                    }

                    promise.fulfill(luxxleNewsController);
                });

        return promise;
    }

    private MessagePipeHandle wrapNativeHandle(long nativeHandle) {
        return CoreImpl.getInstance().acquireNativeHandle(nativeHandle).toMessagePipeHandle();
    }

    @NativeMethods
    interface Natives {
        long getInterfaceToLuxxleNewsController(Profile profile);
    }
}
