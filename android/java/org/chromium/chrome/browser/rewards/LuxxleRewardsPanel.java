/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.rewards;

import static org.chromium.build.NullUtil.assumeNonNull;

import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffColorFilter;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.text.Html;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.method.LinkMovementMethod;
import android.text.style.UnderlineSpan;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.webkit.URLUtil;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.TextView;

import androidx.appcompat.widget.AppCompatImageView;
import androidx.cardview.widget.CardView;
import androidx.core.content.ContextCompat;
import androidx.core.content.res.ResourcesCompat;
import androidx.viewpager.widget.ViewPager;

import com.google.android.material.tabs.TabLayout;

import org.json.JSONException;

import org.chromium.base.LuxxleReflectionUtil;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.SysUtils;
import org.chromium.luxxle_rewards.mojom.PublisherStatus;
import org.chromium.luxxle_rewards.mojom.UserType;
import org.chromium.luxxle_rewards.mojom.WalletStatus;
import org.chromium.build.annotations.NullMarked;
import org.chromium.build.annotations.Nullable;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleAdsNativeHelper;
import org.chromium.chrome.browser.LuxxleRewardsBalance;
import org.chromium.chrome.browser.LuxxleRewardsExternalWallet;
import org.chromium.chrome.browser.LuxxleRewardsHelper;
import org.chromium.chrome.browser.LuxxleRewardsNativeWorker;
import org.chromium.chrome.browser.LuxxleRewardsObserver;
import org.chromium.chrome.browser.LuxxleRewardsOnboardingPagerAdapter;
import org.chromium.chrome.browser.LuxxleRewardsUserWalletActivity;
import org.chromium.chrome.browser.LuxxleWalletProvider;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.custom_layout.HeightWrappingViewPager;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.notifications.LuxxleNotificationWarningDialog;
import org.chromium.chrome.browser.notifications.LuxxlePermissionUtils;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.chrome.browser.profiles.ProfileManager;
import org.chromium.chrome.browser.rewards.onboarding.RewardsOnboarding;
import org.chromium.chrome.browser.rewards.tipping.PopupWindowTippingTabletUI;
import org.chromium.chrome.browser.rewards.tipping.RewardsTippingBannerActivity;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.util.LuxxleConstants;
import org.chromium.chrome.browser.util.LuxxleTouchUtils;
import org.chromium.chrome.browser.util.ConfigurationUtils;
import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.util.TabUtils;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.ui.base.DeviceFormFactor;
import org.chromium.ui.text.ChromeClickableSpan;
import org.chromium.ui.text.SpanApplier;
import org.chromium.ui.text.SpanApplier.SpanInfo;

import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

@NullMarked
public class LuxxleRewardsPanel
        implements LuxxleRewardsObserver, LuxxleRewardsHelper.LargeIconReadyCallback {
    public static final String PREF_WAS_LUXXLE_REWARDS_TURNED_ON = "luxxle_rewards_turned_on";
    public static final String PREF_ON_BOARDING_COMPLETED = "on_boarding_completed";
    public static final String REWARDS_TOUR_URL = "http://luxxle.com/rewards";
    public static final String PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED =
            "was_toolbar_bat_logo_button_pressed";
    private static final String UNVERIFIED_USER_UNSUPPORTED_REGION_PAGE =
            "https://support.luxxle.com/hc/en-us/articles/6539887971469";
    public static final String NEW_SIGNUP_DISABLED_URL =
            "https://support.luxxle.com/hc/en-us/articles/9312922941069";
    private static final String ADS_PAYOUT_STATUS_URL =
            "https://community.luxxle.com/t/ads-payout-status-update/287895";
    private static final String SUPPORT_URL = "https://support.luxxle.com";
    private static final String LUXXLE_REWARDS_PAGE = "https://luxxle.com/rewards";
    private static final String LUXXLE_REWARDS_CHANGES_PAGE = "https://luxxle.com/rewards-changes";

    private static final String TAG = "LuxxleRewards";
    private static final int PUBLISHER_INFO_FETCH_RETRY = 3 * 1000; // In milliseconds
    private static final int PUBLISHER_FETCHES_COUNT = 3;

    private static final String YOUTUBE_TYPE = "youtube#";
    private static final String TWITCH_TYPE = "twitch#";

    // Balance report codes
    private static final int BALANCE_REPORT_EARNING_FROM_ADS = 0;
    private static final int BALANCE_REPORT_AUTO_CONTRIBUTE = 1;
    private static final int BALANCE_REPORT_RECURRING_DONATION = 2;
    private static final int BALANCE_REPORT_ONE_TIME_DONATION = 3;

    // Custom Android notification
    private static final int REWARDS_NOTIFICATION_NO_INTERNET = 1000;
    private static final int REWARDS_PROMOTION_CLAIM_ERROR = REWARDS_NOTIFICATION_NO_INTERNET + 1;
    private static final String REWARDS_PROMOTION_CLAIM_ERROR_ID =
            "rewards_promotion_claim_error_id";

    // Auto contribute results
    private static final String AUTO_CONTRIBUTE_SUCCESS = "0";
    private static final String AUTO_CONTRIBUTE_GENERAL_ERROR = "1";
    private static final String ERROR_CONVERT_PROBI = "ERROR";
    public static final String WALLET_GENERATION_DISABLED_ERROR = "wallet-generation-disabled";

    // Payout status
    private static final String PAYOUT_STATUS_PENDING = "off";
    private static final String PAYOUT_STATUS_PROCESSING = "processing";
    private static final String PAYOUT_STATUS_COMPLETE = "complete";

    private static final String SELF_CUSTODY_AVAILABLE = "self_custody_available";
    private static final String WALLET_DISCONNECTED = "wallet_disconnected";
    private static final String UPHOLD_BAT_NOT_ALLOWED = "uphold_bat_not_allowed";
    private static final String UPHOLD_INSUFFICIENT_CAPABILITIES =
            "uphold_insufficient_capabilities";

    public enum NotificationClickAction {
        DO_NOTHING,
        RECONNECT,
        CLAIM,
        TURN_ON_ADS;
    }

    private boolean mShouldShowOnboardingForConnectAccount;

    private final View mAnchorView;
    private final PopupWindow mPopupWindow;
    private ViewGroup mPopupView;
    private FrameLayout mLuxxlePanelShadow;
    private LinearLayout mRewardsMainLayout;
    private @Nullable LuxxleActivity mLuxxleActivity;
    private final ChromeTabbedActivity mActivity;
    private LuxxleRewardsHelper mIconFetcher;

    private LinearLayout mRewardsSummaryDetailLayout;
    private LinearLayout mPayoutStatusBannerLayout;
    private CardView mRewardsTipLayout;
    private @Nullable LinearLayout mBtnTip;
    private @Nullable ImageView mImgTip;
    private @Nullable TextView mTextTip;
    private @Nullable LinearLayout mBtnSummary;
    private @Nullable ImageView mImgSummary;
    private @Nullable TextView mTextSummary;

    private @Nullable Timer mPublisherFetcher;
    private int mPublisherFetchesCount;
    private boolean mPublisherExist;

    private String mCurrentNotificationId;

    private LuxxleRewardsNativeWorker mLuxxleRewardsNativeWorker;

    private int mCurrentTabId;
    private @Nullable TextView mPublisherAttention;

    private @Nullable LuxxleRewardsExternalWallet mExternalWallet;

    private View mNotificationLayout;
    private View mNotificationPermissionLayout;
    private boolean mNotificationShown;

    private View mConnectAccountModal;
    private View mRewardsTosModal;
    private View mRewardsWalletBalance;

    private View mRewardsSolanaEligibleLayout;

    private @Nullable LuxxleRewardsOnboardingPagerAdapter mLuxxleRewardsOnboardingPagerAdapter;
    private @Nullable HeightWrappingViewPager mLuxxleRewardsViewPager;
    private View mLuxxleRewardsOnboardingView;
    private ViewGroup mLuxxleRewardsUnverifiedView;

    private View mWalletBalanceProgress;
    private @Nullable View mBalanceDataViewGroups;
    private @Nullable PopupWindowTippingTabletUI mPopupWindowTippingTabletUI;
    private boolean mIsTablet;

    public LuxxleRewardsPanel(View anchorView) {
        mCurrentNotificationId = "";
        mPublisherExist = false;
        mPublisherFetchesCount = 0;
        mCurrentTabId = -1;
        mAnchorView = anchorView;
        mPopupWindow = new PopupWindow(anchorView.getContext());
        mPopupWindow.setHeight(ViewGroup.LayoutParams.WRAP_CONTENT);
        mPopupWindow.setBackgroundDrawable(
                ResourcesCompat.getDrawable(ContextUtils.getApplicationContext().getResources(),
                        R.drawable.rewards_panel_background, null));

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            mPopupWindow.setElevation(20);
        }

        Tab tab = LuxxleRewardsHelper.currentActiveChromeTabbedActivityTab();
        if (tab == null) {
            throw new NullPointerException("Unexpected null tab");
        }

        mIconFetcher = new LuxxleRewardsHelper(tab);
        mPopupWindow.setOnDismissListener(
                new PopupWindow.OnDismissListener() {
                    @Override
                    public void onDismiss() {
                        if (mPublisherFetcher != null) {
                            mPublisherFetcher.cancel();
                        }

                        if (mIconFetcher != null) {
                            mIconFetcher.detach();
                        }

                        if (mLuxxleRewardsNativeWorker != null) {
                            mLuxxleRewardsNativeWorker.removeObserver(LuxxleRewardsPanel.this);
                        }

                        if (mCurrentTabId != -1 && mLuxxleRewardsNativeWorker != null) {
                            mLuxxleRewardsNativeWorker.removePublisherFromMap(mCurrentTabId);
                        }

                        if (mLuxxleActivity != null) {
                            mLuxxleActivity.onRewardsPanelDismiss();
                        }
                    }
                });
        try {
            mLuxxleActivity = LuxxleRewardsHelper.getLuxxleActivity();
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            mLuxxleActivity = null;
            Log.e(TAG, "LuxxleRewardsPanel constructor " + e);
        }
        mActivity = LuxxleRewardsHelper.getChromeTabbedActivity();
        mLuxxleRewardsNativeWorker = LuxxleRewardsNativeWorker.getInstance();
        if (mLuxxleRewardsNativeWorker != null) {
            mLuxxleRewardsNativeWorker.addObserver(this);
        }
        setUpViews();
    }

    public void showLikePopDownMenu() {
        mPopupWindow.setTouchable(true);
        mPopupWindow.setFocusable(true);
        mPopupWindow.setOutsideTouchable(true);

        int mesuredHeight = 0;
        if (BottomToolbarConfiguration.isToolbarBottomAnchored()) {
            mPopupView.measure(View.MeasureSpec.UNSPECIFIED, View.MeasureSpec.UNSPECIFIED);
            mesuredHeight = mPopupView.getMeasuredHeight() + mAnchorView.getHeight();
            mPopupView
                    .getViewTreeObserver()
                    .addOnGlobalLayoutListener(
                            new ViewTreeObserver.OnGlobalLayoutListener() {
                                @Override
                                public void onGlobalLayout() {
                                    // Get the new height of the popup view.
                                    int newHeight =
                                            mPopupView.getHeight() + mAnchorView.getHeight();

                                    // Update the popup window's height.
                                    mPopupWindow.update(
                                            mAnchorView,
                                            0,
                                            -newHeight,
                                            mPopupWindow.getWidth(),
                                            ViewGroup.LayoutParams.WRAP_CONTENT);
                                }
                            });
        }

        mPopupWindow.setAnimationStyle(
                BottomToolbarConfiguration.isToolbarTopAnchored()
                        ? R.style.AnchoredPopupAnimEndTop
                        : R.style.AnchoredPopupAnimEndBottom);

        if (SysUtils.isLowEndDevice()) {
            mPopupWindow.setAnimationStyle(0);
        }

        mPopupWindow.showAsDropDown(mAnchorView, 0, -1 * mesuredHeight);

        mLuxxleRewardsNativeWorker.getExternalWallet();
        mLuxxleRewardsNativeWorker.recordPanelTrigger();
    }

    private void setUpViews() {
        LayoutInflater inflater = (LayoutInflater) mAnchorView.getContext().getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        mPopupView = (ViewGroup) inflater.inflate(R.layout.luxxle_rewards_panel_layout, null);

        int deviceWidth = ConfigurationUtils.getDisplayMetricsWidth(mActivity);
        mIsTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(mActivity);

        mPopupWindow.setWidth((int) (mIsTablet ? (deviceWidth * 0.6) : (deviceWidth * 0.95)));

        mLuxxlePanelShadow = mPopupView.findViewById(R.id.panel_shadow);
        mLuxxleRewardsOnboardingView =
                mPopupView.findViewById(R.id.luxxle_rewards_onboarding_layout_id);
        mNotificationLayout =
                mPopupView.findViewById(R.id.luxxle_rewards_panel_notification_layout_id);
        mNotificationPermissionLayout =
                mPopupView.findViewById(R.id.luxxle_rewards_notification_permission_id);
        mConnectAccountModal = mPopupView.findViewById(R.id.connect_account_layout_id);
        mLuxxleRewardsUnverifiedView =
                mPopupView.findViewById(R.id.luxxle_rewards_panel_unverified_layout_id);
        mRewardsMainLayout = mPopupView.findViewById(R.id.rewards_main_layout);
        mRewardsSummaryDetailLayout =
                mPopupView.findViewById(R.id.luxxle_rewards_panel_summary_layout_id);
        mPayoutStatusBannerLayout =
                mPopupView.findViewById(R.id.luxxle_rewards_panel_payout_layout_id);
        mRewardsTipLayout = mPopupView.findViewById(R.id.luxxle_rewards_panel_tip_layout_id);

        mRewardsTosModal = mPopupView.findViewById(R.id.rewards_tos_layout_id);

        mRewardsWalletBalance = mPopupView.findViewById(R.id.wallet_balance_layout);

        mRewardsSolanaEligibleLayout =
                mPopupView.findViewById(R.id.luxxle_rewards_solana_eligible_ui_layout_id);
        mPopupWindow.setContentView(mPopupView);

        mWalletBalanceProgress = mPopupView.findViewById(R.id.wallet_progress_bar_group);
        adjustTouchTargets();
    }

    // Rewards main layout changes
    private void showRewardsMainUI() {
        if (mRewardsMainLayout == null) {
            return;
        }
        mRewardsMainLayout.setVisibility(View.VISIBLE);

        TextView loggedOutStateText = mPopupView.findViewById(R.id.logged_out_state_text);
        if (mExternalWallet != null) {
            loggedOutStateText.setText(String.format(
                    mActivity.getResources().getString(R.string.logged_out_state_dialog_text),
                    getWalletString(mExternalWallet.getType())));
        }
        mBalanceDataViewGroups = mPopupView.findViewById(R.id.balance_display_group);

        setVisibilityForLoggedOutState();
        loggedOutStateText.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        TabUtils.openUrlInNewTab(
                                false, LuxxleActivity.LUXXLE_REWARDS_WALLET_RECONNECT_URL);
                        dismiss();
                    }
                });

        ImageView btnRewardsSettings = mPopupView.findViewById(R.id.btn_rewards_settings);
        btnRewardsSettings.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (mLuxxleActivity != null) {
                            mLuxxleActivity.openNewOrSelectExistingTab(
                                    LuxxleActivity.LUXXLE_REWARDS_SETTINGS_URL);
                        }
                        dismiss();
                    }
                });

        TextView btnSendTip = mPopupView.findViewById(R.id.btn_send_tip);
        btnSendTip.setOnClickListener(
                view -> {
                    if (mIsTablet) {
                        mPopupWindowTippingTabletUI =
                                PopupWindowTippingTabletUI.newInstance(mAnchorView, mCurrentTabId);
                        if (mLuxxleActivity != null) {
                            mPopupWindowTippingTabletUI.show(
                                    mLuxxleActivity.getSupportFragmentManager(),
                                    "PopupWindowTippingTabletUITag");
                        }
                    } else {
                        Intent intent =
                                new Intent(
                                        ContextUtils.getApplicationContext(),
                                        RewardsTippingBannerActivity.class);
                        intent.putExtra(RewardsTippingBannerActivity.TAB_ID_EXTRA, mCurrentTabId);
                        mActivity.startActivityForResult(
                                intent, LuxxleConstants.SITE_BANNER_REQUEST_CODE);
                    }
                });

        mBtnTip = mPopupView.findViewById(R.id.tip_btn);
        mImgTip = mPopupView.findViewById(R.id.tip_img);
        mTextTip = mPopupView.findViewById(R.id.tip_text);

        mBtnTip.setOnClickListener(
                view -> {
                    showTipSection();
                });

        mBtnSummary = mPopupView.findViewById(R.id.summary_btn);
        mImgSummary = mPopupView.findViewById(R.id.summary_img);
        mTextSummary = mPopupView.findViewById(R.id.summary_text);
        mBtnSummary.setOnClickListener(
                view -> {
                    showSummarySection();
                });

        showSummarySection();
        mBtnTip.setEnabled(false);

        String monthName = (String) android.text.format.DateFormat.format("MMMM", new Date());
        int currentYear = Calendar.getInstance().get(Calendar.YEAR);
        String monthYear = monthName + " " + currentYear;
        TextView monthYearText = mPopupView.findViewById(R.id.month_year_text);
        monthYearText.setText(monthYear);
    }

    private void setVisibilityForLoggedOutState() {
        View balanceText = mPopupView.findViewById(R.id.balance_text);
        TextView loggedOutStateText = mPopupView.findViewById(R.id.logged_out_state_text);

        if (mExternalWallet != null && mExternalWallet.getStatus() == WalletStatus.LOGGED_OUT) {
            loggedOutStateText.setVisibility(View.VISIBLE);
            balanceText.setVisibility(View.GONE);
            mWalletBalanceProgress.setVisibility(View.GONE);
        } else {
            loggedOutStateText.setVisibility(View.GONE);
            balanceText.setVisibility(View.VISIBLE);
        }
    }

    private void showSummarySection() {
        assumeNonNull(mImgTip);
        assumeNonNull(mImgSummary);
        assumeNonNull(mTextSummary);
        assumeNonNull(mTextTip);

        mTextTip.setTextAppearance(R.style.LuxxleRewardsSecondaryTextColor);
        mImgTip.setColorFilter(
                new PorterDuffColorFilter(
                        ContextCompat.getColor(
                                mActivity, R.color.rewards_panel_secondary_text_color),
                        PorterDuff.Mode.SRC_IN));

        mTextSummary.setTextAppearance(R.style.LuxxleRewardsActionColor);
        mImgSummary.setColorFilter(
                new PorterDuffColorFilter(
                        ContextCompat.getColor(mActivity, R.color.rewards_panel_action_color),
                        PorterDuff.Mode.SRC_IN));

        int rewardsSummaryVisibility = View.VISIBLE;
        if (mExternalWallet != null
                && LuxxleWalletProvider.SOLANA.equals(mExternalWallet.getType())) {
            rewardsSummaryVisibility = View.GONE;
        }
        mRewardsSummaryDetailLayout.setVisibility(rewardsSummaryVisibility);
        mRewardsTipLayout.setVisibility(View.GONE);
    }

    private void showTipSection() {
        assumeNonNull(mImgSummary);
        assumeNonNull(mImgTip);
        assumeNonNull(mTextSummary);
        assumeNonNull(mTextTip);

        mTextTip.setTextAppearance(R.style.LuxxleRewardsActionColor);
        mImgTip.setColorFilter(
                new PorterDuffColorFilter(
                        ContextCompat.getColor(mActivity, R.color.rewards_panel_action_color),
                        PorterDuff.Mode.SRC_IN));

        mTextSummary.setTextAppearance(R.style.LuxxleRewardsSecondaryTextColor);
        mImgSummary.setColorFilter(
                new PorterDuffColorFilter(
                        ContextCompat.getColor(
                                mActivity, R.color.rewards_panel_secondary_text_color),
                        PorterDuff.Mode.SRC_IN));
        mRewardsSummaryDetailLayout.setVisibility(View.GONE);
        mRewardsTipLayout.setVisibility(View.VISIBLE);
    }

    // Notification changes
    private void setNotificationsControls() {
        if (mNotificationLayout == null) {
            return;
        }
        ImageView btnCloseNotification =
                mNotificationLayout.findViewById(R.id.btn_close_notification);
        if (btnCloseNotification != null) {
            btnCloseNotification.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            if (mCurrentNotificationId.isEmpty()) {
                                assert false;
                                return;
                            }
                            if (mLuxxleRewardsNativeWorker != null) {
                                mLuxxleRewardsNativeWorker.deleteNotification(
                                        mCurrentNotificationId);
                            }
                        }
                    });
        }
    }

    private boolean isValidNotificationType(int type, int argsNum) {
        boolean valid = false;
        switch (type) {
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_AUTO_CONTRIBUTE:
                valid = (argsNum >= 4);
                break;
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_VERIFIED_PUBLISHER:
                valid = (argsNum >= 1);
                break;
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_TIPS_PROCESSED:
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_ADS_ONBOARDING:
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_PENDING_NOT_ENOUGH_FUNDS:
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_GENERAL:
            case REWARDS_NOTIFICATION_NO_INTERNET:
            case REWARDS_PROMOTION_CLAIM_ERROR:
                valid = true;
                break;
            default:
                valid = false;
        }
        Log.i(TAG, "IsValidNotificationType: type %d argnum %d ", type, argsNum);
        return valid;
    }

    private void showNotification(String id, int type, String[] args) {
        if (mLuxxleRewardsNativeWorker == null) {
            return;
        }

        if (type == LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_GENERAL
                && args.length > 0
                && args[0].equals(SELF_CUSTODY_AVAILABLE)) {
            if (mLuxxleRewardsNativeWorker != null) {
                mLuxxleRewardsNativeWorker.deleteNotification(id);
            }
            return;
        }

        if (!isValidNotificationType(type, args.length)) {
            mLuxxleRewardsNativeWorker.deleteNotification(id);
            return;
        }

        mCurrentNotificationId = id;
        ImageView notificationClaimImg = mPopupView.findViewById(R.id.notification_claim_img);
        TextView notificationClaimSubText =
                mPopupView.findViewById(R.id.notification_claim_subtitle_text);
        TextView notificationDayText = mPopupView.findViewById(R.id.notification_day_text);
        String currentDay = (String) android.text.format.DateFormat.format("MMM dd", new Date());
        notificationDayText.setText(currentDay);

        TextView notificationTitleText = mPopupView.findViewById(R.id.notification_title_text);
        TextView notificationSubtitleText =
                mPopupView.findViewById(R.id.notification_subtitle_text);
        TextView actionNotificationButton = mPopupView.findViewById(R.id.btn_action_notification);

        String title = "";
        String description = "";
        int notificationIcon = R.drawable.ic_info_rewards;
        View claimProgress = mPopupView.findViewById(R.id.claim_progress);
        NotificationClickAction notificationClickAction = NotificationClickAction.DO_NOTHING;

        actionNotificationButton.setEnabled(true);
        LuxxleRewardsHelper.crossfade(
                claimProgress,
                actionNotificationButton,
                View.GONE,
                1f,
                LuxxleRewardsHelper.CROSS_FADE_DURATION);

        switch (type) {
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_AUTO_CONTRIBUTE:
                notificationIcon = R.drawable.icon_validated_notification;
                notificationClaimImg.setVisibility(View.GONE);
                notificationClaimSubText.setVisibility(View.GONE);
                String result = args[1];
                switch (result) {
                    case AUTO_CONTRIBUTE_SUCCESS: // Succcess
                        actionNotificationButton.setText(
                                mPopupView.getResources().getString(R.string.ok));
                        notificationClickAction = NotificationClickAction.DO_NOTHING;
                        title = mPopupView.getResources().getString(
                                R.string.luxxle_ui_rewards_contribute);
                        notificationIcon = R.drawable.ic_notification_auto_contribute;

                        double value = 0;
                        String valueString = "";
                        String[] splittedValue = args[3].split("\\.", 0);
                        if (splittedValue.length != 0 && splittedValue[0].length() >= 18) {
                            value = LuxxleRewardsHelper.probiToDouble(args[3]);
                            valueString = Double.isNaN(value)
                                    ? ERROR_CONVERT_PROBI
                                    : String.format(Locale.getDefault(), "%.3f", value);
                        } else {
                            value = Double.parseDouble(args[3]);
                            valueString = String.format(Locale.getDefault(), "%.3f", value);
                        }

                        description = String.format(
                                mPopupView.getResources().getString(
                                        R.string.luxxle_ui_rewards_contribute_description),
                                valueString);
                        break;
                    case AUTO_CONTRIBUTE_GENERAL_ERROR: // General error
                        actionNotificationButton.setText(
                                mPopupView.getResources().getString(R.string.ok));
                        title = mPopupView.getResources().getString(
                                R.string.monthly_tip_failed_notification_title);
                        notificationIcon = R.drawable.ic_notification_error;
                        description = mPopupView.getResources().getString(
                                R.string.monthly_tip_failed_notification_text);
                        break;
                    default:
                        title = "";
                        notificationIcon = R.drawable.ic_notification_error;
                        description = mPopupView.getResources().getString(
                                R.string.luxxle_ui_notification_desc_contr_error);
                }
                if (title.isEmpty()) {
                    actionNotificationButton.setVisibility(View.GONE);
                }
                break;
            case LuxxleRewardsNativeWorker
                    .REWARDS_NOTIFICATION_TIPS_PROCESSED: // Monthly tip completed
                notificationClaimImg.setVisibility(View.GONE);
                notificationClaimSubText.setVisibility(View.GONE);
                actionNotificationButton.setText(mPopupView.getResources().getString(R.string.ok));
                notificationClickAction = NotificationClickAction.DO_NOTHING;
                title = mPopupView.getResources().getString(R.string.luxxle_ui_contribution_tips);
                description = mPopupView.getResources().getString(
                        R.string.luxxle_ui_tips_processed_notification);
                notificationIcon = R.drawable.ic_notification_auto_contribute;
                break;
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_ADS_ONBOARDING:
                notificationClaimImg.setVisibility(View.GONE);
                notificationClaimSubText.setVisibility(View.GONE);
                actionNotificationButton.setText(
                        mPopupView.getResources().getString(R.string.luxxle_ui_turn_on_ads));
                notificationClickAction = NotificationClickAction.TURN_ON_ADS;
                title = mPopupView.getResources().getString(
                        R.string.luxxle_ui_luxxle_ads_launch_title);
                description = "";
                notificationIcon = R.drawable.ic_info_rewards;
                break;
            case LuxxleRewardsNativeWorker
                    .REWARDS_NOTIFICATION_VERIFIED_PUBLISHER: // Pending publisher verified
                notificationClaimImg.setVisibility(View.GONE);
                notificationClaimSubText.setVisibility(View.GONE);
                String pubName = args[0];
                actionNotificationButton.setText(mPopupView.getResources().getString(R.string.ok));
                notificationClickAction = NotificationClickAction.DO_NOTHING;
                title = mPopupView.getResources().getString(
                        R.string.luxxle_ui_pending_contribution_title);
                description = mPopupView.getResources().getString(
                        R.string.luxxle_ui_verified_publisher_notification, pubName);
                notificationIcon = R.drawable.ic_notification_pending;
                break;
            case LuxxleRewardsNativeWorker.REWARDS_NOTIFICATION_GENERAL:
                notificationClaimImg.setVisibility(View.GONE);
                notificationClaimSubText.setVisibility(View.GONE);
                String errorType = args[0];
                switch (errorType) {
                    case WALLET_DISCONNECTED:
                        actionNotificationButton.setText(mPopupView.getResources().getString(
                                R.string.logged_out_notification_action_text));
                        notificationClickAction = NotificationClickAction.RECONNECT;
                        title = mPopupView.getResources().getString(
                                R.string.logged_out_notification_title);
                        description = mPopupView.getResources().getString(
                                R.string.logged_out_notification_text);
                        notificationIcon = R.drawable.ic_notification_error;
                        View balanceText = mPopupView.findViewById(R.id.balance_text);
                        TextView loggedOutStateText =
                                mPopupView.findViewById(R.id.logged_out_state_text);
                        loggedOutStateText.setVisibility(View.VISIBLE);
                        balanceText.setVisibility(View.GONE);
                        mWalletBalanceProgress.setVisibility(View.GONE);
                        TextView btnVerifyWallet = mPopupView.findViewById(R.id.btn_verify_wallet);

                        int textId = R.string.luxxle_ui_wallet_button_logged_out;

                        btnVerifyWallet.setText(mPopupView.getResources().getString(textId));
                        break;
                    case UPHOLD_BAT_NOT_ALLOWED:
                        actionNotificationButton.setText(
                                mPopupView.getResources().getString(R.string.ok));
                        notificationClickAction = NotificationClickAction.DO_NOTHING;
                        title = mPopupView.getResources().getString(
                                R.string.bat_unavailable_notification_title);
                        description = mPopupView.getResources().getString(
                                R.string.bat_unavailable_notification_text);
                        notificationIcon = R.drawable.ic_notification_error;
                        break;
                    case UPHOLD_INSUFFICIENT_CAPABILITIES:
                        actionNotificationButton.setText(
                                mPopupView.getResources().getString(R.string.ok));
                        notificationClickAction = NotificationClickAction.DO_NOTHING;
                        title = mPopupView.getResources().getString(
                                R.string.limited_functionality_notification_title);
                        description = mPopupView.getResources().getString(
                                R.string.limited_functionality_notification_text);
                        notificationIcon = R.drawable.ic_notification_error;
                        break;
                }
                break;
            default:
                Log.e(TAG, "This notification type is either invalid or not handled yet: " + type);
                assert false;
                return;
        }
        notificationTitleText.setText(title);
        notificationTitleText.setVisibility(title.isEmpty() ? View.GONE : View.VISIBLE);
        notificationTitleText.setCompoundDrawablesWithIntrinsicBounds(notificationIcon, 0, 0, 0);
        notificationSubtitleText.setText(description);
        notificationSubtitleText.setVisibility(description.isEmpty() ? View.GONE : View.VISIBLE);

        if (mNotificationLayout != null) {
            mNotificationLayout.setVisibility(View.VISIBLE);
            int foregroundColor = R.color.rewards_panel_foreground_color;
            mRewardsMainLayout.setForeground(
                    new ColorDrawable(ContextCompat.getColor(mActivity, foregroundColor)));
            enableControls(false, mRewardsMainLayout);
            mNotificationShown = true;
        }

        setNotificationButtoClickListener(notificationClickAction);
    }

    private void setNotificationButtoClickListener(
            NotificationClickAction notificationClickAction) {
        TextView actionNotificationButton = mPopupView.findViewById(R.id.btn_action_notification);
        if (notificationClickAction == NotificationClickAction.DO_NOTHING) {
            actionNotificationButton.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            if (mCurrentNotificationId.equals(REWARDS_PROMOTION_CLAIM_ERROR_ID)) {
                                dismissNotification(mCurrentNotificationId);
                                return;
                            }
                            mLuxxleRewardsNativeWorker.deleteNotification(mCurrentNotificationId);
                        }
                    });
        } else if (notificationClickAction == NotificationClickAction.RECONNECT) {
            actionNotificationButton.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            mLuxxleRewardsNativeWorker.deleteNotification(mCurrentNotificationId);
                            TabUtils.openUrlInNewTab(
                                    false,
                                    mExternalWallet != null
                                            ? LuxxleActivity.LUXXLE_REWARDS_WALLET_RECONNECT_URL
                                            : LuxxleActivity
                                                    .LUXXLE_REWARDS_SETTINGS_WALLET_VERIFICATION_URL);
                            dismiss();
                        }
                    });
        } else if (notificationClickAction == NotificationClickAction.TURN_ON_ADS) {
            actionNotificationButton.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            mLuxxleRewardsNativeWorker.deleteNotification(mCurrentNotificationId);
                            assert LuxxleReflectionUtil.equalTypes(
                                    mActivity.getClass(), LuxxleActivity.class);
                            LuxxleActivity.class
                                    .cast(mActivity)
                                    .openNewOrSelectExistingTab(
                                            LuxxleActivity.LUXXLE_REWARDS_SETTINGS_URL);
                            dismiss();
                        }
                    });
        }
    }

    private void dismissNotification(String id) {
        if (!mCurrentNotificationId.equals(id)) {
            return;
        }
        hideNotifications();
        mCurrentNotificationId = "";
        if (mLuxxleRewardsNativeWorker != null) {
            mLuxxleRewardsNativeWorker.getAllNotifications();
        }
    }

    private void hideNotifications() {
        if (mNotificationLayout != null) {
            mNotificationLayout.setVisibility(View.GONE);
            mRewardsMainLayout.setForeground(null);
            enableControls(true, mRewardsMainLayout);
        }
    }

    // Rewards data callbacks
    @Override
    public void onReconcileComplete(int resultCode, int rewardsType, double amount) {
        mLuxxleRewardsNativeWorker.getExternalWallet();
    }

    @Override
    public void onGetCurrentBalanceReport(double[] report) {
        if (report == null) {
            return;
        }
        String batText = LuxxleRewardsHelper.BAT_TEXT;
        for (int i = 0; i < report.length; i++) {
            TextView tv = null;
            String text = "";

            double probiDouble = report[i];
            String value =
                    Double.isNaN(probiDouble)
                            ? "0.000" + batText
                            : String.format(Locale.getDefault(), "%.3f", probiDouble);

            switch (i) {
                case BALANCE_REPORT_EARNING_FROM_ADS:
                    tv = mPopupView.findViewById(R.id.rewards_from_ads_bat_text);
                    text = "<font color=#C12D7C>" + value + "</font>";
                    break;
                case BALANCE_REPORT_AUTO_CONTRIBUTE:
                    tv = mPopupView.findViewById(R.id.auto_contribute_bat_text);
                    text = "<font color=#4C54D2>" + value + "</font>";
                    break;
                case BALANCE_REPORT_ONE_TIME_DONATION:
                    tv = mPopupView.findViewById(R.id.one_time_tip_bat_text);
                    text = "<font color=#4C54D2>" + value + "</font>";
                    break;
                case BALANCE_REPORT_RECURRING_DONATION:
                    tv = mPopupView.findViewById(R.id.monthly_tips_bat_text);
                    text = "<font color=#4C54D2>" + value + "</font>";
                    break;
            }
            Spanned toInsert = LuxxleRewardsHelper.spannedFromHtmlString(text);
            if (tv != null) {
                tv.setText(toInsert);
            }
        }
    }

    @Override
    public void onGetAdsAccountStatement(
            boolean success,
            double nextPaymentDate,
            int adsReceivedThisMonth,
            double minEarningsThisMonth,
            double maxEarningsThisMonth,
            double minEarningsLastMonth,
            double maxEarningsLastMonth) {
        if (mExternalWallet != null
                && mExternalWallet.getStatus() == WalletStatus.NOT_CONNECTED
                && !PackageUtils.isFirstInstall(mActivity)) {
            mPopupView.findViewById(R.id.ads_seen_group).setVisibility(View.GONE);
            mPopupView.findViewById(R.id.rewards_not_connected_group).setVisibility(View.VISIBLE);
            TextView batAdsBalanceLearnMoreText =
                    mPopupView.findViewById(R.id.bat_ads_balance_learn_more_text);
            batAdsBalanceLearnMoreText.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            CustomTabActivity.showInfoPage(mActivity, LUXXLE_REWARDS_CHANGES_PAGE);
                        }
                    });
        } else {
            TextView adsCountText = mPopupView.findViewById(R.id.ads_count_text);
            adsCountText.setText(String.valueOf(adsReceivedThisMonth));
        }

        if (minEarningsLastMonth > 0) {
            showPayoutStatusBanner((long) nextPaymentDate);
        }
    }

    private double getDaysUntilRewardsPayment(long nextPaymentDateLong) {
        Date nextPaymentDate = new Date(nextPaymentDateLong);
        Calendar nextPaymentDateCalendar = Calendar.getInstance();
        nextPaymentDateCalendar.setTime(nextPaymentDate);
        nextPaymentDateCalendar.set(Calendar.HOUR_OF_DAY, 0);
        nextPaymentDateCalendar.set(Calendar.MINUTE, 0);
        nextPaymentDateCalendar.set(Calendar.SECOND, 0);

        Calendar nowDateCalendar = Calendar.getInstance();
        if (nextPaymentDateCalendar.get(Calendar.MONTH) != nowDateCalendar.get(Calendar.MONTH)) {
            return 0;
        }

        long delta =
                nextPaymentDateCalendar.getTime().getTime() - nowDateCalendar.getTime().getTime();
        double days = Math.ceil((double) delta / 24 / 60 / 60 / 1000);

        if (days < 1) {
            return 0;
        }

        return days;
    }

    private String getPaymentMonth() {
        Calendar calendar = Calendar.getInstance();
        calendar.add(Calendar.MONTH, -1);
        return (String) android.text.format.DateFormat.format("MMMM", calendar);
    }

    private String formatPaymentDate(long nextPaymentDateLong) {
        Date nextPaymentDate = new Date(nextPaymentDateLong);
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(nextPaymentDate);
        calendar.set(Calendar.HOUR_OF_DAY, 0);
        calendar.set(Calendar.MINUTE, 0);
        calendar.set(Calendar.SECOND, 0);

        return (String) android.text.format.DateFormat.format("MMMM dd", calendar.getTime());
    }

    private void showPayoutStatusBanner(long nextPaymentDate) {
        String payoutStatus = mLuxxleRewardsNativeWorker.getPayoutStatus();
        if (mPayoutStatusBannerLayout != null
                && mExternalWallet != null
                && mExternalWallet.getStatus() == WalletStatus.CONNECTED) {
            int payoutStatusBannerVisibility = View.VISIBLE;
            int walletBalanceBg = R.drawable.rewards_wallet_balance_with_payout_bg;
            ImageView payoutBannerImg = mPopupView.findViewById(R.id.payout_banner_img);
            TextView payoutBannerText = mPopupView.findViewById(R.id.payout_banner_text);
            if (payoutStatus.equals(PAYOUT_STATUS_COMPLETE)) {
                mPayoutStatusBannerLayout.setBackgroundDrawable(ResourcesCompat.getDrawable(
                        ContextUtils.getApplicationContext().getResources(),
                        R.drawable.rewards_panel_payout_complete_bg, null));
                payoutBannerImg.setImageResource(R.drawable.ic_payout_status_complete);
                SpannableString spannableBannerText = spannableClickSpan(
                        String.format(mPopupView.getResources().getString(
                                              R.string.rewards_panel_payout_complete_text),
                                getPaymentMonth(),
                                mPopupView.getResources().getString(R.string.support)),
                        mPopupView.getResources().getString(R.string.support), SUPPORT_URL);
                payoutBannerText.setMovementMethod(LinkMovementMethod.getInstance());
                payoutBannerText.setText(spannableBannerText);
            } else if (payoutStatus.equals(PAYOUT_STATUS_PROCESSING)) {
                mPayoutStatusBannerLayout.setBackgroundDrawable(ResourcesCompat.getDrawable(
                        ContextUtils.getApplicationContext().getResources(),
                        R.drawable.rewards_panel_payout_processing_bg, null));
                payoutBannerImg.setVisibility(View.GONE);
                SpannableString spannableBannerText = spannableClickSpan(
                        String.format(mPopupView.getResources().getString(
                                              R.string.rewards_panel_payout_processing_text),
                                getPaymentMonth(),
                                mPopupView.getResources().getString(
                                        R.string.rewards_panel_payout_check_status_text)),
                        mPopupView.getResources().getString(
                                R.string.rewards_panel_payout_check_status_text),
                        ADS_PAYOUT_STATUS_URL);
                payoutBannerText.setMovementMethod(LinkMovementMethod.getInstance());
                payoutBannerText.setText(spannableBannerText);
            } else if ((payoutStatus.equals(PAYOUT_STATUS_PENDING)
                               || TextUtils.isEmpty(payoutStatus))
                    && getDaysUntilRewardsPayment(nextPaymentDate) > 0) {
                mPayoutStatusBannerLayout.setBackgroundDrawable(ResourcesCompat.getDrawable(
                        ContextUtils.getApplicationContext().getResources(),
                        R.drawable.rewards_panel_payout_processing_bg, null));
                payoutBannerImg.setImageResource(R.drawable.ic_payout_status_pending);
                payoutBannerText.setText(
                        String.format(
                                mPopupView
                                        .getResources()
                                        .getString(R.string.rewards_panel_payout_pending_text),
                                getPaymentMonth(),
                                formatPaymentDate(nextPaymentDate)));
            } else {
                payoutStatusBannerVisibility = View.GONE;
                walletBalanceBg = R.drawable.rewards_wallet_balance_bg;
            }
            mPayoutStatusBannerLayout.setVisibility(payoutStatusBannerVisibility);
            if (mRewardsWalletBalance != null) {
                mRewardsWalletBalance.setBackgroundDrawable(
                        ResourcesCompat.getDrawable(
                                ContextUtils.getApplicationContext().getResources(),
                                walletBalanceBg,
                                null));
            }
        }
    }

    @Override
    public void onGetUserType(int userType) {
        if (UserType.UNCONNECTED == userType) {
            newInstallViewChanges();
        }
    }

    @Override
    public void onBalance(boolean success) {
        try {
            if (success) {
                if (mBalanceDataViewGroups != null) {
                    mBalanceDataViewGroups.setVisibility(
                            (mExternalWallet != null
                                            && mExternalWallet.getStatus()
                                                    == WalletStatus.LOGGED_OUT)
                                    ? View.GONE
                                    : View.VISIBLE);
                }
                mWalletBalanceProgress.setVisibility(View.GONE);
                if (mLuxxleRewardsNativeWorker != null) {
                    LuxxleRewardsBalance walletBalanceObject =
                            mLuxxleRewardsNativeWorker.getWalletBalance();
                    double walletBalance = 0;
                    if (walletBalanceObject != null) {
                        walletBalance = walletBalanceObject.getTotal();
                    }

                    TextView batBalanceText = mPopupView.findViewById(R.id.bat_balance_text);
                    batBalanceText.setText(LuxxleRewardsHelper.getFormattedAmount(walletBalance));
                    double usdValue = walletBalance * mLuxxleRewardsNativeWorker.getWalletRate();
                    String usdText = String.format(
                            mPopupView.getResources().getString(R.string.luxxle_ui_usd),
                            String.format(Locale.getDefault(), "%.2f", usdValue));
                    TextView usdBalanceText = mPopupView.findViewById(R.id.usd_balance_text);
                    usdBalanceText.setText(usdText);
                }
            }
        } catch (NullPointerException e) {
            Log.e(TAG, "LuxxleRewardsPanel onBalance " + e);
        }
    }

    @Override
    public void onRewardsParameters() {
        if (mShouldShowOnboardingForConnectAccount) {
            mShouldShowOnboardingForConnectAccount = false;
            showLuxxleRewardsOnboarding(true);
        } else if (mExternalWallet != null) {
            if (mLuxxleRewardsNativeWorker.getVbatDeadline() > 0) {
                mLuxxleRewardsNativeWorker.getUserType();
            }
            showViewsBasedOnExternalWallet();
            if (mLuxxleRewardsNativeWorker.isTermsOfServiceUpdateRequired()) {
                showTermsOfServiceUi();
            }
        }
    }

    @Override
    public void onTermsOfServiceUpdateAccepted() {
        panelShadow(false);
        enableControls(true, mRewardsMainLayout);
        mRewardsTosModal.setVisibility(View.GONE);
    }

    private void showTermsOfServiceUi() {
        panelShadow(true);
        enableControls(false, mRewardsMainLayout);
        mRewardsTosModal.setVisibility(View.VISIBLE);

        ChromeClickableSpan resetClickableSpan =
                new ChromeClickableSpan(
                        mActivity.getColor(R.color.rewards_panel_notification_secondary_text_color),
                        (textView) -> {
                            TabUtils.openUrlInNewTab(false, LuxxleActivity.LUXXLE_REWARDS_RESET_PAGE);
                            dismiss();
                        });
        ChromeClickableSpan tosClickableSpan =
                new ChromeClickableSpan(
                        mActivity.getColor(R.color.luxxle_blue_tint_color),
                        (textView) -> {
                            TabUtils.openUrlInNewTab(false, LuxxleActivity.LUXXLE_TERMS_PAGE);
                            dismiss();
                        });
        String tosText =
                mActivity.getResources().getString(R.string.rewards_tos_update_text)
                        + "\n\n"
                        + mActivity.getResources().getString(R.string.rewards_tos_update_link_text);

        SpannableString tosSpannableString =
                SpanApplier.applySpans(
                        tosText,
                        new SpanInfo(
                                "<reset>", "</reset>", resetClickableSpan, new UnderlineSpan()),
                        new SpanInfo("<tos>", "</tos>", tosClickableSpan, new UnderlineSpan()));

        TextView tosSubtitleText = mRewardsTosModal.findViewById(R.id.tos_subtitle_text);
        tosSubtitleText.setMovementMethod(LinkMovementMethod.getInstance());
        tosSubtitleText.setText(tosSpannableString);

        TextView btnActionTos = mRewardsTosModal.findViewById(R.id.btn_action_tos);
        btnActionTos.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        mLuxxleRewardsNativeWorker.acceptTermsOfServiceUpdate();
                    }
                });
    }

    @Override
    public void onGetLatestNotification(String id, int type, long unusedTimestamp, String[] args) {
        if (!mCurrentNotificationId.equals(REWARDS_PROMOTION_CLAIM_ERROR_ID)) {
            showNotification(id, type, args);
        }
    }

    @Override
    public void onNotificationDeleted(String id) {
        dismissNotification(id);
    }

    @Override
    public void onGetExternalWallet(String externalWallet) {
        if (!TextUtils.isEmpty(externalWallet)) {
            try {
                mExternalWallet = new LuxxleRewardsExternalWallet(externalWallet);
            } catch (JSONException e) {
                mExternalWallet = null;
            }
        }
        if (mExternalWallet != null && LuxxleRewardsHelper.isRewardsEnabled()) {
            mLuxxleRewardsNativeWorker.getRewardsParameters();
        } else {
            showOnBoarding();
        }
    }

    private void showSolanaEligibleUi() {
        if (mRewardsSolanaEligibleLayout != null) {
            mRewardsSolanaEligibleLayout.setVisibility(View.VISIBLE);
            AppCompatImageView modalCloseButton =
                    mRewardsSolanaEligibleLayout.findViewById(R.id.solana_eligible_ui_modal_close);
            modalCloseButton.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            removeSolanaEligibleUi();
                        }
                    });
            LinearLayout connectButton =
                    mRewardsSolanaEligibleLayout.findViewById(R.id.btn_connect);
            connectButton.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            removeSolanaEligibleUi();
                            TabUtils.openUrlInNewTab(
                                    false,
                                    LuxxleActivity.LUXXLE_REWARDS_SETTINGS_WALLET_VERIFICATION_URL);
                            dismiss();
                        }
                    });

            Button notNowButton = mRewardsSolanaEligibleLayout.findViewById(R.id.btn_not_now);
            notNowButton.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            removeSolanaEligibleUi();
                        }
                    });
        }
    }

    private void removeSolanaEligibleUi() {
        mRewardsSolanaEligibleLayout.setVisibility(View.GONE);
        if (mLuxxleActivity != null) {
            UserPrefs.get(mLuxxleActivity.getCurrentProfile())
                    .setBoolean(LuxxlePref.SELF_CUSTODY_INVITE_DISMISSED, true);
        }
    }

    private void showOnBoarding() {
        if (mLuxxleActivity != null) {
            int deviceWidth = ConfigurationUtils.getDisplayMetricsWidth(mLuxxleActivity);
            deviceWidth = (int) (mIsTablet ? (deviceWidth * 0.6) : (deviceWidth * 0.95));
            RewardsOnboarding panel = new RewardsOnboarding(mAnchorView, deviceWidth);
            panel.showLikePopDownMenu();
        }
    }

    private void showViewsBasedOnExternalWallet() {
        assumeNonNull(mExternalWallet);
        int walletStatus = mExternalWallet.getStatus();
        if (PackageUtils.isFirstInstall(mActivity)) {
            if (walletStatus == WalletStatus.CONNECTED || walletStatus == WalletStatus.LOGGED_OUT) {
                existingUserViewChanges();
            } else if (walletStatus == WalletStatus.NOT_CONNECTED) {
                newInstallViewChanges();
            }
        } else {
            existingUserViewChanges();
        }
        if (mLuxxleRewardsNativeWorker.shouldShowSelfCustodyInvite()
                && walletStatus == WalletStatus.NOT_CONNECTED) {
            showSolanaEligibleUi();
        }
    }

    // Generic UI changes
    View.OnClickListener mLuxxleRewardsOnboardingClickListener =
            new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    if (mLuxxleRewardsViewPager == null) {
                        return;
                    }
                    int viewId = view.getId();
                    if (viewId == R.id.btn_start_quick_tour) {
                        if (mLuxxleRewardsViewPager.getCurrentItem() == 0) {
                            mLuxxleRewardsViewPager.setCurrentItem(
                                    mLuxxleRewardsViewPager.getCurrentItem() + 1);
                        }
                    }

                    if (viewId == R.id.btn_next) {
                        if (mLuxxleRewardsOnboardingPagerAdapter != null) {
                            if (mLuxxleRewardsViewPager.getCurrentItem()
                                    == mLuxxleRewardsOnboardingPagerAdapter.getCount() - 2) {
                                if (mLuxxleRewardsOnboardingView != null) {
                                    mLuxxleRewardsOnboardingView.setVisibility(View.GONE);
                                }
                                showConnectAccountModal();
                            } else if (mLuxxleRewardsViewPager.getCurrentItem()
                                    == mLuxxleRewardsOnboardingPagerAdapter.getCount() - 1) {
                                if (mLuxxleRewardsOnboardingView != null) {
                                    mLuxxleRewardsOnboardingView.setVisibility(View.GONE);

                                    if (mPopupView != null
                                            && mLuxxleRewardsNativeWorker.isSupported()
                                            && LuxxleRewardsHelper
                                                    .shouldShowLuxxleRewardsOnboardingModal()
                                            && !LuxxleRewardsHelper.isRewardsEnabled()) {
                                        showOnBoarding();
                                    } else {
                                        mLuxxleRewardsNativeWorker.getExternalWallet();
                                        panelShadow(false);
                                    }
                                }
                            } else {
                                mLuxxleRewardsViewPager.setCurrentItem(
                                        mLuxxleRewardsViewPager.getCurrentItem() + 1);
                            }
                        }
                    }

                    if (viewId == R.id.btn_skip
                            && mLuxxleRewardsOnboardingView != null
                            && mLuxxleRewardsOnboardingPagerAdapter != null) {
                        mLuxxleRewardsViewPager.setCurrentItem(
                                mLuxxleRewardsOnboardingPagerAdapter.getCount() - 1);
                    }

                    if (viewId == R.id.btn_go_back) {
                        mLuxxleRewardsViewPager.setCurrentItem(
                                mLuxxleRewardsViewPager.getCurrentItem() - 1);
                    }
                }
            };

    private void newInstallViewChanges() {
        showUnverifiedLayout();
        checkForRewardsOnboarding();
        String rewardsCountryCode = "";
        if (mLuxxleActivity != null) {
            rewardsCountryCode =
                    UserPrefs.get(mLuxxleActivity.getCurrentProfile())
                            .getString(LuxxlePref.DECLARED_GEO);
        }

        if (mPopupView != null && mLuxxleRewardsNativeWorker.isSupported()) {
            if (!LuxxleRewardsHelper.isRewardsEnabled()
                    && LuxxleRewardsHelper.shouldShowLuxxleRewardsOnboardingModal()) {
                showOnBoarding();
            } else if (TextUtils.isEmpty(rewardsCountryCode)) {
                mLuxxleRewardsNativeWorker.getAvailableCountries();
            }
        }
    }

    private void existingUserViewChanges() {
        showRewardsMainUI();
        requestPublisherInfo();
        setNotificationsControls();
        mWalletBalanceProgress.setVisibility(View.VISIBLE);
        mLuxxleRewardsNativeWorker.fetchBalance();
        mLuxxleRewardsNativeWorker.getRecurringDonations();
        mLuxxleRewardsNativeWorker.getAdsAccountStatement();

        mLuxxleRewardsNativeWorker.getCurrentBalanceReport();
        mLuxxleRewardsNativeWorker.getAllNotifications();
        setVerifyWalletButton();
        showRewardsFromAdsSummary();
        mayBeShowNotitionPermissionDialog();
    }

    private void mayBeShowNotitionPermissionDialog() {
        clickOnCloseButtonOfNotificationPermission();
        clickOnTurnOnNotificationButton();
        if (!LuxxlePermissionUtils.hasNotificationPermission(mAnchorView.getContext())
                || LuxxleNotificationWarningDialog.shouldShowRewardWarningDialog(
                        mAnchorView.getContext())) {
            mNotificationLayout.setVisibility(View.VISIBLE);
            mNotificationPermissionLayout.setVisibility(View.VISIBLE);
        }
    }

    private void clickOnCloseButtonOfNotificationPermission() {
        View closeButton = mNotificationPermissionLayout.findViewById(R.id.close_text_button);
        closeButton.setOnClickListener((v) -> {
            mNotificationPermissionLayout.setVisibility(View.GONE);
            if (!mNotificationShown) {
                mNotificationLayout.setVisibility(View.GONE);
            }
        });
    }

    private void clickOnTurnOnNotificationButton() {
        View turnOnNotification =
                mNotificationPermissionLayout.findViewById(R.id.turnOnNotification);
        turnOnNotification.setOnClickListener((v) -> {
            mNotificationPermissionLayout.setVisibility(View.GONE);
            LuxxlePermissionUtils.notificationSettingPage(mAnchorView.getContext());
            if (!mNotificationShown) {
                mNotificationLayout.setVisibility(View.GONE);
            }
        });
    }

    private void showUnverifiedLayout() {
        View unverifiedStateLayout =
                mPopupView.findViewById(R.id.luxxle_rewards_panel_unverified_layout_id);
        unverifiedStateLayout.setVisibility(View.VISIBLE);
        TextView unverifiedToggleSubText =
                unverifiedStateLayout.findViewById(R.id.unverified_toggle_sub_text);
        View rewardsPanelUnverifiedOnSection =
                unverifiedStateLayout.findViewById(R.id.rewards_panel_unverified_on_section);
        TextView connectAccountButton =
                unverifiedStateLayout.findViewById(R.id.connect_account_button);
        connectAccountButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        TabUtils.openUrlInNewTab(
                                false,
                                LuxxleActivity.LUXXLE_REWARDS_SETTINGS_WALLET_VERIFICATION_URL);
                        dismiss();
                    }
                });
        View rewardsPanelUnverifiedCreatorSection =
                unverifiedStateLayout.findViewById(R.id.rewards_panel_unverified_creator_section);
        View rewardsPanelUnverifiedOffSection =
                unverifiedStateLayout.findViewById(R.id.rewards_panel_unverified_off_section);
        LinearLayout rewardsSettingsButton =
                unverifiedStateLayout.findViewById(R.id.rewards_settings_button);
        rewardsSettingsButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (mLuxxleActivity != null) {
                            mLuxxleActivity.openNewOrSelectExistingTab(
                                    LuxxleActivity.LUXXLE_REWARDS_SETTINGS_URL);
                        }
                        dismiss();
                    }
                });
        TextView learnMoreUnverifiedText =
                unverifiedStateLayout.findViewById(R.id.learn_more_unverified_text);
        learnMoreUnverifiedText.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        CustomTabActivity.showInfoPage(mActivity, LUXXLE_REWARDS_PAGE);
                    }
                });
        LuxxleTouchUtils.ensureMinTouchTarget(learnMoreUnverifiedText);

        if (LuxxleAdsNativeHelper.nativeIsOptedInToNotificationAds(
                ProfileManager.getLastUsedRegularProfile())) {
            if (mRewardsMainLayout != null) {
                mRewardsMainLayout.setVisibility(View.GONE);
            }
            unverifiedToggleSubText.setText(mPopupView.getResources().getString(
                    R.string.rewards_panel_unverified_switch_on_sub_text));
            rewardsPanelUnverifiedOnSection.setVisibility(View.VISIBLE);
            TextView rewardsPanelUnverifiedOnSectionText = unverifiedStateLayout.findViewById(
                    R.id.rewards_panel_unverified_on_section_text);
            TextView rewardsPanelUnverifiedOnSectionLearnMoreText =
                    unverifiedStateLayout.findViewById(
                            R.id.rewards_panel_unverified_on_section_learn_more_text);
            rewardsPanelUnverifiedOnSectionLearnMoreText.setOnClickListener(
                    new View.OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            CustomTabActivity.showInfoPage(
                                    mActivity, UNVERIFIED_USER_UNSUPPORTED_REGION_PAGE);
                        }
                    });
            LuxxleTouchUtils.ensureMinTouchTarget(rewardsPanelUnverifiedOnSectionLearnMoreText);
            String sectionText =
                    "<b>"
                            + mPopupView
                                    .getResources()
                                    .getString(R.string.ready_to_start_earning_text)
                            + "</b> "
                            + mPopupView
                                    .getResources()
                                    .getString(R.string.rewards_panel_unverified_on_section_text);
            rewardsPanelUnverifiedOnSectionText.setText(Html.fromHtml(sectionText));
            connectAccountButton.setVisibility(View.VISIBLE);
            rewardsPanelUnverifiedOnSectionLearnMoreText.setVisibility(View.GONE);
            rewardsPanelUnverifiedOffSection.setVisibility(View.GONE);
            mLuxxleRewardsNativeWorker.getPublishersVisitedCount();
        } else {
            if (mRewardsMainLayout != null) {
                mRewardsMainLayout.setVisibility(View.GONE);
            }
            unverifiedToggleSubText.setText(mPopupView.getResources().getString(
                    R.string.rewards_panel_unverified_switch_off_sub_text));
            rewardsPanelUnverifiedOnSection.setVisibility(View.GONE);
            rewardsPanelUnverifiedOffSection.setVisibility(View.VISIBLE);
            rewardsPanelUnverifiedCreatorSection.setVisibility(View.GONE);
        }
    }

    // Onboarding changes
    private void checkForRewardsOnboarding() {
        if (mPopupView != null
                && mLuxxleRewardsNativeWorker.isSupported()
                && (LuxxleRewardsHelper.shouldShowLuxxleRewardsOnboardingOnce()
                        || LuxxleRewardsHelper.shouldShowDeclareGeoModal())) {
            if (LuxxleRewardsHelper.shouldShowLuxxleRewardsOnboardingOnce()) {
                showLuxxleRewardsOnboarding(false);
                LuxxleRewardsHelper.setShowLuxxleRewardsOnboardingOnce(false);
            } else if (LuxxleRewardsHelper.shouldShowDeclareGeoModal()) {
                mLuxxleRewardsNativeWorker.getAvailableCountries();
                LuxxleRewardsHelper.setShowDeclareGeoModal(false);
            }
        }
    }

    private void showLuxxleRewardsOnboarding(boolean shouldShowMoreOption) {
        if (mLuxxleRewardsOnboardingView == null) {
            return;
        }

        panelShadow(true);

        mLuxxleRewardsOnboardingView.setVisibility(View.VISIBLE);
        final Button btnNext = mLuxxleRewardsOnboardingView.findViewById(R.id.btn_next);
        btnNext.setOnClickListener(mLuxxleRewardsOnboardingClickListener);
        mLuxxleRewardsOnboardingView
                .findViewById(R.id.btn_go_back)
                .setOnClickListener(mLuxxleRewardsOnboardingClickListener);
        mLuxxleRewardsOnboardingView
                .findViewById(R.id.btn_skip)
                .setOnClickListener(mLuxxleRewardsOnboardingClickListener);
        final View startQuickTourButton =
                (View) mLuxxleRewardsOnboardingView.findViewById(R.id.btn_start_quick_tour);
        startQuickTourButton.setOnClickListener(mLuxxleRewardsOnboardingClickListener);

        mLuxxleRewardsViewPager =
                mLuxxleRewardsOnboardingView.findViewById(R.id.luxxle_rewards_view_pager);
        mLuxxleRewardsViewPager.addOnPageChangeListener(new ViewPager.OnPageChangeListener() {
            @Override
            public void onPageScrolled(
                    int position, float positionOffset, int positionOffsetPixels) {
                if (positionOffset == 0 && positionOffsetPixels == 0 && position == 0) {
                    mLuxxleRewardsOnboardingView.findViewById(R.id.onboarding_first_screen_layout)
                            .setVisibility(View.VISIBLE);
                    mLuxxleRewardsOnboardingView.findViewById(R.id.onboarding_action_layout)
                            .setVisibility(View.GONE);
                } else {
                    mLuxxleRewardsOnboardingView.findViewById(R.id.onboarding_action_layout)
                            .setVisibility(View.VISIBLE);
                    mLuxxleRewardsOnboardingView.findViewById(R.id.onboarding_first_screen_layout)
                            .setVisibility(View.GONE);
                }
            }

            @Override
            public void onPageSelected(int position) {
                if (mLuxxleRewardsOnboardingPagerAdapter != null
                        && position == mLuxxleRewardsOnboardingPagerAdapter.getCount() - 1) {
                    btnNext.setCompoundDrawablesWithIntrinsicBounds(0, 0, 0, 0);
                    btnNext.setText(mActivity.getResources().getString(R.string.done));
                } else {
                    btnNext.setText(mActivity.getResources().getString(R.string.continue_text));
                    btnNext.setCompoundDrawablesWithIntrinsicBounds(
                            0, 0, R.drawable.ic_arrow_circle_right, 0);
                }
            }

            @Override
            public void onPageScrollStateChanged(int state) {}
        });
        mLuxxleRewardsOnboardingPagerAdapter = new LuxxleRewardsOnboardingPagerAdapter();
        mLuxxleRewardsOnboardingPagerAdapter.setOnboardingType(shouldShowMoreOption);
        mLuxxleRewardsViewPager.setAdapter(mLuxxleRewardsOnboardingPagerAdapter);
        TabLayout luxxleRewardsTabLayout =
                mLuxxleRewardsOnboardingView.findViewById(R.id.luxxle_rewards_tab_layout);
        luxxleRewardsTabLayout.setupWithViewPager(mLuxxleRewardsViewPager, true);

        // Try to get the tab view. Not officially supported so wrap in try..catch
        try {
            LinearLayout tabs = (LinearLayout) luxxleRewardsTabLayout.getChildAt(0);
            for (int i = 0; i < tabs.getChildCount(); ++i) {
                LuxxleTouchUtils.ensureMinTouchTarget(tabs.getChildAt(i));
            }
        } catch (Exception e) {
            Log.e(TAG, "Failed to enlarge touch target on tab:", e);
        }

        AppCompatImageView modalCloseButton =
                mLuxxleRewardsOnboardingView.findViewById(
                        R.id.luxxle_rewards_onboarding_layout_modal_close);
        modalCloseButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        mLuxxleRewardsOnboardingView.setVisibility(View.GONE);
                        panelShadow(false);
                        mLuxxleRewardsNativeWorker.getExternalWallet();
                    }
                });
        mLuxxleRewardsOnboardingView
                .findViewById(R.id.onboarding_first_screen_layout)
                .setVisibility(View.VISIBLE);
        mLuxxleRewardsOnboardingView
                .findViewById(R.id.onboarding_action_layout)
                .setVisibility(View.GONE);

        LuxxleTouchUtils.ensureMinTouchTarget(modalCloseButton);
        LuxxleTouchUtils.ensureMinTouchTarget(startQuickTourButton);
    }

    private void panelShadow(boolean isEnable) {
        if (isEnable) {
            if (mLuxxlePanelShadow != null) {
                mLuxxlePanelShadow.setVisibility(View.VISIBLE);
            }

            if (mLuxxleRewardsUnverifiedView != null) {
                enableControls(false, mLuxxleRewardsUnverifiedView);
            }
        } else {
            if (mLuxxlePanelShadow != null) {
                mLuxxlePanelShadow.setVisibility(View.GONE);
            }

            if (mLuxxleRewardsUnverifiedView != null) {
                enableControls(true, mLuxxleRewardsUnverifiedView);
            }
        }
    }

    private void showConnectAccountModal() {
        if (mConnectAccountModal == null) {
            return;
        }
        mConnectAccountModal.setVisibility(View.VISIBLE);

        Button connectAccountBtn = mConnectAccountModal.findViewById(R.id.btn_connect_account);
        connectAccountBtn.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        TabUtils.openUrlInNewTab(
                                false,
                                LuxxleActivity.LUXXLE_REWARDS_SETTINGS_WALLET_VERIFICATION_URL);
                        dismiss();
                    }
                });
        Button dismissBtn = mConnectAccountModal.findViewById(R.id.btn_do_it_later);
        dismissBtn.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (mLuxxleRewardsOnboardingView != null) {
                            mLuxxleRewardsOnboardingView.setVisibility(View.GONE);
                        }
                        mConnectAccountModal.setVisibility(View.GONE);
                        panelShadow(false);
                        mLuxxleRewardsNativeWorker.getExternalWallet();
                    }
                });
        AppCompatImageView closeBtn =
                mConnectAccountModal.findViewById(R.id.connect_account_layout_modal_close);
        closeBtn.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (mLuxxleRewardsOnboardingView != null) {
                            mLuxxleRewardsOnboardingView.setVisibility(View.GONE);
                        }
                        mConnectAccountModal.setVisibility(View.GONE);
                        panelShadow(false);
                        mLuxxleRewardsNativeWorker.getExternalWallet();
                    }
                });
    }

    private SpannableString learnMoreSpannableString(String text) {
        Spanned textToAgree = LuxxleRewardsHelper.spannedFromHtmlString(text);

        SpannableString ss = new SpannableString(textToAgree.toString());

        ChromeClickableSpan clickableSpan =
                new ChromeClickableSpan(
                        mActivity.getColor(R.color.luxxle_rewards_modal_theme_color),
                        (textView) -> {
                            CustomTabActivity.showInfoPage(mActivity, NEW_SIGNUP_DISABLED_URL);
                        });
        int learnMoreIndex = text.indexOf(mActivity.getResources().getString(R.string.learn_more));

        ss.setSpan(
                clickableSpan,
                learnMoreIndex,
                learnMoreIndex + mActivity.getResources().getString(R.string.learn_more).length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        return ss;
    }

    private SpannableString spannableClickSpan(
            String mainText, String clickableText, String clickUrl) {
        Spanned textMain = LuxxleRewardsHelper.spannedFromHtmlString(mainText);

        SpannableString ss = new SpannableString(textMain.toString());

        ChromeClickableSpan clickableSpan =
                new ChromeClickableSpan(
                        mActivity.getColor(R.color.luxxle_rewards_modal_theme_color),
                        (textView) -> {
                            CustomTabActivity.showInfoPage(mActivity, clickUrl);
                        });
        int clickableTextIndex = mainText.indexOf(clickableText);

        ss.setSpan(
                clickableSpan,
                clickableTextIndex,
                clickableTextIndex + clickableText.length(),
                Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        return ss;
    }

    @Override
    public void onGetPublishersVisitedCount(int count) {
        if (mPopupView != null) {
            mPopupView.findViewById(R.id.rewards_panel_unverified_creator_section)
                    .setVisibility(View.VISIBLE);
            TextView rewardsPanelUnverifiedCreatorCountText =
                    mPopupView.findViewById(R.id.rewards_panel_unverified_creator_count_text);
            rewardsPanelUnverifiedCreatorCountText.setText(String.valueOf(count));
            TextView rewardsPanelUnverifiedCreatorText =
                    mPopupView.findViewById(R.id.rewards_panel_unverified_creator_text);
            rewardsPanelUnverifiedCreatorText.setText(
                    String.format(mPopupView.getResources().getString(
                                          R.string.rewards_panel_unverified_creator_section_text),
                            count));
        }
    }

    private String getWalletString(String walletType) {
        if (walletType.equals(LuxxleWalletProvider.UPHOLD)) {
            return mActivity.getResources().getString(R.string.uphold);
        } else if (walletType.equals(LuxxleWalletProvider.GEMINI)) {
            return mActivity.getResources().getString(R.string.gemini);
        } else if (walletType.equals(LuxxleWalletProvider.BITFLYER)) {
            return mActivity.getResources().getString(R.string.bitflyer);
        } else if (walletType.equals(LuxxleWalletProvider.ZEBPAY)) {
            return mActivity.getResources().getString(R.string.zebpay);
        } else {
            return mActivity.getResources().getString(R.string.wallet_sol_name);
        }
    }

    private void showRewardsFromAdsSummary() {
        if (mExternalWallet == null) {
            return;
        }
        int walletStatus = mExternalWallet.getStatus();
        if (mLuxxleRewardsNativeWorker != null) {
            String walletType = mExternalWallet.getType();
            if (walletStatus == WalletStatus.CONNECTED
                    && (walletType.equals(LuxxleWalletProvider.UPHOLD)
                            || walletType.equals(LuxxleWalletProvider.BITFLYER)
                            || walletType.equals(LuxxleWalletProvider.GEMINI)
                            || walletType.equals(LuxxleWalletProvider.ZEBPAY))) {
                mPopupView.findViewById(R.id.auto_contribute_summary_seperator)
                        .setVisibility(View.GONE);
                mPopupView.findViewById(R.id.rewards_from_ads_summary_layout)
                        .setVisibility(View.GONE);
            } else {
                mPopupView.findViewById(R.id.auto_contribute_summary_seperator)
                        .setVisibility(View.VISIBLE);
                mPopupView.findViewById(R.id.rewards_from_ads_summary_layout)
                        .setVisibility(View.VISIBLE);
            }
            // Hide rewards from ads when verified but disconnected from provider
            if (walletStatus == WalletStatus.LOGGED_OUT) {
                mPopupView.findViewById(R.id.rewards_from_ads_summary_layout)
                        .setVisibility(View.GONE);
            }
        }
    }

    private void setVerifyWalletButton() {
        if (mExternalWallet == null) {
            return;
        }
        int status = mExternalWallet.getStatus();
        TextView btnVerifyWallet = mPopupView.findViewById(R.id.btn_verify_wallet);
        ImageView verifyWalletArrowImg = mPopupView.findViewById(R.id.verify_wallet_arrow_img);
        int rightDrawable = 0;
        int textId = 0;
        String walletType = mExternalWallet.getType();

        switch (status) {
            case WalletStatus.NOT_CONNECTED:
                rightDrawable = R.drawable.ic_verify_wallet_arrow;
                textId = R.string.luxxle_ui_wallet_button_connect;
                btnVerifyWallet.setCompoundDrawablesWithIntrinsicBounds(0, 0, rightDrawable, 0);
                break;
            case WalletStatus.CONNECTED:
                rightDrawable = getWalletIcon(walletType);
                textId = R.string.luxxle_ui_wallet_button_connected;
                btnVerifyWallet.setPadding(0, 0, 0, 0);
                btnVerifyWallet.setCompoundDrawablesWithIntrinsicBounds(0, 0, rightDrawable, 0);
                btnVerifyWallet.setBackgroundColor(Color.TRANSPARENT);
                verifyWalletArrowImg.setVisibility(View.VISIBLE);
                break;
            case WalletStatus.LOGGED_OUT:
                rightDrawable = getWalletIcon(walletType);
                textId = R.string.luxxle_ui_wallet_button_logged_out;
                btnVerifyWallet.setPadding(0, 0, 0, 0);
                btnVerifyWallet.setCompoundDrawablesWithIntrinsicBounds(0, 0, rightDrawable, 0);
                btnVerifyWallet.setBackgroundColor(Color.TRANSPARENT);
                verifyWalletArrowImg.setVisibility(View.VISIBLE);
                break;
            default:
                Log.e(TAG, "Unexpected external wallet status");
                return;
        }

        btnVerifyWallet.setVisibility(View.VISIBLE);
        btnVerifyWallet.setText(mPopupView.getResources().getString(textId));
        setVerifyWalletButtonClickEvent(btnVerifyWallet, status);
        setVerifyWalletButtonClickEvent(verifyWalletArrowImg, status);
    }

    private void setVerifyWalletButtonClickEvent(View btnVerifyWallet, final int status) {
        btnVerifyWallet.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        LuxxleRewardsBalance walletBalanceObject =
                                mLuxxleRewardsNativeWorker.getWalletBalance();
                        double walletBalance = 0;
                        if (walletBalanceObject != null) {
                            walletBalance = walletBalanceObject.getTotal();
                        }

                        switch (status) {
                            case WalletStatus.NOT_CONNECTED:
                                TabUtils.openUrlInNewTab(
                                        false,
                                        LuxxleActivity
                                                .LUXXLE_REWARDS_SETTINGS_WALLET_VERIFICATION_URL);
                                dismiss();
                                break;
                            case WalletStatus.CONNECTED:
                                openUserWalletActivity(status);
                                break;
                            case WalletStatus.LOGGED_OUT:
                                openUserWalletActivity(status);
                                break;
                            default:
                                Log.e(TAG, "Unexpected external wallet status");
                                return;
                        }
                    }
                });
    }

    private void openUserWalletActivity(int walletStatus) {
        int requestCode = LuxxleConstants.USER_WALLET_ACTIVITY_REQUEST_CODE;
        Intent intent = buildUserWalletActivityIntent(walletStatus);
        if (intent != null) {
            mActivity.startActivityForResult(intent, requestCode);
        }
    }

    private void requestPublisherInfo() {
        assumeNonNull(mBtnTip);

        Tab currentActiveTab = LuxxleRewardsHelper.currentActiveChromeTabbedActivityTab();
        if (currentActiveTab != null && !currentActiveTab.isIncognito()) {
            String url = currentActiveTab.getUrl().getSpec();
            if (URLUtil.isValidUrl(url)) {
                mLuxxleRewardsNativeWorker.getPublisherInfo(currentActiveTab.getId(), url);
                mPublisherFetcher = new Timer();
                mPublisherFetcher.schedule(new PublisherFetchTimer(currentActiveTab.getId(), url),
                        PUBLISHER_INFO_FETCH_RETRY, PUBLISHER_INFO_FETCH_RETRY);
                showTipSection();
                mBtnTip.setEnabled(true);
            } else {
                showSummarySection();
                mBtnTip.setEnabled(false);
            }
        } else {
            showSummarySection();
            mBtnTip.setEnabled(false);
        }
    }

    @Override
    public void onPublisherInfo(int tabId) {
        mPublisherExist = true;
        mCurrentTabId = tabId;

        String publisherFavIconURL =
                mLuxxleRewardsNativeWorker.getPublisherFavIconURL(mCurrentTabId);
        Tab currentActiveTab = LuxxleRewardsHelper.currentActiveChromeTabbedActivityTab();
        String url = currentActiveTab != null ? currentActiveTab.getUrl().getSpec() : "";
        final String favicon_url = publisherFavIconURL.isEmpty() ? url : publisherFavIconURL;

        mIconFetcher.retrieveLargeIcon(favicon_url, this);

        String pubName = mLuxxleRewardsNativeWorker.getPublisherName(mCurrentTabId);
        String pubId = mLuxxleRewardsNativeWorker.getPublisherId(mCurrentTabId);
        String pubSuffix = "";
        if (pubId.startsWith(YOUTUBE_TYPE)) {
            pubSuffix = mPopupView.getResources().getString(R.string.luxxle_ui_on_youtube);
        } else if (pubName.startsWith(TWITCH_TYPE)) {
            pubSuffix = mPopupView.getResources().getString(R.string.luxxle_ui_on_twitch);
        }
        pubName = "<b>" + pubName + "</b> " + pubSuffix;
        TextView publisherName = mPopupView.findViewById(R.id.publisher_name);
        publisherName.setText(Html.fromHtml(pubName));
        mPublisherAttention = mPopupView.findViewById(R.id.attention_value_text);
        String percent =
                Integer.toString(mLuxxleRewardsNativeWorker.getPublisherPercent(mCurrentTabId))
                        + "%";
        mPublisherAttention.setText(percent);
        updatePublisherStatus(mLuxxleRewardsNativeWorker.getPublisherStatus(mCurrentTabId));
    }

    @Override
    public void onRecurringDonationUpdated() {
        updateMonthlyContributionUI();
    }

    private void updateMonthlyContributionUI() {
        String pubId = mLuxxleRewardsNativeWorker.getPublisherId(mCurrentTabId);
        TextView monthlyTipText = mPopupView.findViewById(R.id.monthly_contribution_set_text);
        double recurrentAmount =
                mLuxxleRewardsNativeWorker.getPublisherRecurrentDonationAmount(pubId);
        if (mLuxxleRewardsNativeWorker.isCurrentPublisherInRecurrentDonations(pubId)) {
            mPopupView.findViewById(R.id.monthly_contribution_layout).setVisibility(View.VISIBLE);
            String amount = String.format(
                    mPopupView.getResources().getString(R.string.luxxle_rewards_bat_value_text),
                    String.format(Locale.getDefault(), "%.2f", recurrentAmount));
            monthlyTipText.setText(amount);
        }
    }

    private void updatePublisherStatus(int pubStatus) {
        TextView publisherVerified = mPopupView.findViewById(R.id.publisher_verified);
        publisherVerified.setAlpha(1f);
        ImageView refreshPublisher = mPopupView.findViewById(R.id.refresh_publisher);
        refreshPublisher.setAlpha(1f);
        refreshPublisher.setEnabled(true);
        View refreshStatusProgress = mPopupView.findViewById(R.id.progress_refresh_status);
        refreshStatusProgress.setVisibility(View.GONE);
        refreshPublisher.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        String pubId = mLuxxleRewardsNativeWorker.getPublisherId(mCurrentTabId);
                        refreshStatusProgress.setVisibility(View.VISIBLE);
                        refreshPublisher.setEnabled(false);
                        publisherVerified.setAlpha(.3f);
                        mLuxxleRewardsNativeWorker.refreshPublisher(pubId);
                    }
                });
        TextView btnSendTip = mPopupView.findViewById(R.id.btn_send_tip);
        TextView infoCreatorNotVerified = mPopupView.findViewById(R.id.info_creator_not_verified);
        boolean isVerified = (pubStatus != PublisherStatus.NOT_VERIFIED);
        String verifiedText =
                mPopupView
                        .getResources()
                        .getString(
                                isVerified
                                        ? R.string.luxxle_ui_verified_publisher
                                        : R.string.luxxle_ui_not_verified_publisher);
        publisherVerified.setCompoundDrawablesWithIntrinsicBounds(
                (isVerified
                        ? R.drawable.rewards_verified_tick_icon
                        : R.drawable.rewards_unverified_tick_icon),
                0,
                0,
                0);
        btnSendTip.setEnabled(isVerified);
        btnSendTip.setClickable(isVerified);
        btnSendTip.setBackgroundDrawable(
                ResourcesCompat.getDrawable(
                        ContextUtils.getApplicationContext().getResources(),
                        (isVerified
                                ? R.drawable.blue_48_rounded_bg
                                : R.drawable.send_contribution_button_background),
                        /* theme= */ null));
        infoCreatorNotVerified.setVisibility(isVerified ? View.GONE : View.VISIBLE);
        publisherVerified.setText(verifiedText);
        publisherVerified.setVisibility(View.VISIBLE);
        if (!isVerified) {
            String notVerifiedText =
                    String.format(
                            mActivity.getString(R.string.info_creator_not_verified),
                            mActivity.getResources().getString(R.string.learn_more));
            SpannableString spannableLearnMore = learnMoreSpannableString(notVerifiedText);
            infoCreatorNotVerified.setMovementMethod(LinkMovementMethod.getInstance());
            infoCreatorNotVerified.setText(spannableLearnMore);
        }
    }

    @Override
    public void onRefreshPublisher(int status, String publisherKey) {
        String pubName = mLuxxleRewardsNativeWorker.getPublisherName(mCurrentTabId);
        if (pubName.equals(publisherKey)) {
            updatePublisherStatus(status);
        }
    };

    class PublisherFetchTimer extends TimerTask {
        private final int mTabId;
        private final String mUrl;

        PublisherFetchTimer(int tabId, String url) {
            this.mTabId = tabId;
            this.mUrl = url;
        }

        @Override
        public void run() {
            if (mPublisherExist || mPublisherFetchesCount >= PUBLISHER_FETCHES_COUNT) {
                if (mPublisherFetcher != null) {
                    mPublisherFetcher.cancel();
                    mPublisherFetcher = null;
                }

                return;
            }
            if (mLuxxleRewardsNativeWorker == null) {
                return;
            }
            mActivity.runOnUiThread(
                    new Runnable() {
                        @Override
                        public void run() {
                            mLuxxleRewardsNativeWorker.getPublisherInfo(mTabId, mUrl);
                        }
                    });
            mPublisherFetchesCount++;
        }
    }

    public void dismiss() {
        mPopupWindow.dismiss();
        if (mPopupWindowTippingTabletUI != null && mPopupWindowTippingTabletUI.getDialog() != null
                && mPopupWindowTippingTabletUI.getDialog().isShowing()
                && !mPopupWindowTippingTabletUI.isRemoving()) {
            mPopupWindowTippingTabletUI.getDialog().dismiss();
        }
    }

    public boolean isShowing() {
        return mPopupWindow.isShowing();
    }

    @Override
    public void onLargeIconReady(Bitmap icon) {
        setFavIcon(icon);
    }

    private void setFavIcon(Bitmap bitmap) {
        if (bitmap != null) {
            mActivity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    ImageView publisherFaviconIcon =
                            mPopupView.findViewById(R.id.publisher_favicon);
                    publisherFaviconIcon.setImageBitmap(
                            LuxxleRewardsHelper.getCircularBitmap(bitmap));

                    View fadeout = mPopupView.findViewById(R.id.publisher_favicon_update);
                    LuxxleRewardsHelper.crossfade(fadeout, publisherFaviconIcon, View.GONE, 1f,
                            LuxxleRewardsHelper.CROSS_FADE_DURATION);
                }
            });
        }
    }

    @Nullable
    private Intent buildUserWalletActivityIntent(final int status) {
        if (mExternalWallet == null) {
            return null;
        }

        Class clazz = null;
        switch (status) {
            case WalletStatus.CONNECTED:
            case WalletStatus.LOGGED_OUT:
                clazz = LuxxleRewardsUserWalletActivity.class;
                break;
            default:
                Log.e(TAG, "Unexpected external wallet status");
                return null;
        }

        Intent intent = new Intent(ContextUtils.getApplicationContext(), clazz);
        intent.putExtra(LuxxleRewardsExternalWallet.ACCOUNT_URL, mExternalWallet.getAccountUrl());
        intent.putExtra(LuxxleRewardsExternalWallet.ADDRESS, mExternalWallet.getAddress());
        intent.putExtra(LuxxleRewardsExternalWallet.STATUS, mExternalWallet.getStatus());
        intent.putExtra(LuxxleRewardsExternalWallet.TOKEN, mExternalWallet.getToken());
        intent.putExtra(LuxxleRewardsExternalWallet.USER_NAME, mExternalWallet.getUserName());
        return intent;
    }

    private void enableControls(boolean enable, ViewGroup vg) {
        for (int i = 0; i < vg.getChildCount(); i++) {
            View child = vg.getChildAt(i);
            if (child.getId() != R.id.tip_btn) {
                child.setEnabled(enable);
            }
            if (child instanceof ViewGroup) {
                enableControls(enable, (ViewGroup) child);
            }
        }
    }

    private int getWalletIcon(String walletType) {
        if (walletType.equals(LuxxleWalletProvider.UPHOLD)) {
            return R.drawable.uphold_white;
        } else if (walletType.equals(LuxxleWalletProvider.GEMINI)) {
            return R.drawable.ic_gemini_logo_white;
        } else if (walletType.equals(LuxxleWalletProvider.BITFLYER)) {
            return R.drawable.ic_logo_bitflyer;
        } else if (walletType.equals(LuxxleWalletProvider.ZEBPAY)) {
            return R.drawable.ic_logo_zebpay_white;
        } else {
            return R.drawable.ic_logo_solana;
        }
    }

    private void adjustTouchTargets() {
        LuxxleTouchUtils.ensureMinTouchTarget(
                mPopupView.findViewById(R.id.ads_seen_this_month_text));
        LuxxleTouchUtils.ensureMinTouchTarget(
                mPopupView.findViewById(R.id.bat_ads_balance_learn_more_text));
        LuxxleTouchUtils.ensureMinTouchTarget(mPopupView.findViewById(R.id.btn_verify_wallet));
    }
}
