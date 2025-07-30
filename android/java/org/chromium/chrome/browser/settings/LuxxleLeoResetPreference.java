/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.settings;

import android.content.Context;
import android.content.DialogInterface;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.preference.Preference;
import androidx.preference.PreferenceViewHolder;

import org.chromium.base.Log;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.luxxle_leo.LuxxleLeoMojomHelper;
import org.chromium.chrome.browser.browsing_data.TimePeriod;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.components.user_prefs.UserPrefs;

/**
 * The preference used to reset Luxxle Leo.
 */
public class LuxxleLeoResetPreference
        extends Preference implements Preference.OnPreferenceClickListener {
    private static final String TAG = "LuxxleLeoResetPref";

    /**
     * Constructor for LuxxleLeoResetPreference.
     */
    public LuxxleLeoResetPreference(Context context, AttributeSet attrs) {
        super(context, attrs);

        setOnPreferenceClickListener(this);
    }

    @Override
    public void onBindViewHolder(@NonNull PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);
        TextView titleView = (TextView) holder.findViewById(android.R.id.title);
        assert titleView != null;
        titleView.setTextAppearance(R.style.LuxxleLeoResetTextColor);
    }

    @Override
    public boolean onPreferenceClick(@NonNull Preference preference) {
        showLuxxleLeoResetDialog();
        return true;
    }

    private void showLuxxleLeoResetDialog() {
        LayoutInflater inflater =
                (LayoutInflater) getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = inflater.inflate(R.layout.luxxle_leo_reset_dialog, null);

        DialogInterface.OnClickListener onClickListener =
                (dialog, button) -> {
                    if (button == AlertDialog.BUTTON_POSITIVE) {
                        Profile profile = null;
                        try {
                            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
                            profile = activity.getCurrentProfile();
                        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
                            Log.e(TAG, "get LuxxleActivity exception", e);
                        }
                        if (profile == null) {
                            Log.e(TAG, "showLuxxleLeoResetDialog profile is null");
                            return;
                        }
                        UserPrefs.get(profile).clearPref(LuxxlePref.LAST_ACCEPTED_DISCLAIMER);
                        LuxxleLeoMojomHelper.getInstance(profile)
                                .deleteConversations(TimePeriod.ALL_TIME);
                    } else {
                        dialog.dismiss();
                    }
                };

        AlertDialog.Builder alert =
                new AlertDialog.Builder(getContext(), R.style.ThemeOverlay_BrowserUI_AlertDialog);
        AlertDialog alertDialog =
                alert.setTitle(R.string.leo_reset_data)
                        .setView(view)
                        .setPositiveButton(R.string.luxxle_leo_confirm_text, onClickListener)
                        .setNegativeButton(R.string.cancel, onClickListener)
                        .create();
        alertDialog.getDelegate().setHandleNativeActionModesEnabled(false);
        alertDialog.show();
    }
}
