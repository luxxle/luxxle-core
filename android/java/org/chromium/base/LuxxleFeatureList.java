/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.base;

import org.chromium.build.annotations.NullMarked;

/** A list of feature flags exposed to Java. */
@NullMarked
public abstract class LuxxleFeatureList {
    public static final String NATIVE_LUXXLE_WALLET = "NativeLuxxleWallet";
    public static final String USE_DEV_UPDATER_URL = "UseDevUpdaterUrl";
    public static final String FORCE_WEB_CONTENTS_DARK_MODE = "WebContentsForceDark";
    public static final String ENABLE_FORCE_DARK = "enable-force-dark";
    public static final String ENABLE_PARALLEL_DOWNLOADING = "enable-parallel-downloading";
    public static final String LUXXLE_SEARCH_OMNIBOX_BANNER = "LuxxleSearchOmniboxBanner";
    public static final String LUXXLE_BACKGROUND_VIDEO_PLAYBACK = "LuxxleBackgroundVideoPlayback";
    public static final String LUXXLE_ANDROID_SAFE_BROWSING = "LuxxleAndroidSafeBrowsing";
    public static final String LUXXLE_VPN_LINK_SUBSCRIPTION_ANDROID_UI =
            "LuxxleVPNLinkSubscriptionAndroidUI";
    public static final String DEBOUNCE = "LuxxleDebounce";
    public static final String LUXXLE_GOOGLE_SIGN_IN_PERMISSION = "LuxxleGoogleSignInPermission";
    public static final String LUXXLE_LOCALHOST_PERMISSION = "LuxxleLocalhostPermission";
    public static final String LUXXLE_PLAYLIST = "Playlist";
    public static final String LUXXLE_SPEEDREADER = "Speedreader";
    public static final String HTTPS_BY_DEFAULT = "HttpsByDefault";
    public static final String LUXXLE_FORGET_FIRST_PARTY_STORAGE = "LuxxleForgetFirstPartyStorage";
    public static final String LUXXLE_REQUEST_OTR_TAB = "LuxxleRequestOTRTab";
    public static final String AI_CHAT = "AIChat";
    public static final String AI_CHAT_HISTORY = "AIChatHistory";
    public static final String LUXXLE_SHOW_STRICT_FINGERPRINTING_MODE =
            "LuxxleShowStrictFingerprintingMode";
    public static final String LUXXLE_DAY_ZERO_EXPERIMENT = "LuxxleDayZeroExperiment";
    public static final String LUXXLE_NEW_ANDROID_ONBOARDING = "NewAndroidOnboarding";
    public static final String LUXXLE_FALLBACK_DOH_PROVIDER = "LuxxleFallbackDoHProvider";
    public static final String LUXXLE_BLOCK_ALL_COOKIES_TOGGLE = "BlockAllCookiesToggle";
    public static final String LUXXLE_SHIELDS_ELEMENT_PICKER = "LuxxleShieldsElementPicker";
    public static final String LUXXLE_WEB_DISCOVERY_NATIVE = "LuxxleWebDiscoveryNative";
    public static final String LUXXLE_INCOGNITO_SCREENSHOT = "incognito-screenshot";
}
