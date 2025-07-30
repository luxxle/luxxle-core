/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.media.ui;

import org.chromium.build.annotations.IdentifierNameString;
import org.chromium.chrome.browser.base.SplitCompatService;

public class LuxxleMediaNotificationControllerServices {
    public static class PlaybackListenerMicService extends SplitCompatService {
        private static @IdentifierNameString String sImplClassName =
                "org.chromium.chrome.browser.media.ui."
                        + "LuxxleMediaNotificationControllerDelegate$PlaybackListenerMicServiceImpl";

        public PlaybackListenerMicService() {
            super(sImplClassName);
        }
    }
}
