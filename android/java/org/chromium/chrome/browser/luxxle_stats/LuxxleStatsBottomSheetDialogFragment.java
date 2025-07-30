/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.luxxle_stats;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Dialog;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewParent;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.google.android.material.bottomsheet.BottomSheetDialogFragment;

import org.chromium.base.BuildInfo;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.local_database.DatabaseHelper;
import org.chromium.chrome.browser.night_mode.GlobalNightModeStateProviderHolder;
import org.chromium.chrome.browser.notifications.LuxxlePermissionUtils;
import org.chromium.chrome.browser.util.LuxxleTouchUtils;
import org.chromium.ui.base.DeviceFormFactor;

import java.util.List;

public class LuxxleStatsBottomSheetDialogFragment extends BottomSheetDialogFragment {
    public static final String TAG_FRAGMENT = "LUXXLESTATS_FRAG";
    private DatabaseHelper mDatabaseHelper = DatabaseHelper.getInstance();

    private static final int WEBSITES = 0;
    private static final int TRACKERS = 1;

    private static final int DAYS_7 = -7;
    private static final int DAYS_30 = -30;
    private static final int DAYS_90 = -90;

    private TextView adsTrackersCountText;
    private TextView adsTrackersText;
    private TextView dataSavedCountText;
    private TextView dataSavedText;
    private TextView timeSavedCountText;
    private TextView timeSavedText;
    private TextView noDataText;
    private TextView luxxleStatsSubSectionText;
    private LinearLayout emptyDataLayout;
    private LinearLayout websitesLayout;
    private LinearLayout trackersLayout;

    private RadioButton mWeekRadioButton;
    private RadioButton mMonthRadioButton;
    private RadioButton mMonthsRadioButton;
    private RadioButton mTrackersRadioButton;
    private RadioButton mWebsitesRadioButton;
    private View statsNotificationView;

    private int selectedType = WEBSITES;
    private int selectedDuration = DAYS_7;

    private Context mContext;

    public static LuxxleStatsBottomSheetDialogFragment newInstance() {
        return new LuxxleStatsBottomSheetDialogFragment();
    }

    @Override
    @SuppressLint("SourceLockedOrientationActivity")
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setStyle(STYLE_NORMAL, R.style.AppBottomSheetDialogTheme);
        mContext = ContextUtils.getApplicationContext();
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
    }

    @Override
    public void show(FragmentManager manager, String tag) {
        try {
            LuxxleStatsBottomSheetDialogFragment fragment = (LuxxleStatsBottomSheetDialogFragment) manager.findFragmentByTag(LuxxleStatsBottomSheetDialogFragment.TAG_FRAGMENT);
            FragmentTransaction transaction = manager.beginTransaction();
            if (fragment != null) {
                transaction.remove(fragment);
            }
            transaction.add(this, tag);
            transaction.commitAllowingStateLoss();
        } catch (IllegalStateException e) {
            Log.e("LuxxleStatsBottomSheetDialogFragment", e.getMessage());
        }
    }

    @Override
    public void setupDialog(Dialog dialog, int style) {
        super.setupDialog(dialog, style);

        final View view = LayoutInflater.from(getContext()).inflate(R.layout.luxxle_stats_bottom_sheet, null);

        emptyDataLayout = view.findViewById(R.id.luxxle_stats_empty_layout);

        RadioGroup durationRadioGroup = view.findViewById(R.id.duration_radio_group);
        durationRadioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, int checkedId) {
                if (checkedId == R.id.week_radio) {
                    selectedDuration = DAYS_7;
                } else if (checkedId == R.id.month_radio) {
                    selectedDuration = DAYS_30;
                } else if (checkedId == R.id.months_radio) {
                    selectedDuration = DAYS_90;
                }
                updateLuxxleStatsLayoutAsync();
            }
        });

        mWeekRadioButton = view.findViewById(R.id.week_radio);
        mMonthRadioButton = view.findViewById(R.id.month_radio);
        mMonthsRadioButton = view.findViewById(R.id.months_radio);
        mTrackersRadioButton = view.findViewById(R.id.trackers_radio);
        mWebsitesRadioButton = view.findViewById(R.id.websites_radio);

        LinearLayout layout = view.findViewById(R.id.luxxle_stats_layout);
        adsTrackersCountText = layout.findViewById(R.id.ads_trackers_count_text);
        adsTrackersText = layout.findViewById(R.id.ads_trackers_text);
        dataSavedCountText = layout.findViewById(R.id.data_saved_count_text);
        dataSavedText = layout.findViewById(R.id.data_saved_text);
        timeSavedCountText = layout.findViewById(R.id.time_saved_count_text);
        timeSavedText = layout.findViewById(R.id.time_saved_text);
        websitesLayout = layout.findViewById(R.id.wesites_layout);
        trackersLayout = layout.findViewById(R.id.trackers_layout);
        luxxleStatsSubSectionText = layout.findViewById(R.id.luxxle_stats_sub_section_text);
        statsNotificationView = view.findViewById(R.id.luxxle_stats_notification_permission);

        LuxxleTouchUtils.ensureMinTouchTarget(mWeekRadioButton);
        LuxxleTouchUtils.ensureMinTouchTarget(mMonthRadioButton);
        LuxxleTouchUtils.ensureMinTouchTarget(mMonthsRadioButton);
        LuxxleTouchUtils.ensureMinTouchTarget(mTrackersRadioButton);
        LuxxleTouchUtils.ensureMinTouchTarget(mWebsitesRadioButton);

        RadioGroup statTypeRadioGroup = layout.findViewById(R.id.stat_type_radio_group);
        statTypeRadioGroup.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(RadioGroup radioGroup, int checkedId) {
                if (checkedId == R.id.websites_radio) {
                    selectedType = WEBSITES;
                } else if (checkedId == R.id.trackers_radio) {
                    selectedType = TRACKERS;
                }
                showWebsitesTrackers();
            }
        });

        noDataText = layout.findViewById(R.id.empty_data_text);
        ImageView btnClose = view.findViewById(R.id.luxxle_stats_bottom_sheet_close);
        btnClose.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dismiss();
            }
        });
        updateLuxxleStatsLayoutAsync();
        updateNotificationView(view);

        dialog.setContentView(view);
        ViewParent parent = view.getParent();
        ((View)parent).getLayoutParams().height = ViewGroup.LayoutParams.MATCH_PARENT;

    }

    public void onRequestPermissionsResult(
            int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            statsNotificationView.setVisibility(View.GONE);
        }
    }

    @Override
    public void onResume() {
        super.onResume();
        if (!LuxxlePermissionUtils.hasPermission(
                        getContext(), Manifest.permission.POST_NOTIFICATIONS)
                || LuxxlePermissionUtils.isGeneralNotificationPermissionBlocked(getActivity())) {
            statsNotificationView.setVisibility(View.VISIBLE);
        } else {
            statsNotificationView.setVisibility(View.GONE);
        }
    }

    private void updateNotificationView(View view) {
        ImageView btnDismiss = view.findViewById(R.id.button_dismiss);
        btnDismiss.setOnClickListener(
                v -> {
                    statsNotificationView.setVisibility(View.GONE);
                });
        View notificationOnButton = view.findViewById(R.id.notification_on_button);
        notificationOnButton.setOnClickListener(
                v -> {
                    if (LuxxlePermissionUtils.isGeneralNotificationPermissionBlocked(getActivity())
                            || getActivity()
                                    .shouldShowRequestPermissionRationale(
                                            Manifest.permission.POST_NOTIFICATIONS)
                            || (Build.VERSION.SDK_INT < Build.VERSION_CODES.TIRAMISU
                                    || !BuildInfo.targetsAtLeastT())) {
                        // other than android 13 redirect to
                        // setting page and for android 13 Last time don't allow selected in
                        // permission
                        // dialog, then enable through setting
                        LuxxlePermissionUtils.notificationSettingPage(getContext());
                    } else {
                        // 1st time request permission
                        ActivityCompat.requestPermissions(
                                getActivity(),
                                new String[] {Manifest.permission.POST_NOTIFICATIONS},
                                1);
                    }
                });
    }

    @Override
    public void onDestroyView() {
        getActivity().setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
        super.onDestroyView();
    }

    private void updateLuxxleStatsLayoutAsync() {
        new AsyncTask<Void>() {
            long adsTrackersCount;
            long totalSavedBandwidth;
            long adsTrackersCountToCheckForMonth;
            long adsTrackersCountToCheckFor3Month;

            @Override
            protected Void doInBackground() {
                adsTrackersCount =
                        mDatabaseHelper
                                .getAllStatsWithDate(
                                        LuxxleStatsUtil.getCalculatedDate(
                                                "yyyy-MM-dd", selectedDuration),
                                        LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", 0))
                                .size();
                totalSavedBandwidth =
                        mDatabaseHelper.getTotalSavedBandwidthWithDate(
                                LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", selectedDuration),
                                LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                adsTrackersCountToCheckForMonth =
                        mDatabaseHelper
                                .getAllStatsWithDate(
                                        LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", DAYS_30),
                                        LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", DAYS_7))
                                .size();
                adsTrackersCountToCheckFor3Month =
                        mDatabaseHelper
                                .getAllStatsWithDate(
                                        LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", DAYS_90),
                                        LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", DAYS_30))
                                .size();
                return null;
            }

            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
                Pair<String, String> adsTrackersPair =
                        LuxxleStatsUtil.getLuxxleStatsStringFormNumberPair(adsTrackersCount, false);
                adsTrackersCountText.setText(
                        String.format(
                                mContext.getResources().getString(R.string.ntp_stat_text),
                                adsTrackersPair.first,
                                adsTrackersPair.second));

                Pair<String, String> dataSavedPair =
                        LuxxleStatsUtil.getLuxxleStatsStringFormNumberPair(totalSavedBandwidth, true);
                dataSavedCountText.setText(dataSavedPair.first);
                boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(mContext);
                if (isTablet) {
                    adsTrackersText.setText(
                            String.format(
                                    mContext.getResources().getString(R.string.trackers_and_ads),
                                    dataSavedPair.second));
                    dataSavedText.setText(
                            String.format(
                                    mContext.getResources()
                                            .getString(R.string.data_saved_tablet_text),
                                    dataSavedPair.second));
                } else {
                    adsTrackersText.setText(
                            String.format(
                                    mContext.getResources().getString(R.string.ads_trackers_text),
                                    dataSavedPair.second));
                    dataSavedText.setText(
                            String.format(
                                    mContext.getResources().getString(R.string.data_saved_text),
                                    dataSavedPair.second));
                }

                long timeSavedCount = adsTrackersCount * LuxxleStatsUtil.MILLISECONDS_PER_ITEM;
                Pair<String, String> timeSavedPair =
                        LuxxleStatsUtil.getLuxxleStatsStringFromTime(timeSavedCount / 1000);
                timeSavedCountText.setText(
                        String.format(
                                mContext.getResources().getString(R.string.ntp_stat_text),
                                timeSavedPair.first,
                                timeSavedPair.second));
                timeSavedText.setText(mContext.getResources().getString(R.string.time_saved_text));

                if (adsTrackersCount > 0) {
                    emptyDataLayout.setVisibility(View.GONE);
                } else {
                    emptyDataLayout.setVisibility(View.VISIBLE);
                }

                // Check for month option
                if (adsTrackersCountToCheckForMonth > 0) {
                    mMonthRadioButton.setEnabled(true);
                    mMonthRadioButton.setAlpha(1.0f);
                } else {
                    mMonthRadioButton.setEnabled(false);
                    mMonthRadioButton.setAlpha(0.2f);
                }

                // Check for 3 month option
                if (adsTrackersCountToCheckFor3Month > 0) {
                    mMonthsRadioButton.setEnabled(true);
                    mMonthsRadioButton.setAlpha(1.0f);
                } else {
                    mMonthsRadioButton.setEnabled(false);
                    mMonthsRadioButton.setAlpha(0.2f);
                }
                showWebsitesTrackers();
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void showWebsitesTrackers() {
        new AsyncTask<Void>() {
            List<Pair<String, Integer>> websiteTrackers;

            @Override
            protected Void doInBackground() {
                if (selectedType == WEBSITES) {
                    websiteTrackers =
                            mDatabaseHelper.getStatsWithDate(
                                    LuxxleStatsUtil.getCalculatedDate(
                                            "yyyy-MM-dd", selectedDuration),
                                    LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                } else {
                    websiteTrackers =
                            mDatabaseHelper.getSitesWithDate(
                                    LuxxleStatsUtil.getCalculatedDate(
                                            "yyyy-MM-dd", selectedDuration),
                                    LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                }
                return null;
            }

            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
                LinearLayout rootView = null;
                if (selectedType == WEBSITES) {
                    websitesLayout.setVisibility(View.VISIBLE);
                    trackersLayout.setVisibility(View.GONE);
                    rootView = websitesLayout;
                } else {
                    websitesLayout.setVisibility(View.GONE);
                    trackersLayout.setVisibility(View.VISIBLE);
                    rootView = trackersLayout;
                }

                rootView.removeAllViews();

                if (websiteTrackers.size() > 0) {
                    for (Pair<String, Integer> statPair : websiteTrackers) {
                        LayoutInflater inflater = LayoutInflater.from(mContext);
                        ViewGroup layout =
                                (ViewGroup) inflater.inflate(R.layout.tracker_item_layout, null);

                        TextView mTrackerCountText =
                                (TextView) layout.findViewById(R.id.tracker_count_text);
                        TextView mSiteText = (TextView) layout.findViewById(R.id.site_text);

                        mTrackerCountText.setText(String.valueOf(statPair.second));
                        mSiteText.setText(statPair.first);
                        if (GlobalNightModeStateProviderHolder.getInstance().isInNightMode()) {
                            mSiteText.setTextColor(
                                    mContext.getColor(R.color.luxxle_stats_text_dark_color));
                            mTrackerCountText.setTextColor(
                                    mContext.getColor(R.color.luxxle_stats_text_dark_color));
                        } else {
                            mSiteText.setTextColor(
                                    mContext.getColor(R.color.luxxle_stats_text_light_color));
                            mTrackerCountText.setTextColor(
                                    mContext.getColor(R.color.luxxle_stats_text_light_color));
                        }

                        rootView.addView(layout);
                    }
                    noDataText.setVisibility(View.GONE);
                    luxxleStatsSubSectionText.setVisibility(View.VISIBLE);
                } else {
                    noDataText.setVisibility(View.VISIBLE);
                    luxxleStatsSubSectionText.setVisibility(View.GONE);
                }
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }
}
