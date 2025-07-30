/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.browsing_data;

import android.os.Bundle;
import android.text.SpannableString;
import android.view.View;

import org.chromium.base.Callback;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleAdsNativeHelper;
import org.chromium.chrome.browser.LuxxleRewardsHelper;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.luxxle_leo.LuxxleLeoMojomHelper;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.util.TabUtils;
import org.chromium.components.browser_ui.settings.ClickableSpansTextMessagePreference;
import org.chromium.components.browser_ui.settings.SpinnerPreference;
import org.chromium.ui.text.ChromeClickableSpan;
import org.chromium.ui.text.SpanApplier;
import org.chromium.ui.text.SpanApplier.SpanInfo;

public class LuxxleClearBrowsingDataFragment extends ClearBrowsingDataFragment {
    ClearBrowsingDataCheckBoxPreference mClearAIChatDataCheckBoxPreference;

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        super.onCreatePreferences(savedInstanceState, rootKey);

        getPreferenceScreen().addPreference(buildClearLeoAIHistory());
        getPreferenceScreen()
                .addPreference(
                        LuxxleRewardsHelper.isRewardsEnabled()
                                ? buildResetLuxxleRewardsDataPref()
                                : buildClearLuxxleAdsDataPref());
    }

    private ClearBrowsingDataCheckBoxPreference buildClearLeoAIHistory() {
        mClearAIChatDataCheckBoxPreference =
                new ClearBrowsingDataCheckBoxPreference(getContext(), null);
        mClearAIChatDataCheckBoxPreference.setTitle(R.string.luxxle_clear_ai_history_title);
        mClearAIChatDataCheckBoxPreference.setSummary(R.string.luxxle_clear_ai_history_summary);
        mClearAIChatDataCheckBoxPreference.setIcon(R.drawable.ic_luxxle_ai);

        return mClearAIChatDataCheckBoxPreference;
    }

    private ClickableSpansTextMessagePreference buildResetLuxxleRewardsDataPref() {
        SpannableString resetLuxxleRewardsDataText =
                SpanApplier.applySpans(
                        getContext().getString(R.string.reset_luxxle_rewards_data),
                        new SpanInfo(
                                "<link1>",
                                "</link1>",
                                new ChromeClickableSpan(
                                        requireContext(), resetLuxxleRewardsDataCallback())));

        ClickableSpansTextMessagePreference resetLuxxleRewardsDataPref =
                new ClickableSpansTextMessagePreference(getContext(), null);
        resetLuxxleRewardsDataPref.setSummary(resetLuxxleRewardsDataText);
        return resetLuxxleRewardsDataPref;
    }

    private ClickableSpansTextMessagePreference buildClearLuxxleAdsDataPref() {
        SpannableString clearLuxxleAdsDataText =
                SpanApplier.applySpans(
                        getContext().getString(R.string.clear_luxxle_ads_data),
                        new SpanInfo(
                                "<link1>",
                                "</link1>",
                                new ChromeClickableSpan(
                                        requireContext(), clearLuxxleAdsDataCallback())));

        ClickableSpansTextMessagePreference clearLuxxleAdsDataPref =
                new ClickableSpansTextMessagePreference(getContext(), null);
        clearLuxxleAdsDataPref.setSummary(clearLuxxleAdsDataText);
        return clearLuxxleAdsDataPref;
    }

    private Callback<View> resetLuxxleRewardsDataCallback() {
        return (view) -> {
            try {
                TabUtils.openUrlInNewTab(false, LuxxleActivity.LUXXLE_REWARDS_RESET_PAGE);
                TabUtils.bringChromeTabbedActivityToTheTop(LuxxleActivity.getLuxxleActivity());
            } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            }
        };
    }

    private Callback<View> clearLuxxleAdsDataCallback() {
        return (view) -> {
            Profile profile = getProfile();
            if (profile != null) {
                LuxxleAdsNativeHelper.nativeClearData(profile);
            }

            if (getActivity() != null) {
                getActivity().finish();
            }
        };
    }

    @Override
    protected void onClearBrowsingData() {
        super.onClearBrowsingData();

        if (mClearAIChatDataCheckBoxPreference != null
                && mClearAIChatDataCheckBoxPreference.isChecked()) {
            Profile profile = getProfile();
            if (profile == null) {
                return;
            }
            Object spinnerSelection =
                    ((SpinnerPreference) findPreference(PREF_TIME_RANGE)).getSelectedOption();
            @TimePeriod
            int lastSelectedTimePeriod =
                    ((TimePeriodUtils.TimePeriodSpinnerOption) spinnerSelection).getTimePeriod();

            LuxxleLeoMojomHelper.getInstance(profile).deleteConversations(lastSelectedTimePeriod);
        }
    }
}
