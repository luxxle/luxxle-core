/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.toolbar.top;

import static org.chromium.ui.base.ViewUtils.dpToPx;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.app.Activity;
import android.content.Context;
import android.content.res.ColorStateList;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Canvas;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.util.AttributeSet;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.core.content.ContextCompat;
import androidx.core.content.res.ResourcesCompat;
import androidx.core.widget.ImageViewCompat;

import com.luxxle.playlist.enums.PlaylistOptionsEnum;
import com.luxxle.playlist.listener.PlaylistOnboardingActionClickListener;
import com.luxxle.playlist.listener.PlaylistOptionsListener;
import com.luxxle.playlist.model.PlaylistOptionsModel;
import com.luxxle.playlist.model.SnackBarActionModel;
import com.luxxle.playlist.util.ConstantUtils;
import com.luxxle.playlist.util.PlaylistViewUtils;

import org.chromium.base.ApiCompatibilityUtils;
import org.chromium.base.LuxxleFeatureList;
import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.base.LuxxleReflectionUtil;
import org.chromium.base.Log;
import org.chromium.base.MathUtils;
import org.chromium.base.ThreadUtils;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.task.AsyncTask;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleRewardsHelper;
import org.chromium.chrome.browser.LuxxleRewardsNativeWorker;
import org.chromium.chrome.browser.LuxxleRewardsObserver;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.luxxle_stats.LuxxleStatsUtil;
import org.chromium.chrome.browser.crypto_wallet.controller.DAppsWalletController;
import org.chromium.chrome.browser.custom_layout.popup_window_tooltip.PopupWindowTooltip;
import org.chromium.chrome.browser.customtabs.FullScreenCustomTabActivity;
import org.chromium.chrome.browser.customtabs.features.toolbar.CustomTabToolbar;
import org.chromium.chrome.browser.dialogs.LuxxleAdsSignupDialog;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.local_database.LuxxleStatsTable;
import org.chromium.chrome.browser.local_database.DatabaseHelper;
import org.chromium.chrome.browser.local_database.SavedBandwidthTable;
import org.chromium.chrome.browser.ntp.NtpUtil;
import org.chromium.chrome.browser.omnibox.LuxxleLocationBarCoordinator;
import org.chromium.chrome.browser.omnibox.LocationBarCoordinator;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.onboarding.v2.HighlightItem;
import org.chromium.chrome.browser.onboarding.v2.HighlightView;
import org.chromium.chrome.browser.playlist.PlaylistServiceFactoryAndroid;
import org.chromium.chrome.browser.playlist.PlaylistServiceObserverImpl;
import org.chromium.chrome.browser.playlist.PlaylistServiceObserverImpl.PlaylistServiceObserverImplDelegate;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.preferences.website.LuxxleShieldsContentSettings;
import org.chromium.chrome.browser.preferences.website.LuxxleShieldsContentSettingsObserver;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.rewards.LuxxleRewardsPanel;
import org.chromium.chrome.browser.rewards.onboarding.RewardsOnboarding;
import org.chromium.chrome.browser.shields.LuxxleShieldsHandler;
import org.chromium.chrome.browser.shields.LuxxleShieldsMenuObserver;
import org.chromium.chrome.browser.shields.LuxxleShieldsUtils;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabHidingType;
import org.chromium.chrome.browser.tab.TabSelectionType;
import org.chromium.chrome.browser.tabmodel.TabModelSelector;
import org.chromium.chrome.browser.tabmodel.TabModelSelectorTabModelObserver;
import org.chromium.chrome.browser.tabmodel.TabModelSelectorTabObserver;
import org.chromium.chrome.browser.theme.ThemeUtils;
import org.chromium.chrome.browser.toolbar.ToolbarDataProvider;
import org.chromium.chrome.browser.toolbar.ToolbarProgressBar;
import org.chromium.chrome.browser.toolbar.ToolbarTabController;
import org.chromium.chrome.browser.toolbar.back_button.BackButtonCoordinator;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarVariationManager;
import org.chromium.chrome.browser.toolbar.home_button.HomeButton;
import org.chromium.chrome.browser.toolbar.menu_button.LuxxleMenuButtonCoordinator;
import org.chromium.chrome.browser.toolbar.menu_button.MenuButtonCoordinator;
import org.chromium.chrome.browser.toolbar.reload_button.ReloadButtonCoordinator;
import org.chromium.chrome.browser.toolbar.top.NavigationPopup.HistoryDelegate;
import org.chromium.chrome.browser.user_education.UserEducationHelper;
import org.chromium.chrome.browser.util.LuxxleConstants;
import org.chromium.chrome.browser.util.LuxxleTouchUtils;
import org.chromium.chrome.browser.util.ConfigurationUtils;
import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.components.embedder_support.util.UrlUtilities;
import org.chromium.components.feature_engagement.Tracker;
import org.chromium.content_public.browser.NavigationHandle;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.playlist.mojom.PlaylistItem;
import org.chromium.playlist.mojom.PlaylistService;
import org.chromium.ui.UiUtils;
import org.chromium.ui.base.DeviceFormFactor;
import org.chromium.ui.base.ViewUtils;
import org.chromium.ui.interpolators.Interpolators;
import org.chromium.ui.util.ColorUtils;
import org.chromium.ui.widget.Toast;
import org.chromium.url.GURL;
import org.chromium.url.mojom.Url;

import java.net.URL;
import java.net.URLEncoder;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Collections;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;

public abstract class LuxxleToolbarLayoutImpl extends ToolbarLayout
        implements LuxxleToolbarLayout,
                OnClickListener,
                View.OnLongClickListener,
                LuxxleRewardsObserver,
                LuxxleRewardsNativeWorker.PublisherObserver,
                ConnectionErrorHandler,
                PlaylistServiceObserverImplDelegate {
    private static final String TAG = "LuxxleToolbar";

    private static final int URL_FOCUS_TOOLBAR_BUTTONS_TRANSLATION_X_DP = 10;

    private static final int DAYS_7 = 7;
    public static boolean mShouldShowPlaylistMenu;

    private PlaylistServiceObserverImpl mPlaylistServiceObserver;

    private DatabaseHelper mDatabaseHelper = DatabaseHelper.getInstance();

    private ImageButton mLuxxleWalletButton;
    private ImageButton mLuxxleShieldsButton;
    private ImageButton mLuxxleRewardsButton;
    private HomeButton mHomeButton;
    private FrameLayout mWalletLayout;
    private FrameLayout mShieldsLayout;
    private FrameLayout mRewardsLayout;
    private LuxxleShieldsHandler mLuxxleShieldsHandler;

    // TabModelSelectorTabObserver setups observer at the ctor
    @SuppressWarnings("UnusedVariable")
    private TabModelSelectorTabObserver mTabModelSelectorTabObserver;

    // TabModelSelectorTabModelObserver setups observer at the ctor
    @SuppressWarnings("UnusedVariable")
    private TabModelSelectorTabModelObserver mTabModelSelectorTabModelObserver;

    private LuxxleRewardsNativeWorker mLuxxleRewardsNativeWorker;
    private LuxxleRewardsPanel mRewardsPopup;
    private DAppsWalletController mDAppsWalletController;
    private LuxxleShieldsContentSettings mLuxxleShieldsContentSettings;
    private LuxxleShieldsContentSettingsObserver mLuxxleShieldsContentSettingsObserver;
    private TextView mLuxxleRewardsNotificationsCount;
    private ImageView mLuxxleRewardsOnboardingIcon;
    private View mLuxxleWalletBadge;
    private ImageView mWalletIcon;
    private int mCurrentToolbarColor;

    private boolean mIsPublisherVerified;
    private String mPublisherId;
    private boolean mIsNotificationPosted;
    private boolean mIsInitialNotificationPosted; // initial red circle notification

    private PopupWindowTooltip mShieldsPopupWindowTooltip;

    private boolean mIsBottomControlsVisible;

    private ColorStateList mDarkModeTint;
    private ColorStateList mLightModeTint;

    private final Set<Integer> mTabsWithWalletIcon =
            Collections.synchronizedSet(new HashSet<Integer>());

    private PlaylistService mPlaylistService;

    private enum BigtechCompany { Google, Facebook, Amazon }

    public LuxxleToolbarLayoutImpl(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void destroy() {
        if (mLuxxleShieldsContentSettings != null) {
            mLuxxleShieldsContentSettings.removeObserver(mLuxxleShieldsContentSettingsObserver);
        }
        if (mPlaylistService != null) {
            mPlaylistService.close();
        }
        if (mPlaylistServiceObserver != null) {
            mPlaylistServiceObserver.close();
            mPlaylistServiceObserver.destroy();
            mPlaylistServiceObserver = null;
        }
        super.destroy();
        if (mLuxxleRewardsNativeWorker != null) {
            mLuxxleRewardsNativeWorker.removeObserver(this);
            mLuxxleRewardsNativeWorker.removePublisherObserver(this);
        }
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        if (LuxxleReflectionUtil.equalTypes(this.getClass(), ToolbarTablet.class)) {
            ImageButton forwardButton = findViewById(R.id.forward_button);
            if (forwardButton != null) {
                final Drawable forwardButtonDrawable =
                        UiUtils.getTintedDrawable(
                                getContext(),
                                R.drawable.btn_right_tablet,
                                R.color.default_icon_color_tint_list);
                forwardButton.setImageDrawable(forwardButtonDrawable);
            }
        }

        mWalletLayout = (FrameLayout) findViewById(R.id.luxxle_wallet_button_layout);
        mShieldsLayout = (FrameLayout) findViewById(R.id.luxxle_shields_button_layout);
        mRewardsLayout = (FrameLayout) findViewById(R.id.luxxle_rewards_button_layout);
        mLuxxleRewardsNotificationsCount = (TextView) findViewById(R.id.br_notifications_count);
        mLuxxleRewardsOnboardingIcon = findViewById(R.id.br_rewards_onboarding_icon);
        mLuxxleWalletButton = (ImageButton) findViewById(R.id.luxxle_wallet_button);
        mLuxxleShieldsButton = (ImageButton) findViewById(R.id.luxxle_shields_button);
        mLuxxleRewardsButton = (ImageButton) findViewById(R.id.luxxle_rewards_button);
        mHomeButton = (HomeButton) findViewById(R.id.home_button);
        mLuxxleWalletBadge = findViewById(R.id.wallet_notfication_badge);
        if (mWalletLayout != null) {
            mWalletIcon = mWalletLayout.findViewById(R.id.luxxle_wallet_button);
        }

        mDarkModeTint = ThemeUtils.getThemedToolbarIconTint(getContext(), false);
        mLightModeTint =
                ColorStateList.valueOf(ContextCompat.getColor(getContext(), R.color.luxxle_white));

        if (mHomeButton != null) {
            mHomeButton.setOnLongClickListener(this);
        }

        if (mLuxxleShieldsButton != null) {
            mLuxxleShieldsButton.setClickable(true);
            mLuxxleShieldsButton.setOnClickListener(this);
            mLuxxleShieldsButton.setOnLongClickListener(this);
            LuxxleTouchUtils.ensureMinTouchTarget(mLuxxleShieldsButton);
        }

        if (mLuxxleRewardsButton != null) {
            mLuxxleRewardsButton.setClickable(true);
            mLuxxleRewardsButton.setOnClickListener(this);
            mLuxxleRewardsButton.setOnLongClickListener(this);
            LuxxleTouchUtils.ensureMinTouchTarget(mLuxxleRewardsButton);
        }

        if (mLuxxleWalletButton != null) {
            mLuxxleWalletButton.setClickable(true);
            mLuxxleWalletButton.setOnClickListener(this);
            mLuxxleWalletButton.setOnLongClickListener(this);
            LuxxleTouchUtils.ensureMinTouchTarget(mLuxxleWalletButton);
        }

        mLuxxleShieldsHandler = new LuxxleShieldsHandler(getContext());
        mLuxxleShieldsHandler.addObserver(
                new LuxxleShieldsMenuObserver() {
                    @Override
                    public void onMenuTopShieldsChanged(boolean isOn, boolean isTopShield) {
                        Tab currentTab = getToolbarDataProvider().getTab();
                        if (currentTab == null) {
                            return;
                        }
                        if (isTopShield) {
                            updateLuxxleShieldsButtonState(currentTab);
                        }
                        if (currentTab.isLoading()) {
                            currentTab.stopLoading();
                        }
                        currentTab.reloadIgnoringCache();
                        if (null != mLuxxleShieldsHandler) {
                            // Clean the Bravery Panel
                            mLuxxleShieldsHandler.updateValues(0, 0, 0);
                        }
                    }
                });
        mLuxxleShieldsContentSettingsObserver = new LuxxleShieldsContentSettingsObserver() {
            @Override
            public void blockEvent(int tabId, String blockType, String subresource) {
                mLuxxleShieldsHandler.addStat(tabId, blockType, subresource);
                Tab currentTab = getToolbarDataProvider().getTab();
                if (currentTab == null || currentTab.getId() != tabId) {
                    return;
                }
                mLuxxleShieldsHandler.updateValues(tabId);
                if (!isIncognito() && OnboardingPrefManager.getInstance().isLuxxleStatsEnabled()
                        && (blockType.equals(LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_ADS)
                                || blockType.equals(LuxxleShieldsContentSettings
                                                            .RESOURCE_IDENTIFIER_TRACKERS))) {
                    addStatsToDb(blockType, subresource, currentTab.getUrl().getSpec());
                }
            }

            @Override
            public void savedBandwidth(long savings) {
                if (!isIncognito() && OnboardingPrefManager.getInstance().isLuxxleStatsEnabled()) {
                    addSavedBandwidthToDb(savings);
                }
            }
        };
        // Initially show shields off image. Shields button state will be updated when tab is
        // shown and loading state is changed.
        updateLuxxleShieldsButtonState(null);
        if (LuxxleReflectionUtil.equalTypes(this.getClass(), ToolbarPhone.class)) {
            if (getMenuButtonCoordinator() != null
                    && isMenuButtonOnBottomControls()
                    && BottomToolbarConfiguration.isToolbarTopAnchored()) {
                getMenuButtonCoordinator().setVisibility(false);
            }
        }

        if (LuxxleReflectionUtil.equalTypes(this.getClass(), CustomTabToolbar.class)) {
            LinearLayout customActionButtons = findViewById(R.id.action_buttons);
            assert customActionButtons != null : "Something has changed in the upstream!";
            if (customActionButtons != null && mLuxxleShieldsButton != null) {
                ViewGroup.MarginLayoutParams luxxleShieldsButtonLayout =
                        (ViewGroup.MarginLayoutParams) mLuxxleShieldsButton.getLayoutParams();
                ViewGroup.MarginLayoutParams actionButtonsLayout =
                        (ViewGroup.MarginLayoutParams) customActionButtons.getLayoutParams();
                actionButtonsLayout.setMarginEnd(actionButtonsLayout.getMarginEnd()
                        + luxxleShieldsButtonLayout.getMarginEnd());
                customActionButtons.setLayoutParams(actionButtonsLayout);
            }
        }
        updateShieldsLayoutBackground(isIncognito() || !NtpUtil.shouldShowRewardsIcon());
    }

    public String getLocationBarQuery() {
        if (getLocationBar() instanceof LuxxleLocationBarCoordinator) {
            String query =
                    ((LuxxleLocationBarCoordinator) getLocationBar())
                            .getUrlBarTextWithoutAutocomplete();
            return query;
        }
        return "";
    }

    public void clearOmniboxFocus() {
        if (getLocationBar() instanceof LuxxleLocationBarCoordinator) {
            ((LuxxleLocationBarCoordinator) getLocationBar()).clearOmniboxFocus();
        }
    }

    public boolean isUrlBarFocused() {
        if (getLocationBar() instanceof LuxxleLocationBarCoordinator) {
            return ((LuxxleLocationBarCoordinator) getLocationBar()).isUrlBarFocused();
        }
        return false;
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (isPlaylistEnabledByPrefsAndFlags()) {
            mPlaylistService = null;
            initPlaylistService();
        }
    }

    private void initPlaylistService() {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (mPlaylistService != null || currentTab == null) {
            return;
        }

        if (currentTab.isIncognito()) {
            return;
        }

        mPlaylistService =
                PlaylistServiceFactoryAndroid.getInstance()
                        .getPlaylistService(
                                Profile.fromWebContents(currentTab.getWebContents()), this);
    }

    @Override
    public void onTermsOfServiceUpdateAccepted() {
        showOrHideRewardsBadge(false);
    }

    private void showOrHideRewardsBadge(boolean shouldShow) {
        Context context = getContext();
        if (context instanceof Activity
                && (((Activity) context).isFinishing() || ((Activity) context).isDestroyed())) {
            return;
        }
        View rewardsBadge = findViewById(R.id.rewards_notfication_badge);
        if (rewardsBadge != null) {
            rewardsBadge.setVisibility(shouldShow ? View.VISIBLE : View.GONE);
        }
    }

    @Override
    protected void onNativeLibraryReady() {
        super.onNativeLibraryReady();
        if (isPlaylistEnabledByPrefsAndFlags()) {
            initPlaylistService();
            mPlaylistServiceObserver = new PlaylistServiceObserverImpl(this);
            mPlaylistService.addObserver(mPlaylistServiceObserver);
        }

        mLuxxleShieldsContentSettings = LuxxleShieldsContentSettings.getInstance();
        mLuxxleShieldsContentSettings.addObserver(mLuxxleShieldsContentSettingsObserver);

        mLuxxleRewardsNativeWorker = LuxxleRewardsNativeWorker.getInstance();
        if (mLuxxleRewardsNativeWorker != null
                && mLuxxleRewardsNativeWorker.isSupported()
                && NtpUtil.shouldShowRewardsIcon()
                && mRewardsLayout != null) {
            mRewardsLayout.setVisibility(View.VISIBLE);
        }
        if (mLuxxleRewardsNativeWorker != null
                && mLuxxleRewardsNativeWorker.isRewardsEnabled()
                && mLuxxleRewardsNativeWorker.isSupported()
                && mLuxxleRewardsNativeWorker.isTermsOfServiceUpdateRequired()) {
            showOrHideRewardsBadge(true);
        }
        if (mShieldsLayout != null) {
            updateShieldsLayoutBackground(
                    !(mRewardsLayout != null && mRewardsLayout.getVisibility() == View.VISIBLE));
            mShieldsLayout.setVisibility(View.VISIBLE);
        }
        if (mLuxxleRewardsNativeWorker != null) {
            mLuxxleRewardsNativeWorker.addObserver(this);
            mLuxxleRewardsNativeWorker.addPublisherObserver(this);
            mLuxxleRewardsNativeWorker.getAllNotifications();
        }
    }

    public void setTabModelSelector(TabModelSelector selector) {
        // We might miss events before calling setTabModelSelector, so we need
        // to proactively update the shields button state here, otherwise shields
        // might sometimes show as disabled while it is actually enabled.
        updateLuxxleShieldsButtonState(getToolbarDataProvider().getTab());
        mTabModelSelectorTabObserver =
                new TabModelSelectorTabObserver(selector) {
                    @Override
                    protected void onTabRegistered(Tab tab) {
                        super.onTabRegistered(tab);
                        if (tab.isIncognito()) {
                            showWalletIcon(false);
                        }
                    }

                    @Override
                    public void onShown(Tab tab, @TabSelectionType int type) {
                        // Update shields button state when visible tab is changed.
                        updateLuxxleShieldsButtonState(tab);
                        // case when window.open is triggered from dapps site and new tab is in
                        // focus
                        if (type != TabSelectionType.FROM_USER) {
                            dismissWalletPanelOrDialog();
                        }
                        findMediaFiles();
                    }

                    @Override
                    public void onHidden(Tab tab, @TabHidingType int reason) {
                        hidePlaylistButton();
                    }

                    @Override
                    public void onPageLoadStarted(Tab tab, GURL url) {
                        showWalletIcon(false, tab);
                        if (getToolbarDataProvider().getTab() == tab) {
                            updateLuxxleShieldsButtonState(tab);
                        }
                        mLuxxleShieldsHandler.clearLuxxleShieldsCount(tab.getId());
                        dismissShieldsTooltip();
                        hidePlaylistButton();
                        mPublisherId = "";
                    }

                    @Override
                    public void onPageLoadFinished(final Tab tab, GURL url) {
                        if (getToolbarDataProvider().getTab() == tab) {
                            mLuxxleShieldsHandler.updateUrlSpec(url.getSpec());
                            updateLuxxleShieldsButtonState(tab);

                            if (mLuxxleShieldsButton != null
                                    && mLuxxleShieldsButton.isShown()
                                    && mLuxxleShieldsHandler != null
                                    && !mLuxxleShieldsHandler.isShowing()) {
                                checkForTooltip(tab);
                            }
                        }

                        String countryCode = Locale.getDefault().getCountry();
                        if (countryCode.equals(LuxxleConstants.INDIA_COUNTRY_CODE)
                                && url.domainIs(LuxxleConstants.YOUTUBE_DOMAIN)
                                && ChromeSharedPreferences.getInstance()
                                        .readBoolean(
                                                LuxxlePreferenceKeys.LUXXLE_AD_FREE_CALLOUT_DIALOG,
                                                true)) {
                            ChromeSharedPreferences.getInstance()
                                    .writeBoolean(LuxxlePreferenceKeys.LUXXLE_OPENED_YOUTUBE, true);
                        }
                    }

                    @Override
                    public void onDidFinishNavigationInPrimaryMainFrame(
                            Tab tab, NavigationHandle navigation) {
                        if (mLuxxleRewardsNativeWorker != null) {
                            mLuxxleRewardsNativeWorker.triggerOnNotifyFrontTabUrlChanged();
                        }
                        if (getToolbarDataProvider().getTab() == tab
                                && mLuxxleRewardsNativeWorker != null
                                && !tab.isIncognito()) {
                            mLuxxleRewardsNativeWorker.onNotifyFrontTabUrlChanged(
                                    tab.getId(), tab.getUrl().getSpec());
                        }
                        if (!LuxxleRewardsHelper.shouldShowNewRewardsUI()
                                && PackageUtils.isFirstInstall(getContext())
                                && tab.getUrl().getSpec() != null
                                && tab.getUrl()
                                        .getSpec()
                                        .equals(LuxxleActivity.LUXXLE_REWARDS_SETTINGS_URL)
                                && LuxxleRewardsHelper.shouldShowLuxxleRewardsOnboardingModal()
                                && mLuxxleRewardsNativeWorker != null
                                && !mLuxxleRewardsNativeWorker.isRewardsEnabled()
                                && mLuxxleRewardsNativeWorker.isSupported()) {
                            showOnBoarding();
                        }
                        hidePlaylistButton();
                    }

                    @Override
                    public void onDestroyed(Tab tab) {
                        // Remove references for the ads from the Database. Tab is destroyed, they
                        // are not
                        // needed anymore.
                        new Thread() {
                            @Override
                            public void run() {
                                mDatabaseHelper.deleteDisplayAdsFromTab(tab.getId());
                            }
                        }.start();
                        mLuxxleShieldsHandler.removeStat(tab.getId());
                        mTabsWithWalletIcon.remove(tab.getId());
                    }
                };

        mTabModelSelectorTabModelObserver =
                new TabModelSelectorTabModelObserver(selector) {
                    @Override
                    public void didSelectTab(Tab tab, @TabSelectionType int type, int lastId) {
                        if (mLuxxleRewardsNativeWorker != null && !tab.isIncognito()) {
                            mLuxxleRewardsNativeWorker.onNotifyFrontTabUrlChanged(
                                    tab.getId(), tab.getUrl().getSpec());
                            Tab providerTab = getToolbarDataProvider().getTab();
                            if (providerTab != null && providerTab.getId() == tab.getId()) {
                                showWalletIcon(mTabsWithWalletIcon.contains(tab.getId()));
                            } else if (mWalletLayout != null) {
                                mWalletLayout.setVisibility(
                                        mTabsWithWalletIcon.contains(tab.getId())
                                                ? View.VISIBLE
                                                : View.GONE);
                            }
                        }
                    }
                };
    }

    private void showOnBoarding() {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            int deviceWidth = ConfigurationUtils.getDisplayMetricsWidth(activity);
            boolean isTablet = DeviceFormFactor.isNonMultiDisplayContextOnTablet(activity);
            deviceWidth = (int) (isTablet ? (deviceWidth * 0.6) : (deviceWidth * 0.95));
            RewardsOnboarding panel = new RewardsOnboarding(mLuxxleRewardsButton, deviceWidth);
            panel.showLikePopDownMenu();
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "RewardsOnboarding failed " + e);
        }
    }

    private boolean isPlaylistEnabledByPrefsAndFlags() {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (currentTab == null) {
            return false;
        }
        return ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_PLAYLIST)
                && ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePreferenceKeys.PREF_ENABLE_PLAYLIST, true)
                && !currentTab.isIncognito();
    }

    private void hidePlaylistButton() {
        mShouldShowPlaylistMenu = false;
        try {
            ViewGroup viewGroup =
                    LuxxleActivity.getLuxxleActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);
            View playlistButton = viewGroup.findViewById(R.id.playlist_button_id);
            if (playlistButton != null && playlistButton.getVisibility() == View.VISIBLE) {
                playlistButton.setVisibility(View.GONE);
            }
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "hidePlaylistButton " + e);
        }
    }

    private boolean isPlaylistButtonVisible() {
        try {
            ViewGroup viewGroup =
                    LuxxleActivity.getLuxxleActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);
            View playlistButton = viewGroup.findViewById(R.id.playlist_button_id);
            return playlistButton != null && playlistButton.getVisibility() == View.VISIBLE;
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "isPlaylistButtonVisible " + e);
            return false;
        }
    }

    private void findMediaFiles() {
        if (mPlaylistService != null && isPlaylistEnabledByPrefsAndFlags()) {
            hidePlaylistButton();
            mPlaylistService.findMediaFilesFromActiveTab();
        }
    }

    private void showPlaylistButton(PlaylistItem[] items) {
        try {
            ViewGroup viewGroup =
                    LuxxleActivity.getLuxxleActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);

            PlaylistOptionsListener playlistOptionsListener =
                    new PlaylistOptionsListener() {
                        @Override
                        public void onPlaylistOptionClicked(
                                PlaylistOptionsModel playlistOptionsModel) {
                            try {
                                if (playlistOptionsModel.getOptionType()
                                        == PlaylistOptionsEnum.ADD_MEDIA) {
                                    addMediaToPlaylist(items);
                                } else if (playlistOptionsModel.getOptionType()
                                        == PlaylistOptionsEnum.OPEN_PLAYLIST) {
                                    LuxxleActivity.getLuxxleActivity()
                                            .openPlaylistActivity(
                                                    getContext(), ConstantUtils.DEFAULT_PLAYLIST);
                                } else if (playlistOptionsModel.getOptionType()
                                        == PlaylistOptionsEnum.PLAYLIST_SETTINGS) {
                                    LuxxleActivity.getLuxxleActivity().openLuxxlePlaylistSettings();
                                }
                            } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
                                Log.e(TAG, "showPlaylistButton onOptionClicked " + e);
                            }
                        }
                    };
            if (!isPlaylistButtonVisible()) {
                PlaylistOnboardingActionClickListener playlistOnboardingActionClickListener =
                        new PlaylistOnboardingActionClickListener() {
                            @Override
                            public void onOnboardingActionClick() {
                                addMediaToPlaylist(items);
                            }
                        };

                PlaylistViewUtils.showPlaylistButton(
                        LuxxleActivity.getLuxxleActivity(),
                        viewGroup,
                        playlistOptionsListener,
                        playlistOnboardingActionClickListener);
            }
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "showPlaylistButton " + e);
        }
    }

    private void addMediaToPlaylist(PlaylistItem[] items) {
        if (mPlaylistService == null) {
            return;
        }
        mPlaylistService.getPlaylist(
                ConstantUtils.DEFAULT_PLAYLIST,
                defaultPlaylist -> {
                    Set<String> pageSources = new HashSet<String>();
                    for (PlaylistItem defaultPlaylistItem : defaultPlaylist.items) {
                        pageSources.add(defaultPlaylistItem.pageSource.url);
                    }
                    List<PlaylistItem> playlistItems = new ArrayList();
                    for (PlaylistItem playlistItem : items) {
                        // Check for duplicates in default playlist
                        if (!pageSources.contains(playlistItem.pageSource.url)) {
                            playlistItems.add(playlistItem);
                        }
                    }
                    if (playlistItems.size() > 0) {
                        mPlaylistService.addMediaFiles(
                                playlistItems.toArray(new PlaylistItem[0]),
                                ConstantUtils.DEFAULT_PLAYLIST,
                                true,
                                addedItems -> {
                                    if (addedItems.length > 0) {
                                        showAddedToPlaylistSnackBar();
                                    }
                                });
                    } else {
                        showAlreadyAddedToPlaylistSnackBar();
                    }
                });
    }

    public void addMediaToPlaylist() {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (mPlaylistService == null || currentTab == null) {
            return;
        }
        mPlaylistService.addMediaFilesFromActiveTabToPlaylist(
                ConstantUtils.DEFAULT_PLAYLIST,
                true,
                addedItems -> {
                    if (addedItems.length > 0) {
                        showAddedToPlaylistSnackBar();
                    }
                });
    }

    private void showAddedToPlaylistSnackBar() {
        SnackBarActionModel snackBarActionModel =
                new SnackBarActionModel(
                        getContext().getResources().getString(R.string.view_action),
                        new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                try {
                                    LuxxleActivity.getLuxxleActivity()
                                            .openPlaylistActivity(
                                                    getContext(), ConstantUtils.DEFAULT_PLAYLIST);
                                } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
                                    Log.e(TAG, "showAddedToPlaylistSnackBar onClick ", e);
                                }
                            }
                        });
        try {
            ViewGroup viewGroup =
                    LuxxleActivity.getLuxxleActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);
            String playlistName =
                    getContext().getResources().getString(R.string.playlist_play_later);
            PlaylistViewUtils.showSnackBarWithActions(
                    viewGroup,
                    String.format(
                            getContext().getResources().getString(R.string.added_to_playlist),
                            playlistName),
                    snackBarActionModel);
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "showAddedToPlaylistSnackBar ", e);
        }
    }

    private void showAlreadyAddedToPlaylistSnackBar() {
        SnackBarActionModel snackBarActionModel =
                new SnackBarActionModel(
                        getContext().getResources().getString(R.string.close_text),
                        new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                // Do nothing
                            }
                        });
        try {
            ViewGroup viewGroup =
                    LuxxleActivity.getLuxxleActivity()
                            .getWindow()
                            .getDecorView()
                            .findViewById(android.R.id.content);
            PlaylistViewUtils.showSnackBarWithActions(
                    viewGroup,
                    getContext().getResources().getString(R.string.already_added_in_playlist),
                    snackBarActionModel);
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "showAlreadyAddedToPlaylistSnackBar " + e);
        }
    }

    private void checkForTooltip(Tab tab) {
        // We are disabling this feature for now for bottom address bar, until new design is ready
        // https://github.com/luxxle/luxxle-browser/issues/46252
        if (BottomToolbarConfiguration.isToolbarBottomAnchored()) return;
        try {
            if (!LuxxleShieldsUtils.isTooltipShown
                    && !LuxxleActivity.getLuxxleActivity().mIsDeepLink) {
                if (!LuxxleShieldsUtils.hasShieldsTooltipShown(
                            LuxxleShieldsUtils.PREF_SHIELDS_TOOLTIP)
                        && mLuxxleShieldsHandler.getTrackersBlockedCount(tab.getId())
                                        + mLuxxleShieldsHandler.getAdsBlockedCount(tab.getId())
                                > 0) {
                    showTooltip(LuxxleShieldsUtils.PREF_SHIELDS_TOOLTIP, tab.getId());
                }
            }
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "checkForTooltip " + e);
        }
    }

    private void showTooltip(String tooltipPref, int tabId) {
        try {
            HighlightView highlightView = new HighlightView(getContext(), null);
            highlightView.setColor(ContextCompat.getColor(
                    getContext(), R.color.onboarding_search_highlight_color));
            ViewGroup viewGroup =
                    LuxxleActivity.getLuxxleActivity().getWindow().getDecorView().findViewById(
                            android.R.id.content);
            float padding = (float) dpToPx(getContext(), 20);
            mShieldsPopupWindowTooltip =
                    new PopupWindowTooltip.Builder(getContext())
                            .anchorView(mLuxxleShieldsButton)
                            .arrowColor(ContextCompat.getColor(
                                    getContext(), R.color.onboarding_arrow_color))
                            .gravity(Gravity.BOTTOM)
                            .dismissOnOutsideTouch(true)
                            .dismissOnInsideTouch(false)
                            .backgroundDimDisabled(true)
                            .padding(padding)
                            .parentPaddingHorizontal(dpToPx(getContext(), 10))
                            .modal(true)
                            .onDismissListener(tooltip -> {
                                if (viewGroup != null && highlightView != null) {
                                    highlightView.stopAnimation();
                                    viewGroup.removeView(highlightView);
                                }
                            })
                            .contentView(R.layout.luxxle_shields_tooltip_layout)
                            .build();

            ArrayList<String> blockerNamesList = mLuxxleShieldsHandler.getBlockerNamesList(tabId);

            int adsTrackersCount = mLuxxleShieldsHandler.getTrackersBlockedCount(tabId)
                    + mLuxxleShieldsHandler.getAdsBlockedCount(tabId);

            String displayTrackerName = "";
            if (blockerNamesList.contains(BigtechCompany.Google.name())) {
                displayTrackerName = BigtechCompany.Google.name();
            } else if (blockerNamesList.contains(BigtechCompany.Facebook.name())) {
                displayTrackerName = BigtechCompany.Facebook.name();
            } else if (blockerNamesList.contains(BigtechCompany.Amazon.name())) {
                displayTrackerName = BigtechCompany.Amazon.name();
            }

            String trackerText = "";
            if (!displayTrackerName.isEmpty()) {
                if (adsTrackersCount - 1 == 0) {
                    trackerText =
                            String.format(getContext().getResources().getString(
                                                  R.string.shield_bigtech_tracker_only_blocked),
                                    displayTrackerName);

                } else {
                    trackerText = String.format(getContext().getResources().getString(
                                                        R.string.shield_bigtech_tracker_blocked),
                            displayTrackerName, String.valueOf(adsTrackersCount - 1));
                }
            } else {
                trackerText = String.format(
                        getContext().getResources().getString(R.string.shield_tracker_blocked),
                        String.valueOf(adsTrackersCount));
            }

            TextView tvBlocked = mShieldsPopupWindowTooltip.findViewById(R.id.tv_blocked);
            tvBlocked.setText(trackerText);

            if (mLuxxleShieldsButton != null && mLuxxleShieldsButton.isShown()) {
                viewGroup.addView(highlightView);
                HighlightItem item = new HighlightItem(mLuxxleShieldsButton);

                ImageButton luxxleShieldButton =
                        new ImageButton(getContext(), null, R.style.ToolbarButton);
                luxxleShieldButton.setImageResource(R.drawable.btn_luxxle);
                FrameLayout.LayoutParams luxxleShieldParams =
                        new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT,
                                FrameLayout.LayoutParams.WRAP_CONTENT);

                int[] location = new int[2];
                highlightView.getLocationOnScreen(location);
                luxxleShieldParams.leftMargin = item.getScreenLeft() + dpToPx(getContext(), 10);
                luxxleShieldParams.topMargin = item.getScreenTop()
                        + ((item.getScreenBottom() - item.getScreenTop()) / 4) - location[1];
                luxxleShieldButton.setLayoutParams(luxxleShieldParams);
                highlightView.addView(luxxleShieldButton);

                highlightView.setShouldShowHighlight(true);
                highlightView.setHighlightTransparent(true);
                highlightView.setHighlightItem(item);
                highlightView.initializeAnimators();
                highlightView.startAnimation();

                mShieldsPopupWindowTooltip.show();
                LuxxleShieldsUtils.setShieldsTooltipShown(tooltipPref, true);
                LuxxleShieldsUtils.isTooltipShown = true;
            }

        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "showTooltip " + e);
        }
    }

    public void dismissShieldsTooltip() {
        if (mShieldsPopupWindowTooltip != null && mShieldsPopupWindowTooltip.isShowing()) {
            mShieldsPopupWindowTooltip.dismiss();
            mShieldsPopupWindowTooltip = null;
        }
    }

    public void reopenShieldsPanel() {
        if (mLuxxleShieldsHandler != null && mLuxxleShieldsHandler.isShowing()) {
            mLuxxleShieldsHandler.hideLuxxleShieldsMenu();
            showShieldsMenu(mLuxxleShieldsButton);
        }
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        Context context = getContext();
        if (context instanceof Activity
                && (((Activity) context).isFinishing() || ((Activity) context).isDestroyed())) {
            return;
        }
        dismissShieldsTooltip();
        reopenShieldsPanel();
        // TODO: show wallet panel
    }

    private void addSavedBandwidthToDb(long savings) {
        new AsyncTask<Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    SavedBandwidthTable savedBandwidthTable =
                            new SavedBandwidthTable(
                                    savings, LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                    long unused_rowId = mDatabaseHelper.insertSavedBandwidth(savedBandwidthTable);
                } catch (Exception e) {
                    // Do nothing if url is invalid.
                    // Just return w/o showing shields popup.
                    return null;
                }
                return null;
            }

            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    private void addStatsToDb(String statType, String statSite, String url) {
        new AsyncTask<Void>() {
            @Override
            protected Void doInBackground() {
                try {
                    URL urlObject = new URL(url);
                    URL siteObject = new URL(statSite);
                    LuxxleStatsTable luxxleStatsTable =
                            new LuxxleStatsTable(
                                    url,
                                    urlObject.getHost(),
                                    statType,
                                    statSite,
                                    siteObject.getHost(),
                                    LuxxleStatsUtil.getCalculatedDate("yyyy-MM-dd", 0));
                    long unused_rowId = mDatabaseHelper.insertStats(luxxleStatsTable);
                } catch (Exception e) {
                    // Do nothing if url is invalid.
                    // Just return w/o showing shields popup.
                    return null;
                }
                return null;
            }

            @Override
            protected void onPostExecute(Void result) {
                assert ThreadUtils.runningOnUiThread();
                if (isCancelled()) return;
            }
        }.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    public boolean isWalletIconVisible() {
        if (mWalletLayout == null) {
            return false;
        }
        return mWalletLayout.getVisibility() == View.VISIBLE;
    }

    public void showWalletIcon(boolean show, Tab tab) {
        // The layout could be null in Custom Tabs layout
        if (mWalletLayout == null) {
            return;
        }
        Tab currentTab = tab;
        if (currentTab == null) {
            currentTab = getToolbarDataProvider().getTab();
            if (currentTab == null) {
                return;
            }
        }
        if (show) {
            mWalletLayout.setVisibility(View.VISIBLE);
            mTabsWithWalletIcon.add(currentTab.getId());
        } else {
            mWalletLayout.setVisibility(View.GONE);
            mTabsWithWalletIcon.remove(currentTab.getId());
        }
    }

    public void showWalletIcon(boolean show) {
        showWalletIcon(show, null);
    }

    public void hideRewardsOnboardingIcon() {
        if (mLuxxleRewardsOnboardingIcon != null) {
            mLuxxleRewardsOnboardingIcon.setVisibility(View.GONE);
        }
        if (mLuxxleRewardsNotificationsCount != null) {
            mLuxxleRewardsNotificationsCount.setVisibility(View.GONE);
        }
        ChromeSharedPreferences.getInstance()
                .writeBoolean(LuxxleRewardsPanel.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, true);
    }

    @Override
    public void onClickImpl(View v) {
        if (mLuxxleShieldsHandler == null) {
            assert false;
            return;
        }
        if (mLuxxleShieldsButton == v && mLuxxleShieldsButton != null) {
            showShieldsMenu(mLuxxleShieldsButton);
        } else if (mLuxxleRewardsButton == v && mLuxxleRewardsButton != null) {
            if (null != mRewardsPopup) {
                return;
            }
            hideRewardsOnboardingIcon();
            OnboardingPrefManager.getInstance().setOnboardingShown(true);
            if (LuxxleRewardsHelper.shouldShowNewRewardsUI()) {
                showRewardsPage();
            } else {
                mRewardsPopup = new LuxxleRewardsPanel(v);
                mRewardsPopup.showLikePopDownMenu();
            }

            if (mLuxxleRewardsNotificationsCount.isShown()) {
                ChromeSharedPreferences.getInstance()
                        .writeBoolean(
                                LuxxleRewardsPanel.PREF_WAS_TOOLBAR_BAT_LOGO_BUTTON_PRESSED, true);
                mLuxxleRewardsNotificationsCount.setVisibility(View.INVISIBLE);
                mIsInitialNotificationPosted = false;
            }
        } else if (mHomeButton == v) {
            // Helps Luxxle News know how to behave on home button action
            try {
                LuxxleActivity.getLuxxleActivity().setComesFromNewTab(true);
            } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
                Log.e(TAG, "HomeButton click " + e);
            }
        } else if (mLuxxleWalletButton == v && mLuxxleWalletButton != null) {
            maybeShowWalletPanel();
        }
    }

    public void showRewardsPage() {
        String rewardsUrl = LuxxleActivity.LUXXLE_REWARDS_SETTINGS_URL + "?bubble";
        if (mPublisherId != null && !mPublisherId.isEmpty()) {
            rewardsUrl += "&creator=" + URLEncoder.encode(mPublisherId);
        }
        FullScreenCustomTabActivity.showPage(getContext(), rewardsUrl);
    }

    private void maybeShowWalletPanel() {
        try {
            LuxxleActivity activity = LuxxleActivity.getLuxxleActivity();
            activity.showWalletPanel(true);
        } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
            Log.e(TAG, "maybeShowWalletPanel " + e);
        }
    }

    private void showWalletPanelInternal(View v) {
        mDAppsWalletController =
                new DAppsWalletController(getContext(), v, dialog -> mDAppsWalletController = null);
        mDAppsWalletController.showWalletPanel();
    }

    public void showWalletPanel() {
        if (mDAppsWalletController == null) {
            showWalletPanelInternal(this);
        } else if (!mDAppsWalletController.isShowingPanel()) {
            mDAppsWalletController.showWalletPanel();
        }
    }

    @Override
    public void onClick(View v) {
        onClickImpl(v);
    }

    private boolean checkForRewardsOnboarding() {
        return PackageUtils.isFirstInstall(getContext())
                && mLuxxleRewardsNativeWorker != null
                && !mLuxxleRewardsNativeWorker.isRewardsEnabled()
                && mLuxxleRewardsNativeWorker.isSupported()
                && !OnboardingPrefManager.getInstance().isOnboardingShown()
                && (LuxxleRewardsHelper.getRewardsOnboardingIconInvisibleTiming() == 0
                        || (LuxxleRewardsHelper.getRewardsOnboardingIconInvisibleTiming() > 0
                                && System.currentTimeMillis()
                                        <= LuxxleRewardsHelper
                                                .getRewardsOnboardingIconInvisibleTiming()));
    }

    private void showShieldsMenu(View mLuxxleShieldsButton) {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (currentTab == null) {
            return;
        }
        try {
            URL url = new URL(currentTab.getUrl().getSpec());
            // Don't show shields popup if protocol is not valid for shields.
            if (!isValidProtocolForShields(url.getProtocol())) {
                return;
            }
            mLuxxleShieldsHandler.show(mLuxxleShieldsButton, currentTab);
        } catch (Exception e) {
            // Do nothing if url is invalid.
            // Just return w/o showing shields popup.
            return;
        }
    }

    @Override
    public boolean onLongClickImpl(View v) {
        // Use null as the default description since Toast.showAnchoredToast
        // will return false if it is null.
        String description = null;
        Context context = getContext();
        Resources resources = context.getResources();

        if (v == mLuxxleShieldsButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_luxxle_shields);
        } else if (v == mLuxxleRewardsButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_luxxle_rewards);
        } else if (v == mHomeButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_home);
        } else if (v == mLuxxleWalletButton) {
            description = resources.getString(R.string.accessibility_toolbar_btn_luxxle_wallet);
        }

        return Toast.showAnchoredToast(context, v, description);
    }

    @Override
    public boolean onLongClick(View v) {
        return onLongClickImpl(v);
    }

    @Override
    public void populateUrlAnimatorSetImpl(
            boolean showExpandedState,
            int urlFocusToolbarButtonsDuration,
            int urlClearFocusTabStackDelayMs,
            List<Animator> animators) {
        if (mLuxxleShieldsButton != null) {
            Animator animator;
            if (showExpandedState) {
                float density = getContext().getResources().getDisplayMetrics().density;
                boolean isRtl = getLayoutDirection() == LAYOUT_DIRECTION_RTL;
                float toolbarButtonTranslationX =
                        MathUtils.flipSignIf(URL_FOCUS_TOOLBAR_BUTTONS_TRANSLATION_X_DP, isRtl)
                        * density;
                animator = ObjectAnimator.ofFloat(
                        mLuxxleShieldsButton, TRANSLATION_X, toolbarButtonTranslationX);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setInterpolator(Interpolators.FAST_OUT_LINEAR_IN_INTERPOLATOR);
                animators.add(animator);

                animator = ObjectAnimator.ofFloat(mLuxxleShieldsButton, ALPHA, 0);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setInterpolator(Interpolators.FAST_OUT_LINEAR_IN_INTERPOLATOR);
                animators.add(animator);
            } else {
                animator = ObjectAnimator.ofFloat(mLuxxleShieldsButton, TRANSLATION_X, 0);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setStartDelay(urlClearFocusTabStackDelayMs);
                animator.setInterpolator(Interpolators.FAST_OUT_SLOW_IN_INTERPOLATOR);
                animators.add(animator);

                animator = ObjectAnimator.ofFloat(mLuxxleShieldsButton, ALPHA, 1);
                animator.setDuration(urlFocusToolbarButtonsDuration);
                animator.setStartDelay(urlClearFocusTabStackDelayMs);
                animator.setInterpolator(Interpolators.FAST_OUT_SLOW_IN_INTERPOLATOR);
                animators.add(animator);
            }
        }
    }

    @Override
    public void updateModernLocationBarColorImpl(int color) {
        mCurrentToolbarColor = color;
        if (mShieldsLayout != null) {
            mShieldsLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
        if (mRewardsLayout != null) {
            mRewardsLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
        if (mWalletLayout != null) {
            mWalletLayout.getBackground().setColorFilter(color, PorterDuff.Mode.SRC_IN);
        }
    }

    /**
     * If |tab| is null, set disabled image to shields button and |urlString| is ignored. If
     * |urlString| is null, url is fetched from |tab|.
     */
    public void updateLuxxleShieldsButtonState(Tab tab) {
        if (mLuxxleShieldsButton == null) {
            assert false;
            return;
        }

        if (tab == null) {
            mLuxxleShieldsButton.setImageResource(R.drawable.btn_luxxle_off);
            return;
        }
        mLuxxleShieldsButton.setImageResource(
                isShieldsOnForTab(tab) ? R.drawable.btn_luxxle : R.drawable.btn_luxxle_off);

        if (mRewardsLayout == null) return;
        if (isIncognito()) {
            mRewardsLayout.setVisibility(View.GONE);
            updateShieldsLayoutBackground(true);
        } else if (isNativeLibraryReady()
                && mLuxxleRewardsNativeWorker != null
                && mLuxxleRewardsNativeWorker.isSupported()
                && NtpUtil.shouldShowRewardsIcon()) {
            mRewardsLayout.setVisibility(View.VISIBLE);
            updateShieldsLayoutBackground(false);
        }
    }

    private boolean isShieldsOnForTab(Tab tab) {
        if (!isNativeLibraryReady()
                || tab == null
                || Profile.fromWebContents(tab.getWebContents()) == null) {
            return false;
        }

        return LuxxleShieldsContentSettings.getShields(
                Profile.fromWebContents(tab.getWebContents()),
                tab.getUrl().getSpec(),
                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_LUXXLE_SHIELDS);
    }

    private boolean isValidProtocolForShields(String protocol) {
        if (protocol.equals("http") || protocol.equals("https")) {
            return true;
        }

        return false;
    }

    public void dismissRewardsPanel() {
        if (mRewardsPopup != null) {
            mRewardsPopup.dismiss();
            mRewardsPopup = null;
        }
    }

    public void dismissWalletPanelOrDialog() {
        if (mDAppsWalletController != null) {
            mDAppsWalletController.dismiss();
            mDAppsWalletController = null;
        }
    }

    public void onRewardsPanelDismiss() {
        mRewardsPopup = null;
    }

    public void openRewardsPanel() {
        onClick(mLuxxleRewardsButton);
    }

    public boolean isShieldsTooltipShown() {
        if (mShieldsPopupWindowTooltip != null) {
            return mShieldsPopupWindowTooltip.isShowing();
        }
        return false;
    }

    @Override
    public void onCompleteReset(boolean success) {
        if (success) {
            LuxxleRewardsHelper.resetRewards();
            if (!LuxxleRewardsHelper.shouldShowNewRewardsUI()) {
                showOrHideRewardsBadge(false);
            }
        }
    }

    @Override
    public void onNotificationAdded(String id, int type, long timestamp, String[] args) {
        if (mLuxxleRewardsNativeWorker == null) {
            return;
        }
        mLuxxleRewardsNativeWorker.getAllNotifications();
    }

    private boolean mayShowLuxxleAdsOnboardingDialog() {
        Context context = getContext();

        if (LuxxleAdsSignupDialog.shouldShowNewUserDialog(context)) {
            LuxxleAdsSignupDialog.showNewUserDialog(getContext());
            return true;
        } else if (LuxxleAdsSignupDialog.shouldShowNewUserDialogIfRewardsIsSwitchedOff(context)) {
            LuxxleAdsSignupDialog.showNewUserDialog(getContext());
            return true;
        } else if (LuxxleAdsSignupDialog.shouldShowExistingUserDialog(context)) {
            LuxxleAdsSignupDialog.showExistingUserDialog(getContext());
            return true;
        }

        return false;
    }

    @Override
    public void onNotificationsCount(int count) {
        if (mLuxxleRewardsNotificationsCount != null) {
            if (count != 0) {
                String value = Integer.toString(count);
                if (count > 99) {
                    mLuxxleRewardsNotificationsCount.setBackground(
                            ResourcesCompat.getDrawable(getContext().getResources(),
                                    R.drawable.luxxle_rewards_rectangle, /* theme= */ null));
                    value = "99+";
                } else {
                    mLuxxleRewardsNotificationsCount.setBackground(
                            ResourcesCompat.getDrawable(getContext().getResources(),
                                    R.drawable.luxxle_rewards_circle, /* theme= */ null));
                }
                mLuxxleRewardsNotificationsCount.setText(value);
                mLuxxleRewardsNotificationsCount.setVisibility(View.VISIBLE);
                mIsNotificationPosted = true;
            } else {
                mLuxxleRewardsNotificationsCount.setText("");
                mLuxxleRewardsNotificationsCount.setBackgroundResource(0);
                mLuxxleRewardsNotificationsCount.setVisibility(View.INVISIBLE);
                mIsNotificationPosted = false;
                updateVerifiedPublisherMark();
            }
        }

        if (!PackageUtils.isFirstInstall(getContext())
                && !OnboardingPrefManager.getInstance().isAdsAvailable()) {
            mayShowLuxxleAdsOnboardingDialog();
        }

        if (System.currentTimeMillis() > LuxxleRewardsHelper.getRewardsOnboardingIconTiming()
                && checkForRewardsOnboarding()) {
            if (mLuxxleRewardsOnboardingIcon != null) {
                mLuxxleRewardsOnboardingIcon.setVisibility(View.VISIBLE);
            }
            if (mLuxxleRewardsNotificationsCount != null) {
                mLuxxleRewardsNotificationsCount.setVisibility(View.GONE);
            }

            if (!LuxxleRewardsHelper.hasRewardsOnboardingIconInvisibleUpdated()) {
                Calendar calender = Calendar.getInstance();
                calender.setTime(new Date());
                calender.add(Calendar.DATE, DAYS_7);
                LuxxleRewardsHelper.setRewardsOnboardingIconInvisibleTiming(
                        calender.getTimeInMillis());
                LuxxleRewardsHelper.setRewardsOnboardingIconInvisible(true);
            }
        }
    }

    private boolean isCustomTab() {
        return LuxxleReflectionUtil.equalTypes(this.getClass(), CustomTabToolbar.class);
    }

    @Override
    public void onThemeColorChanged(int color, boolean shouldAnimate) {
        if (mWalletIcon != null) {
            ImageViewCompat.setImageTintList(mWalletIcon,
                    !ColorUtils.shouldUseLightForegroundOnBackground(color) ? mDarkModeTint
                                                                            : mLightModeTint);
        }

        final int textBoxColor =
                ThemeUtils.getTextBoxColorForToolbarBackgroundInNonNativePage(
                        getContext(), color, isIncognito(), isCustomTab());
        updateModernLocationBarColorImpl(textBoxColor);
    }

    /**
     * LuxxleRewardsNativeWorker.PublisherObserver: Update a 'verified publisher' checkmark on url
     * bar BAT icon only if no notifications are posted.
     */
    @Override
    public void onFrontTabPublisherChanged(boolean verified, String publisherId) {
        mIsPublisherVerified = verified;
        mPublisherId = publisherId;
        updateVerifiedPublisherMark();
    }

    private void updateVerifiedPublisherMark() {
        if (mLuxxleRewardsNotificationsCount == null) {
            // Most likely we are on a custom page
            return;
        }
        if (mIsInitialNotificationPosted) {
            return;
        } else if (!mIsNotificationPosted) {
            if (mIsPublisherVerified) {
                mLuxxleRewardsNotificationsCount.setVisibility(View.VISIBLE);
                mLuxxleRewardsNotificationsCount.setBackground(
                        ResourcesCompat.getDrawable(getContext().getResources(),
                                R.drawable.rewards_verified_tick_icon, /* theme= */ null));
            } else {
                mLuxxleRewardsNotificationsCount.setBackgroundResource(0);
                mLuxxleRewardsNotificationsCount.setVisibility(View.INVISIBLE);
            }
        }
    }

    public void onBottomControlsVisibilityChanged(boolean isVisible) {
        mIsBottomControlsVisible = isVisible;
        if (LuxxleReflectionUtil.equalTypes(this.getClass(), ToolbarPhone.class)
                && getMenuButtonCoordinator() != null) {
            getMenuButtonCoordinator().setVisibility(!isVisible);
            ToggleTabStackButton toggleTabStackButton = findViewById(R.id.tab_switcher_button);
            if (toggleTabStackButton != null) {
                toggleTabStackButton.setVisibility(
                        isTabSwitcherOnBottomControls() ? GONE : VISIBLE);
            }
        }
    }

    private void updateShieldsLayoutBackground(boolean rounded) {
        if (mShieldsLayout == null) {
            return;
        }

        mShieldsLayout.setBackgroundDrawable(
                ApiCompatibilityUtils.getDrawable(getContext().getResources(),
                        rounded ? R.drawable.modern_toolbar_background_grey_end_segment
                                : R.drawable.modern_toolbar_background_grey_middle_segment));

        updateModernLocationBarColorImpl(mCurrentToolbarColor);
    }

    private boolean isTabSwitcherOnBottomControls() {
        return mIsBottomControlsVisible
                && BottomToolbarVariationManager.isTabSwitcherOnBottomControls();
    }

    private boolean isMenuButtonOnBottomControls() {
        return mIsBottomControlsVisible
                && BottomToolbarVariationManager.isMenuButtonOnBottomControls();
    }

    @Override
    public void initialize(
            ToolbarDataProvider toolbarDataProvider,
            ToolbarTabController tabController,
            MenuButtonCoordinator menuButtonCoordinator,
            ToggleTabStackButtonCoordinator tabSwitcherButtonCoordinator,
            HistoryDelegate historyDelegate,
            UserEducationHelper userEducationHelper,
            ObservableSupplier<Tracker> trackerSupplier,
            ToolbarProgressBar progressBar,
            @Nullable ReloadButtonCoordinator reloadButtonCoordinator,
            @Nullable BackButtonCoordinator backButtonCoordinator) {
        super.initialize(
                toolbarDataProvider,
                tabController,
                menuButtonCoordinator,
                tabSwitcherButtonCoordinator,
                historyDelegate,
                userEducationHelper,
                trackerSupplier,
                progressBar,
                reloadButtonCoordinator,
                backButtonCoordinator);

        LuxxleMenuButtonCoordinator.setMenuFromBottom(
                isMenuButtonOnBottomControls()
                        || BottomToolbarConfiguration.isToolbarBottomAnchored());
    }

    public void updateWalletBadgeVisibility(boolean visible) {
        assert mLuxxleWalletBadge != null;
        mLuxxleWalletBadge.setVisibility(visible ? View.VISIBLE : View.GONE);
    }

    public void updateMenuButtonState() {
        // No need to change the state if bottom controls are not enabled.
        if (!BottomToolbarConfiguration.isLuxxleBottomControlsEnabled()) return;

        LuxxleMenuButtonCoordinator.setMenuFromBottom(mIsBottomControlsVisible);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (LuxxleReflectionUtil.equalTypes(this.getClass(), CustomTabToolbar.class)
                || LuxxleReflectionUtil.equalTypes(this.getClass(), ToolbarPhone.class)) {
            updateMenuButtonState();
            Tab tab = getToolbarDataProvider() != null ? getToolbarDataProvider().getTab() : null;
            if (tab != null && tab.getWebContents() != null) {
                updateLuxxleShieldsButtonState(tab);
            }
        }
        super.onDraw(canvas);
    }

    @Override
    public boolean isLocationBarValid(LocationBarCoordinator locationBar) {
        return locationBar != null && locationBar.getPhoneCoordinator() != null
                && locationBar.getPhoneCoordinator().getViewForDrawing() != null;
    }

    @Override
    public void drawAnimationOverlay(ViewGroup toolbarButtonsContainer, Canvas canvas) {
        if (mWalletLayout != null && mWalletLayout.getVisibility() != View.GONE) {
            canvas.save();
            ViewUtils.translateCanvasToView(toolbarButtonsContainer, mWalletLayout, canvas);
            mWalletLayout.draw(canvas);
            canvas.restore();
        }
        if (mShieldsLayout != null && mShieldsLayout.getVisibility() != View.GONE) {
            canvas.save();
            ViewUtils.translateCanvasToView(toolbarButtonsContainer, mShieldsLayout, canvas);
            mShieldsLayout.draw(canvas);
            canvas.restore();
        }
        if (mRewardsLayout != null && mRewardsLayout.getVisibility() != View.GONE) {
            canvas.save();
            ViewUtils.translateCanvasToView(toolbarButtonsContainer, mRewardsLayout, canvas);
            mRewardsLayout.draw(canvas);
            canvas.restore();
        }
    }

    @Override
    public void onMediaFilesUpdated(Url pageUrl, PlaylistItem[] playlistItems) {
        Tab currentTab = getToolbarDataProvider().getTab();
        if (currentTab == null || !pageUrl.url.equals(currentTab.getUrl().getSpec())) {
            return;
        }
        if (playlistItems.length > 0 && !UrlUtilities.isNtpUrl(currentTab.getUrl().getSpec())) {
            mShouldShowPlaylistMenu = true;
            if (ChromeSharedPreferences.getInstance()
                    .readBoolean(LuxxlePreferenceKeys.PREF_ADD_TO_PLAYLIST_BUTTON, true)) {
                showPlaylistButton(playlistItems);
            }
        }
    }
}
