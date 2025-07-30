/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.playlist.settings;

import android.os.Bundle;

import androidx.preference.Preference;

import com.luxxle.playlist.local_database.PlaylistRepository;
import com.luxxle.playlist.util.PlaylistPreferenceUtils;

import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.ObservableSupplierImpl;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleRelaunchUtils;
import org.chromium.chrome.browser.playlist.PlaylistServiceFactoryAndroid;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.settings.LuxxlePreferenceFragment;
import org.chromium.components.browser_ui.settings.ChromeSwitchPreference;
import org.chromium.components.browser_ui.settings.SettingsUtils;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.playlist.mojom.PlaylistService;

public class LuxxlePlaylistPreferences extends LuxxlePreferenceFragment
        implements ConnectionErrorHandler, Preference.OnPreferenceChangeListener {
    private ChromeSwitchPreference mEnablePlaylistSwitch;
    private ChromeSwitchPreference mAddToPlaylistButtonSwitch;
    private ChromeSwitchPreference mRememberFilePlaybackPositionSwitch;
    private ChromeSwitchPreference mRememberListPlaybackPositionSwitch;
    private ChromeSwitchPreference mContinuousListeningSwitch;
    private LuxxlePlaylistResetPreference mResetPlaylist;

    private PlaylistService mPlaylistService;

    private final ObservableSupplierImpl<String> mPageTitle = new ObservableSupplierImpl<>();

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        mEnablePlaylistSwitch =
                (ChromeSwitchPreference) findPreference(LuxxlePreferenceKeys.PREF_ENABLE_PLAYLIST);
        mEnablePlaylistSwitch.setOnPreferenceChangeListener(this);

        mAddToPlaylistButtonSwitch =
                (ChromeSwitchPreference)
                        findPreference(LuxxlePreferenceKeys.PREF_ADD_TO_PLAYLIST_BUTTON);
        mAddToPlaylistButtonSwitch.setOnPreferenceChangeListener(this);

        mRememberFilePlaybackPositionSwitch =
                (ChromeSwitchPreference)
                        findPreference(LuxxlePreferenceKeys.PREF_REMEMBER_FILE_PLAYBACK_POSITION);
        mRememberListPlaybackPositionSwitch =
                (ChromeSwitchPreference)
                        findPreference(LuxxlePreferenceKeys.PREF_REMEMBER_LIST_PLAYBACK_POSITION);
        mContinuousListeningSwitch =
                (ChromeSwitchPreference)
                        findPreference(LuxxlePreferenceKeys.PREF_CONTINUOUS_LISTENING);

        mResetPlaylist =
                (LuxxlePlaylistResetPreference)
                        findPreference(LuxxlePreferenceKeys.PREF_RESET_PLAYLIST);
        mResetPlaylist.setOnPreferenceClickListener(
                preference -> {
                    if (mPlaylistService != null) {
                        PostTask.postTask(
                                TaskTraits.USER_VISIBLE_MAY_BLOCK,
                                () -> {
                                    mPlaylistService.clearAllQueries();
                                    mPlaylistService.resetAll();
                                    PlaylistRepository playlistRepository =
                                            new PlaylistRepository(getActivity());
                                    playlistRepository.deleteAllLastPlayedPosition();
                                    playlistRepository.deleteAllHlsContentQueueModel();
                                    PlaylistPreferenceUtils.resetPlaylistPrefs(getActivity());
                                    getActivity()
                                            .runOnUiThread(
                                                    (Runnable)
                                                            () ->
                                                                    LuxxleRelaunchUtils
                                                                            .askForRelaunch(
                                                                                    getActivity()));
                                });
                    }
                    return true;
                });

        updatePlaylistSettingsState(
                ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePreferenceKeys.PREF_ENABLE_PLAYLIST, true));
    }

    @Override
    public void onDestroy() {
        if (mPlaylistService != null) {
            mPlaylistService.close();
            mPlaylistService = null;
        }
        super.onDestroy();
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (mPlaylistService != null) {
            mPlaylistService.close();
            mPlaylistService = null;
        }
        mPlaylistService = null;
        initPlaylistService();
    }

    private void initPlaylistService() {
        if (mPlaylistService != null) {
            return;
        }

        mPlaylistService =
                PlaylistServiceFactoryAndroid.getInstance().getPlaylistService(getProfile(), this);
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        mPageTitle.set(getString(R.string.luxxle_playlist));
        SettingsUtils.addPreferencesFromResource(this, R.xml.luxxle_playlist_preferences);
        initPlaylistService();
    }

    @Override
    public ObservableSupplier<String> getPageTitle() {
        return mPageTitle;
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String key = preference.getKey();
        if (LuxxlePreferenceKeys.PREF_ENABLE_PLAYLIST.equals(key)) {
            updatePlaylistSettingsState((boolean) newValue);
        }

        return true;
    }

    private void updatePlaylistSettingsState(boolean isPlaylistEnabled) {
        if (isPlaylistEnabled) {
            if (mAddToPlaylistButtonSwitch != null) {
                getPreferenceScreen().addPreference(mAddToPlaylistButtonSwitch);
            }
            if (mRememberFilePlaybackPositionSwitch != null) {
                getPreferenceScreen().addPreference(mRememberFilePlaybackPositionSwitch);
            }
            if (mRememberListPlaybackPositionSwitch != null) {
                getPreferenceScreen().addPreference(mRememberListPlaybackPositionSwitch);
            }
            if (mContinuousListeningSwitch != null) {
                getPreferenceScreen().addPreference(mContinuousListeningSwitch);
            }
            if (mResetPlaylist != null) {
                getPreferenceScreen().addPreference(mResetPlaylist);
            }
        } else {
            if (mAddToPlaylistButtonSwitch != null) {
                getPreferenceScreen().removePreference(mAddToPlaylistButtonSwitch);
            }
            if (mRememberFilePlaybackPositionSwitch != null) {
                getPreferenceScreen().removePreference(mRememberFilePlaybackPositionSwitch);
            }
            if (mRememberListPlaybackPositionSwitch != null) {
                getPreferenceScreen().removePreference(mRememberListPlaybackPositionSwitch);
            }
            if (mContinuousListeningSwitch != null) {
                getPreferenceScreen().removePreference(mContinuousListeningSwitch);
            }
            if (mResetPlaylist != null) {
                getPreferenceScreen().removePreference(mResetPlaylist);
            }
        }
    }
}
