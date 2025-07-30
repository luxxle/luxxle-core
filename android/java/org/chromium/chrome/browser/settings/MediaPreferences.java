/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.os.Bundle;

import androidx.preference.Preference;

import org.chromium.base.LuxxleFeatureList;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.luxxle_shields.mojom.FilterListAndroidHandler;
import org.chromium.luxxle_shields.mojom.FilterListConstants;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleLocalState;
import org.chromium.chrome.browser.LuxxleRelaunchUtils;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.chrome.browser.preferences.LuxxlePrefServiceBridge;
import org.chromium.chrome.browser.shields.FilterListServiceFactory;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.components.user_prefs.UserPrefs;

/* Class for Media section of main preferences */
public class MediaPreferences extends LuxxlePreferenceFragment
        implements Preference.OnPreferenceChangeListener {
    public static final String PREF_WIDEVINE_ENABLED = "widevine_enabled";
    public static final String PREF_BACKGROUND_VIDEO_PLAYBACK = "background_video_playback";
    public static final String PLAY_YT_VIDEO_IN_BROWSER_KEY = "play_yt_video_in_browser";
    private static final String PREF_HIDE_YOUTUBE_RECOMMENDED_CONTENT =
            "hide_youtube_recommended_content";
    private static final String PREF_HIDE_YOUTUBE_DISTRACTING_ELEMENTS =
            "hide_youtube_distracting_elements";
    private static final String PREF_HIDE_YOUTUBE_SHORTS = "hide_youtube_shorts";

    private FilterListAndroidHandler mFilterListAndroidHandler;

    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mPageTitle.set(getString(R.string.prefs_media));
        SettingsUtils.addPreferencesFromResource(this, R.xml.media_preferences);

        ChromeSwitchPreference backgroundVideoPlaybackPref =
                findPreference(PREF_BACKGROUND_VIDEO_PLAYBACK);
        if (backgroundVideoPlaybackPref != null) {
            backgroundVideoPlaybackPref.setVisible(
                    ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_BACKGROUND_VIDEO_PLAYBACK));
        }
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        initFilterListAndroidHandler();

        ChromeSwitchPreference enableWidevinePref =
                (ChromeSwitchPreference) findPreference(PREF_WIDEVINE_ENABLED);
        if (enableWidevinePref != null) {
            enableWidevinePref.setChecked(
                    LuxxleLocalState.get().getBoolean(LuxxlePref.WIDEVINE_ENABLED));
            enableWidevinePref.setOnPreferenceChangeListener(this);
        }

        ChromeSwitchPreference backgroundVideoPlaybackPref =
                findPreference(PREF_BACKGROUND_VIDEO_PLAYBACK);
        if (backgroundVideoPlaybackPref != null
                && ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_BACKGROUND_VIDEO_PLAYBACK)) {
            backgroundVideoPlaybackPref.setOnPreferenceChangeListener(this);
            backgroundVideoPlaybackPref.setChecked(
                    UserPrefs.get(getProfile())
                            .getBoolean(LuxxlePref.BACKGROUND_VIDEO_PLAYBACK_ENABLED));
        }

        ChromeSwitchPreference openYoutubeLinksLuxxlePref =
                (ChromeSwitchPreference) findPreference(PLAY_YT_VIDEO_IN_BROWSER_KEY);
        if (openYoutubeLinksLuxxlePref != null) {
            // Initially enabled.
            openYoutubeLinksLuxxlePref.setChecked(
                    LuxxlePrefServiceBridge.getInstance().getPlayYTVideoInBrowserEnabled());
            openYoutubeLinksLuxxlePref.setOnPreferenceChangeListener(this);
        }

        ChromeSwitchPreference hideYoutubeRecommendedContentPref =
                (ChromeSwitchPreference) findPreference(PREF_HIDE_YOUTUBE_RECOMMENDED_CONTENT);
        if (hideYoutubeRecommendedContentPref != null) {
            hideYoutubeRecommendedContentPref.setOnPreferenceChangeListener(this);
        }
        ChromeSwitchPreference hideYoutubeDistractingElementsPref =
                (ChromeSwitchPreference) findPreference(PREF_HIDE_YOUTUBE_DISTRACTING_ELEMENTS);
        if (hideYoutubeDistractingElementsPref != null) {
            hideYoutubeDistractingElementsPref.setOnPreferenceChangeListener(this);
        }
        ChromeSwitchPreference hideYoutubeShortsPref =
                (ChromeSwitchPreference) findPreference(PREF_HIDE_YOUTUBE_SHORTS);
        if (hideYoutubeShortsPref != null) {
            hideYoutubeShortsPref.setOnPreferenceChangeListener(this);
        }

        if (mFilterListAndroidHandler != null) {
            mFilterListAndroidHandler.isFilterListEnabled(
                    FilterListConstants.HIDE_YOUTUBE_RECOMMENDED_CONTENT_UUID,
                    isEnabled -> {
                        if (hideYoutubeRecommendedContentPref != null) {
                            hideYoutubeRecommendedContentPref.setChecked(isEnabled);
                        }
                    });
            mFilterListAndroidHandler.isFilterListEnabled(
                    FilterListConstants.HIDE_YOUTUBE_DISTRACTING_ELEMENTS_UUID,
                    isEnabled -> {
                        if (hideYoutubeDistractingElementsPref != null) {
                            hideYoutubeDistractingElementsPref.setChecked(isEnabled);
                        }
                    });
            mFilterListAndroidHandler.isFilterListEnabled(
                    FilterListConstants.HIDE_YOUTUBE_SHORTS_UUID,
                    isEnabled -> {
                        if (hideYoutubeShortsPref != null) {
                            hideYoutubeShortsPref.setChecked(isEnabled);
                        }
                    });
        }
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String key = preference.getKey();
        boolean shouldRelaunch = false;
        if (PREF_WIDEVINE_ENABLED.equals(key)) {
            LuxxleLocalState.get()
                    .setBoolean(
                            LuxxlePref.WIDEVINE_ENABLED,
                            !LuxxleLocalState.get().getBoolean(LuxxlePref.WIDEVINE_ENABLED));
            shouldRelaunch = true;
        } else if (PREF_BACKGROUND_VIDEO_PLAYBACK.equals(key)) {
            UserPrefs.get(getProfile())
                    .setBoolean(LuxxlePref.BACKGROUND_VIDEO_PLAYBACK_ENABLED, (boolean) newValue);
            shouldRelaunch = true;
        } else if (PLAY_YT_VIDEO_IN_BROWSER_KEY.equals(key)) {
            LuxxlePrefServiceBridge.getInstance().setPlayYTVideoInBrowserEnabled((boolean) newValue);
        } else if (PREF_HIDE_YOUTUBE_RECOMMENDED_CONTENT.equals(key)) {
            if (mFilterListAndroidHandler != null) {
                mFilterListAndroidHandler.enableFilter(
                        FilterListConstants.HIDE_YOUTUBE_RECOMMENDED_CONTENT_UUID,
                        (boolean) newValue);
            }
        } else if (PREF_HIDE_YOUTUBE_DISTRACTING_ELEMENTS.equals(key)) {
            if (mFilterListAndroidHandler != null) {
                mFilterListAndroidHandler.enableFilter(
                        FilterListConstants.HIDE_YOUTUBE_DISTRACTING_ELEMENTS_UUID,
                        (boolean) newValue);
            }
        } else if (PREF_HIDE_YOUTUBE_SHORTS.equals(key)) {
            if (mFilterListAndroidHandler != null) {
                mFilterListAndroidHandler.enableFilter(
                        FilterListConstants.HIDE_YOUTUBE_SHORTS_UUID, (boolean) newValue);
                shouldRelaunch = true;
            }
        }

        if (shouldRelaunch) {
            LuxxleRelaunchUtils.askForRelaunch(getActivity());
        }

        return true;
    }

    private void initFilterListAndroidHandler() {
        if (mFilterListAndroidHandler != null) {
            return;
        }

        mFilterListAndroidHandler =
                FilterListServiceFactory.getInstance()
                        .getFilterListAndroidHandler(getProfile(), null);
    }

    @Override
    public void onDestroy() {
        if (mFilterListAndroidHandler != null) {
            mFilterListAndroidHandler.close();
            mFilterListAndroidHandler = null;
        }
        super.onDestroy();
    }
}
