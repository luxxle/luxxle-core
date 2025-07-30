/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.externalnav;

import android.content.Intent;

import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.preferences.LuxxlePrefServiceBridge;
import org.chromium.chrome.browser.privacy.settings.LuxxlePrivacySettings;
import org.chromium.chrome.browser.util.LuxxleConstants;
import org.chromium.components.external_intents.ExternalNavigationDelegate;
import org.chromium.components.external_intents.ExternalNavigationHandler;
import org.chromium.components.external_intents.ExternalNavigationHandler.OverrideUrlLoadingResult;
import org.chromium.components.external_intents.ExternalNavigationParams;
import org.chromium.url.GURL;

/**
 * Extends Chromium's ExternalNavigationHandler
 */
public class LuxxleExternalNavigationHandler extends ExternalNavigationHandler {
    private static final String TAG = "LuxxleUrlHandler";

    public LuxxleExternalNavigationHandler(ExternalNavigationDelegate delegate) {
        super(delegate);
    }

    @Override
    public OverrideUrlLoadingResult shouldOverrideUrlLoading(ExternalNavigationParams params) {
        String originalUrl = params.getUrl().getSpec();
        if (originalUrl.equalsIgnoreCase("chrome://adblock/")) {
            try {
                LuxxleActivity.getLuxxleActivity().openLuxxleContentFilteringSettings();
            } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
                Log.e(TAG, "adblock url " + e);
            }
            return OverrideUrlLoadingResult.forExternalIntent();
        }
        return super.shouldOverrideUrlLoading(params);
    }

    @Override
    protected OverrideUrlLoadingResult startActivity(
            Intent intent,
            ExternalNavigationParams params,
            boolean requiresIntentChooser,
            QueryIntentActivitiesSupplier resolvingInfos,
            ResolveActivitySupplier resolveActivity,
            GURL browserFallbackUrl,
            GURL intentTargetUrl) {
        boolean isYoutubeDomain =
                intentTargetUrl != null
                        ? intentTargetUrl.domainIs(LuxxleConstants.YOUTUBE_DOMAIN)
                        : false;
        if ((isYoutubeDomain
                        && !LuxxlePrefServiceBridge.getInstance().getPlayYTVideoInBrowserEnabled())
                || (!isYoutubeDomain
                        && ContextUtils.getAppSharedPreferences()
                                .getBoolean(LuxxlePrivacySettings.PREF_APP_LINKS, true))) {
            return super.startActivity(
                    intent,
                    params,
                    requiresIntentChooser,
                    resolvingInfos,
                    resolveActivity,
                    browserFallbackUrl,
                    intentTargetUrl);
        } else {
            return OverrideUrlLoadingResult.forNoOverride();
        }
    }
}
