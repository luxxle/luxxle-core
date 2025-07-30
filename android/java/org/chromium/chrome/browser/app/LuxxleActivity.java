/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.app;

import android.app.Activity;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentSender;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageView;

import androidx.annotation.MainThread;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.widget.AppCompatEditText;
import androidx.coordinatorlayout.widget.CoordinatorLayout;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import androidx.lifecycle.LiveData;
import androidx.lifecycle.MutableLiveData;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.luxxle.playlist.util.ConstantUtils;
import com.luxxle.playlist.util.PlaylistPreferenceUtils;
import com.luxxle.playlist.util.PlaylistUtils;
import com.google.android.material.dialog.MaterialAlertDialogBuilder;
import com.google.android.play.core.appupdate.AppUpdateInfo;
import com.google.android.play.core.appupdate.AppUpdateManager;
import com.google.android.play.core.appupdate.AppUpdateManagerFactory;
import com.google.android.play.core.install.InstallStateUpdatedListener;
import com.google.android.play.core.install.model.AppUpdateType;
import com.google.android.play.core.install.model.InstallStatus;
import com.google.android.play.core.install.model.UpdateAvailability;
import com.google.android.play.core.tasks.Task;
import com.wireguard.android.backend.GoBackend;

import org.jni_zero.JNINamespace;
import org.jni_zero.NativeMethods;

import org.chromium.base.ApplicationStatus;
import org.chromium.base.LuxxleFeatureList;
import org.chromium.base.LuxxlePreferenceKeys;
import org.chromium.base.LuxxleReflectionUtil;
import org.chromium.base.CollectionUtil;
import org.chromium.base.CommandLine;
import org.chromium.base.ContextUtils;
import org.chromium.base.IntentUtils;
import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.supplier.ObservableSupplier;
import org.chromium.base.supplier.UnownedUserDataSupplier;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;
import org.chromium.luxxle.browser.quick_search_engines.settings.QuickSearchEnginesCallback;
import org.chromium.luxxle.browser.quick_search_engines.settings.QuickSearchEnginesFragment;
import org.chromium.luxxle.browser.quick_search_engines.settings.QuickSearchEnginesModel;
import org.chromium.luxxle.browser.quick_search_engines.utils.QuickSearchEnginesUtil;
import org.chromium.luxxle.browser.quick_search_engines.views.QuickSearchEnginesViewAdapter;
import org.chromium.luxxle_wallet.mojom.AssetRatioService;
import org.chromium.luxxle_wallet.mojom.BlockchainRegistry;
import org.chromium.luxxle_wallet.mojom.LuxxleWalletService;
import org.chromium.luxxle_wallet.mojom.CoinType;
import org.chromium.luxxle_wallet.mojom.EthTxManagerProxy;
import org.chromium.luxxle_wallet.mojom.JsonRpcService;
import org.chromium.luxxle_wallet.mojom.KeyringService;
import org.chromium.luxxle_wallet.mojom.NetworkInfo;
import org.chromium.luxxle_wallet.mojom.SignDataUnion;
import org.chromium.luxxle_wallet.mojom.SolanaTxManagerProxy;
import org.chromium.luxxle_wallet.mojom.SwapService;
import org.chromium.luxxle_wallet.mojom.TxService;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleAdFreeCalloutDialogFragment;
import org.chromium.chrome.browser.LuxxleHelper;
import org.chromium.chrome.browser.LuxxleIntentHandler;
import org.chromium.chrome.browser.LuxxleRelaunchUtils;
import org.chromium.chrome.browser.LuxxleRewardsHelper;
import org.chromium.chrome.browser.LuxxleSyncWorker;
import org.chromium.chrome.browser.ChromeTabbedActivity;
import org.chromium.chrome.browser.DormantUsersEngagementDialogFragment;
import org.chromium.chrome.browser.IntentHandler;
import org.chromium.chrome.browser.InternetConnection;
import org.chromium.chrome.browser.LaunchIntentDispatcher;
import org.chromium.chrome.browser.OpenYtInLuxxleDialogFragment;
import org.chromium.chrome.browser.app.domain.WalletModel;
import org.chromium.chrome.browser.billing.InAppPurchaseWrapper;
import org.chromium.chrome.browser.billing.PurchaseModel;
import org.chromium.chrome.browser.bookmarks.TabBookmarker;
import org.chromium.chrome.browser.luxxle_leo.LuxxleLeoPrefUtils;
import org.chromium.chrome.browser.luxxle_leo.LuxxleLeoUtils;
import org.chromium.chrome.browser.luxxle_news.LuxxleNewsUtils;
import org.chromium.chrome.browser.luxxle_news.models.FeedItemsCard;
import org.chromium.chrome.browser.luxxle_stats.LuxxleStatsBottomSheetDialogFragment;
import org.chromium.chrome.browser.luxxle_stats.LuxxleStatsUtil;
import org.chromium.chrome.browser.browsing_data.BrowsingDataBridge;
import org.chromium.chrome.browser.browsing_data.BrowsingDataType;
import org.chromium.chrome.browser.browsing_data.TimePeriod;
import org.chromium.chrome.browser.compositor.layouts.LayoutManagerChrome;
import org.chromium.chrome.browser.crypto_wallet.AssetRatioServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.BlockchainRegistryFactory;
import org.chromium.chrome.browser.crypto_wallet.LuxxleWalletServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.SwapServiceFactory;
import org.chromium.chrome.browser.crypto_wallet.activities.AddAccountActivity;
import org.chromium.chrome.browser.crypto_wallet.activities.LuxxleWalletActivity;
import org.chromium.chrome.browser.crypto_wallet.activities.LuxxleWalletDAppsActivity;
import org.chromium.chrome.browser.crypto_wallet.model.CryptoAccountTypeInfo;
import org.chromium.chrome.browser.crypto_wallet.util.Utils;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.customtabs.FullScreenCustomTabActivity;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.flags.ChromeSwitches;
import org.chromium.chrome.browser.fullscreen.BrowserControlsManager;
import org.chromium.chrome.browser.informers.LuxxleSyncAccountDeletedInformer;
import org.chromium.chrome.browser.lifetime.ApplicationLifetime;
import org.chromium.chrome.browser.misc_metrics.MiscAndroidMetricsConnectionErrorHandler;
import org.chromium.chrome.browser.misc_metrics.MiscAndroidMetricsFactory;
import org.chromium.chrome.browser.multiwindow.LuxxleMultiWindowUtils;
import org.chromium.chrome.browser.multiwindow.MultiInstanceManager;
import org.chromium.chrome.browser.multiwindow.MultiWindowUtils;
import org.chromium.chrome.browser.notifications.permissions.NotificationPermissionController;
import org.chromium.chrome.browser.notifications.retention.RetentionNotificationUtil;
import org.chromium.chrome.browser.ntp.NewTabPageManager;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.onboarding.v2.HighlightDialogFragment;
import org.chromium.chrome.browser.playlist.PlaylistHostActivity;
import org.chromium.chrome.browser.playlist.settings.LuxxlePlaylistPreferences;
import org.chromium.chrome.browser.preferences.LuxxlePref;
import org.chromium.chrome.browser.preferences.LuxxlePrefServiceBridge;
import org.chromium.chrome.browser.preferences.ChromePreferenceKeys;
import org.chromium.chrome.browser.preferences.ChromeSharedPreferences;
import org.chromium.chrome.browser.preferences.Pref;
import org.chromium.chrome.browser.preferences.PrefServiceUtil;
import org.chromium.chrome.browser.preferences.website.LuxxleShieldsContentSettings;
import org.chromium.chrome.browser.prefetch.settings.PreloadPagesSettingsBridge;
import org.chromium.chrome.browser.prefetch.settings.PreloadPagesState;
import org.chromium.chrome.browser.privacy.settings.LuxxlePrivacySettings;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.profiles.ProfileManager;
import org.chromium.chrome.browser.rate.LuxxleRateDialogFragment;
import org.chromium.chrome.browser.rate.RateUtils;
import org.chromium.chrome.browser.rewards.adaptive_captcha.AdaptiveCaptchaHelper;
import org.chromium.chrome.browser.safe_browsing.SafeBrowsingBridge;
import org.chromium.chrome.browser.safe_browsing.SafeBrowsingState;
import org.chromium.chrome.browser.search_engines.TemplateUrlServiceFactory;
import org.chromium.chrome.browser.set_default_browser.LuxxleSetDefaultBrowserUtils;
import org.chromium.chrome.browser.settings.LuxxleNewsPreferencesV2;
import org.chromium.chrome.browser.settings.LuxxleSearchEngineUtils;
import org.chromium.chrome.browser.settings.LuxxleWalletPreferences;
import org.chromium.chrome.browser.settings.SettingsNavigationFactory;
import org.chromium.chrome.browser.settings.developer.LuxxleQAPreferences;
import org.chromium.chrome.browser.share.ShareDelegate;
import org.chromium.chrome.browser.share.ShareDelegate.ShareOrigin;
import org.chromium.chrome.browser.shields.ContentFilteringFragment;
import org.chromium.chrome.browser.shields.CreateCustomFiltersFragment;
import org.chromium.chrome.browser.site_settings.LuxxleWalletEthereumConnectedSites;
import org.chromium.chrome.browser.speedreader.LuxxleSpeedReaderUtils;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tab.TabLaunchType;
import org.chromium.chrome.browser.tab.TabSelectionType;
import org.chromium.chrome.browser.tabmodel.TabClosureParams;
import org.chromium.chrome.browser.tabmodel.TabList;
import org.chromium.chrome.browser.tabmodel.TabModel;
import org.chromium.chrome.browser.tabmodel.TabModelUtils;
import org.chromium.chrome.browser.toolbar.LuxxleToolbarManager;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.toolbar.top.LuxxleToolbarLayoutImpl;
import org.chromium.chrome.browser.ui.RootUiCoordinator;
import org.chromium.chrome.browser.ui.messages.snackbar.Snackbar;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManager.SnackbarController;
import org.chromium.chrome.browser.ui.messages.snackbar.SnackbarManagerProvider;
import org.chromium.chrome.browser.util.LuxxleConstants;
import org.chromium.chrome.browser.util.LuxxleDbUtil;
import org.chromium.chrome.browser.util.KeyboardVisibilityHelper;
import org.chromium.chrome.browser.util.LiveDataUtil;
import org.chromium.chrome.browser.util.PackageUtils;
import org.chromium.chrome.browser.util.UsageMonitor;
import org.chromium.chrome.browser.vpn.LuxxleVpnNativeWorker;
import org.chromium.chrome.browser.vpn.LuxxleVpnObserver;
import org.chromium.chrome.browser.vpn.activities.LuxxleVpnProfileActivity;
import org.chromium.chrome.browser.vpn.fragments.LinkVpnSubscriptionDialogFragment;
import org.chromium.chrome.browser.vpn.timer.TimerDialogFragment;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnApiResponseUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnProfileUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnUtils;
import org.chromium.chrome.browser.vpn.wireguard.WireguardConfigUtils;
import org.chromium.chrome.browser.widget.quickactionsearchandbookmark.promo.SearchWidgetPromoPanel;
import org.chromium.components.browser_ui.settings.SettingsNavigation;
import org.chromium.components.embedder_support.util.UrlConstants;
import org.chromium.components.embedder_support.util.UrlUtilities;
import org.chromium.components.prefs.PrefChangeRegistrar;
import org.chromium.components.prefs.PrefChangeRegistrar.PrefObserver;
import org.chromium.components.safe_browsing.LuxxleSafeBrowsingApiHandler;
import org.chromium.components.search_engines.TemplateUrl;
import org.chromium.components.search_engines.TemplateUrlService;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.content_public.browser.LoadUrlParams;
import org.chromium.content_public.browser.WebContents;
import org.chromium.misc_metrics.mojom.MiscAndroidMetrics;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.ui.KeyboardUtils;
import org.chromium.ui.widget.Toast;

import java.util.Arrays;
import java.util.Calendar;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Locale;
import java.util.Set;
import java.util.concurrent.CopyOnWriteArrayList;

/** Luxxle's extension for ChromeActivity */
@JNINamespace("chrome::android")
@SuppressWarnings("UseSharedPreferencesManagerFromChromeCheck")
public abstract class LuxxleActivity extends ChromeActivity
        implements BrowsingDataBridge.OnClearBrowsingDataListener,
                LuxxleVpnObserver,
                ConnectionErrorHandler,
                PrefObserver,
                LuxxleSafeBrowsingApiHandler.LuxxleSafeBrowsingApiHandlerDelegate,
                MiscAndroidMetricsConnectionErrorHandler
                        .MiscAndroidMetricsConnectionErrorHandlerDelegate,
                QuickSearchEnginesCallback,
                KeyboardVisibilityHelper.KeyboardVisibilityListener,
                OnSharedPreferenceChangeListener {
    public static final String LUXXLE_WALLET_HOST = "wallet";
    public static final String LUXXLE_WALLET_ORIGIN = "luxxle://wallet/";
    public static final String LUXXLE_WALLET_URL = "luxxle://wallet/crypto/portfolio/assets";
    public static final String LUXXLE_BUY_URL = "luxxle://wallet/crypto/fund-wallet";
    public static final String LUXXLE_SEND_URL = "luxxle://wallet/send";
    public static final String LUXXLE_SWAP_URL = "luxxle://wallet/swap";
    public static final String LUXXLE_DEPOSIT_URL = "luxxle://wallet/crypto/deposit-funds";
    public static final String LUXXLE_REWARDS_SETTINGS_URL = "luxxle://rewards/";
    public static final String LUXXLE_REWARDS_SETTINGS_WALLET_VERIFICATION_URL =
            "luxxle://rewards/#verify";
    public static final String LUXXLE_REWARDS_WALLET_RECONNECT_URL = "luxxle://rewards/reconnect";
    public static final String LUXXLE_REWARDS_SETTINGS_MONTHLY_URL = "luxxle://rewards/#monthly";
    public static final String REWARDS_AC_SETTINGS_URL = "luxxle://rewards/contribute";
    public static final String LUXXLE_REWARDS_RESET_PAGE = "luxxle://rewards/#reset";
    public static final String LUXXLE_AI_CHAT_URL = "chrome-untrusted://chat/tab";
    public static final String REWARDS_LEARN_MORE_URL =
            "https://luxxle.com/faq-rewards/#unclaimed-funds";
    public static final String LUXXLE_TERMS_PAGE =
            "https://basicattentiontoken.org/user-terms-of-service/";
    public static final String LUXXLE_PRIVACY_POLICY = "https://luxxle.com/privacy/browser/#rewards";
    public static final String OPEN_URL = "open_url";
    public static final String LUXXLE_WEBCOMPAT_INFO_WIKI_URL =
            "https://github.com/luxxle/luxxle-browser/wiki/Web-compatibility-reports";

    private static final int DAYS_4 = 4;
    private static final int DAYS_7 = 7;

    private static final int MONTH_1 = 1;

    public static final int MAX_FAILED_CAPTCHA_ATTEMPTS = 10;

    public static final int APP_OPEN_COUNT_FOR_WIDGET_PROMO = 25;

    public static final String GOOGLE_SEARCH_ENGINE_KEYWORD = ":g";
    public static final String YOUTUBE_SEARCH_ENGINE_KEYWORD = ":yt";
    public static final String LUXXLE_SEARCH_ENGINE_KEYWORD = ":br";
    public static final String BING_SEARCH_ENGINE_KEYWORD = ":b";
    public static final String STARTPAGE_SEARCH_ENGINE_KEYWORD = ":sp";

    private static final boolean ENABLE_IN_APP_UPDATE =
            Build.VERSION.SDK_INT < Build.VERSION_CODES.UPSIDE_DOWN_CAKE;

    /** Settings for sending local notification reminders. */
    public static final String CHANNEL_ID = "com.luxxle.browser";

    // Explicitly declare this variable to avoid build errors.
    // It will be removed in asm and parent variable will be used instead.
    private UnownedUserDataSupplier<BrowserControlsManager> mBrowserControlsManagerSupplier;

    private static final List<String> sYandexRegions =
            Arrays.asList("AM", "AZ", "BY", "KG", "KZ", "MD", "RU", "TJ", "TM", "UZ");

    private boolean mIsVerification;
    public boolean mIsDeepLink;
    private LuxxleWalletService mLuxxleWalletService;
    private KeyringService mKeyringService;
    private JsonRpcService mJsonRpcService;
    private MiscAndroidMetrics mMiscAndroidMetrics;
    private SwapService mSwapService;
    @Nullable private WalletModel mWalletModel;
    private BlockchainRegistry mBlockchainRegistry;
    private TxService mTxService;
    private EthTxManagerProxy mEthTxManagerProxy;
    private SolanaTxManagerProxy mSolanaTxManagerProxy;
    private AssetRatioService mAssetRatioService;
    public boolean mLoadedFeed;
    public boolean mComesFromNewTab;
    public CopyOnWriteArrayList<FeedItemsCard> mNewsItemsFeedCards;
    private boolean mIsProcessingPendingDappsTxRequest;
    private int mLastTabId;
    private boolean mNativeInitialized;
    private boolean mSafeBrowsingFlagEnabled;
    private NewTabPageManager mNewTabPageManager;
    private UsageMonitor mUsageMonitor;
    private NotificationPermissionController mNotificationPermissionController;
    private MiscAndroidMetricsConnectionErrorHandler mMiscAndroidMetricsConnectionErrorHandler;
    private AppUpdateManager mAppUpdateManager;
    private boolean mWalletBadgeVisible;
    private boolean mSpoofCustomTab;

    private View mQuickSearchEnginesView;

    private SearchWidgetPromoPanel mSearchWidgetPromoPanel;

    /** Serves as a general exception for failed attempts to get LuxxleActivity. */
    public static class LuxxleActivityNotFoundException extends Exception {
        public LuxxleActivityNotFoundException(String message) {
            super(message);
        }
    }

    public LuxxleActivity() {}

    @Override
    public void onResumeWithNative() {
        super.onResumeWithNative();
        LuxxleActivityJni.get().restartStatsUpdater();
        if (LuxxleVpnUtils.isVpnFeatureSupported(LuxxleActivity.this)) {
            LuxxleVpnNativeWorker.getInstance().addObserver(this);
            LuxxleVpnUtils.reportBackgroundUsageP3A();
        }

        // The check on mNativeInitialized is mostly to ensure that mojo
        // services for wallet are initialized.
        // TODO(sergz): verify do we need it in that phase or not.
        if (mNativeInitialized) {
            LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
            if (layout != null && layout.isWalletIconVisible()) {
                updateWalletBadgeVisibility();
            }

            // If a full screen custom tab was closed and bottom controls are enabled,
            // show the bottom toolbar controls again
            if (FullScreenCustomTabActivity.sIsFullScreenCustomTabActivityClosed
                    && BottomToolbarConfiguration.isLuxxleBottomControlsEnabled()) {
                layout.onBottomControlsVisibilityChanged(true);
            }
            // Reset the flag tracking whether a full screen custom tab was closed
            FullScreenCustomTabActivity.sIsFullScreenCustomTabActivityClosed = false;
        }

        LuxxleSafeBrowsingApiHandler.getInstance()
                .setDelegate(LuxxleActivityJni.get().getSafeBrowsingApiKey(), this);

        // We can store a state of that flag as a browser has to be restarted
        // when the flag state is changed in any case
        mSafeBrowsingFlagEnabled =
                ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_ANDROID_SAFE_BROWSING);

        executeInitSafeBrowsing(0);

        if (ENABLE_IN_APP_UPDATE) {
            if (mAppUpdateManager == null) {
                mAppUpdateManager = AppUpdateManagerFactory.create(LuxxleActivity.this);
            }
            mAppUpdateManager
                    .getAppUpdateInfo()
                    .addOnSuccessListener(
                            appUpdateInfo -> {
                                if (appUpdateInfo.installStatus() == InstallStatus.DOWNLOADED) {
                                    completeUpdateSnackbar();
                                }
                            });
        }
    }

    @Override
    public void onPauseWithNative() {
        if (mUsageMonitor != null) {
            mUsageMonitor.stop();
        }
        if (LuxxleVpnUtils.isVpnFeatureSupported(LuxxleActivity.this)) {
            LuxxleVpnNativeWorker.getInstance().removeObserver(this);
        }
        super.onPauseWithNative();
    }

    @Override
    public boolean onMenuOrKeyboardAction(int id, boolean fromMenu) {
        final Tab currentTab = getActivityTab();
        // Handle items replaced by Luxxle.
        if (id == R.id.info_menu_id && currentTab != null) {
            ShareDelegate shareDelegate = (ShareDelegate) getShareDelegateSupplier().get();
            shareDelegate.share(currentTab, false, ShareOrigin.OVERFLOW_MENU);
            return true;
        } else if (id == R.id.reload_menu_id) {
            setComesFromNewTab(true);
        }

        if (super.onMenuOrKeyboardAction(id, fromMenu)) {
            return true;
        }

        // Handle items added by Luxxle.
        if (currentTab == null) {
            return false;
        } else if (id == R.id.exit_id) {
            exitLuxxle();
        } else if (id == R.id.set_default_browser) {
            LuxxleSetDefaultBrowserUtils.openDefaultAppsSettings(LuxxleActivity.this);
        } else if (id == R.id.luxxle_rewards_id) {
            showRewardsPage();
        } else if (id == R.id.luxxle_wallet_id) {
            openLuxxleWallet(false, false, false);
        } else if (id == R.id.luxxle_playlist_id) {
            openPlaylist(true);
        } else if (id == R.id.add_to_playlist_id) {
            LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
            layout.addMediaToPlaylist();
        } else if (id == R.id.luxxle_news_id) {
            openLuxxleNewsSettings();
        } else if (id == R.id.request_luxxle_vpn_id || id == R.id.request_luxxle_vpn_check_id) {
            if (!InternetConnection.isNetworkAvailable(LuxxleActivity.this)) {
                Toast.makeText(LuxxleActivity.this, R.string.no_internet, Toast.LENGTH_SHORT).show();
            } else {
                if (LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(LuxxleActivity.this)) {
                    TimerDialogFragment timerDialogFragment = new TimerDialogFragment();
                    timerDialogFragment.show(getSupportFragmentManager(), TimerDialogFragment.TAG);
                } else {
                    if (LuxxleVpnNativeWorker.getInstance().isPurchasedUser()) {
                        LuxxleVpnPrefUtils.setSubscriptionPurchase(true);
                        if (WireguardConfigUtils.isConfigExist(LuxxleActivity.this)) {
                            LuxxleVpnProfileUtils.getInstance().startVpn(LuxxleActivity.this);
                        } else {
                            LuxxleVpnUtils.openLuxxleVpnProfileActivity(LuxxleActivity.this);
                        }
                    } else {
                        LuxxleVpnUtils.showProgressDialog(
                                LuxxleActivity.this,
                                getResources().getString(R.string.vpn_connect_text));
                        if (LuxxleVpnPrefUtils.isSubscriptionPurchase()) {
                            verifySubscription();
                        } else {
                            LuxxleVpnUtils.dismissProgressDialog();
                            LuxxleVpnUtils.openLuxxleVpnPlansActivity(LuxxleActivity.this);
                        }
                    }
                }
            }
        } else if (id == R.id.request_vpn_location_id || id == R.id.request_vpn_location_icon_id) {
            LuxxleVpnUtils.openVpnServerSelectionActivity(LuxxleActivity.this);
        } else if (id == R.id.luxxle_speedreader_id) {
            enableSpeedreaderMode();
        } else if (id == R.id.luxxle_leo_id) {
            openLuxxleLeo();
        } else {
            return false;
        }
        return true;
    }

    // Handles only wallet related mojo failures. Don't add handlers for mojo connections that
    // are not related to wallet functionality.
    @Override
    public void onConnectionError(MojoException e) {
        cleanUpWalletNativeServices();
        initWalletNativeServices();
    }

    @Override
    protected void onDestroyInternal() {
        if (mNotificationPermissionController != null) {
            NotificationPermissionController.detach(mNotificationPermissionController);
            mNotificationPermissionController = null;
        }

        LuxxleSafeBrowsingApiHandler.getInstance().shutdownSafeBrowsing();
        if (ENABLE_IN_APP_UPDATE && mAppUpdateManager != null) {
            mAppUpdateManager.unregisterListener(mInstallStateUpdatedListener);
        }
        super.onDestroyInternal();
        cleanUpWalletNativeServices();
        cleanUpMiscAndroidMetrics();
    }

    /**
     * Gets Wallet model for Luxxle activity. It may be {@code null} if native initialization has not
     * completed yet.
     */
    @Nullable
    public WalletModel getWalletModel() {
        return mWalletModel;
    }

    private void setWalletBadgeVisibility(boolean visible) {
        mWalletBadgeVisible = visible;
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.updateWalletBadgeVisibility(visible);
    }

    private void maybeShowPendingTransactions() {
        if (mWalletModel != null) {
            // Trigger observer to refresh the transactions and process any pending request.
            mWalletModel.getCryptoModel().refreshTransactions();
        }
    }

    private void maybeShowSignSolTransactionsRequestLayout(
            @NonNull final Runnable openWalletPanelRunnable) {
        assert mLuxxleWalletService != null;
        mLuxxleWalletService.getPendingSignSolTransactionsRequests(
                requests -> {
                    if (requests != null && requests.length != 0) {
                        openLuxxleWalletDAppsActivity(
                                LuxxleWalletDAppsActivity.ActivityType.SIGN_SOL_TRANSACTIONS);
                        return;
                    }
                    maybeShowSignMessageErrorsLayout(openWalletPanelRunnable);
                });
    }

    private void maybeShowSignMessageErrorsLayout(@NonNull final Runnable openWalletPanelRunnable) {
        assert mLuxxleWalletService != null;
        mLuxxleWalletService.getPendingSignMessageErrors(
                errors -> {
                    if (errors != null && errors.length != 0) {
                        openLuxxleWalletDAppsActivity(
                                LuxxleWalletDAppsActivity.ActivityType.SIGN_MESSAGE_ERROR);
                    }
                });
        maybeShowSignMessageRequestLayout(openWalletPanelRunnable);
    }

    private void maybeShowSignMessageRequestLayout(
            @NonNull final Runnable openWalletPanelRunnable) {
        assert mLuxxleWalletService != null;
        mLuxxleWalletService.getPendingSignMessageRequests(
                requests -> {
                    if (requests != null && requests.length != 0) {
                        LuxxleWalletDAppsActivity.ActivityType activityType =
                                (requests[0].signData.which() == SignDataUnion.Tag.EthSiweData)
                                        ? LuxxleWalletDAppsActivity.ActivityType.SIWE_MESSAGE
                                        : LuxxleWalletDAppsActivity.ActivityType.SIGN_MESSAGE;
                        openLuxxleWalletDAppsActivity(activityType);
                        return;
                    }
                    maybeShowChainRequestLayout(openWalletPanelRunnable);
                });
    }

    private void maybeShowChainRequestLayout(@NonNull final Runnable openWalletPanelRunnable) {
        assert mJsonRpcService != null;
        mJsonRpcService.getPendingAddChainRequests(
                networks -> {
                    if (networks != null && networks.length != 0) {
                        openLuxxleWalletDAppsActivity(
                                LuxxleWalletDAppsActivity.ActivityType.ADD_ETHEREUM_CHAIN);

                        return;
                    }
                    maybeShowSwitchChainRequestLayout(openWalletPanelRunnable);
                });
    }

    private void maybeShowSwitchChainRequestLayout(
            @NonNull final Runnable openWalletPanelRunnable) {
        assert mJsonRpcService != null;
        mJsonRpcService.getPendingSwitchChainRequests(
                requests -> {
                    if (requests != null && requests.length != 0) {
                        openLuxxleWalletDAppsActivity(
                                LuxxleWalletDAppsActivity.ActivityType.SWITCH_ETHEREUM_CHAIN);

                        return;
                    }
                    maybeShowAddSuggestTokenRequestLayout(openWalletPanelRunnable);
                });
    }

    private void maybeShowAddSuggestTokenRequestLayout(
            @NonNull final Runnable openWalletPanelRunnable) {
        assert mLuxxleWalletService != null;
        mLuxxleWalletService.getPendingAddSuggestTokenRequests(
                requests -> {
                    if (requests != null && requests.length != 0) {
                        openLuxxleWalletDAppsActivity(
                                LuxxleWalletDAppsActivity.ActivityType.ADD_TOKEN);

                        return;
                    }
                    maybeShowGetEncryptionPublicKeyRequestLayout(openWalletPanelRunnable);
                });
    }

    private void maybeShowGetEncryptionPublicKeyRequestLayout(
            @NonNull final Runnable openWalletPanelRunnable) {
        assert mLuxxleWalletService != null;
        mLuxxleWalletService.getPendingGetEncryptionPublicKeyRequests(
                requests -> {
                    if (requests != null && requests.length != 0) {
                        openLuxxleWalletDAppsActivity(
                                LuxxleWalletDAppsActivity.ActivityType
                                        .GET_ENCRYPTION_PUBLIC_KEY_REQUEST);

                        return;
                    }
                    maybeShowDecryptRequestLayout(openWalletPanelRunnable);
                });
    }

    private void maybeShowDecryptRequestLayout(@NonNull final Runnable openWalletPanelRunnable) {
        assert mLuxxleWalletService != null;
        mLuxxleWalletService.getPendingDecryptRequests(
                requests -> {
                    if (requests != null && requests.length != 0) {
                        openLuxxleWalletDAppsActivity(
                                LuxxleWalletDAppsActivity.ActivityType.DECRYPT_REQUEST);

                        return;
                    }
                    openWalletPanelRunnable.run();
                });
    }

    public void dismissWalletPanelOrDialog() {
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.dismissWalletPanelOrDialog();
    }

    public void showWalletPanel(final boolean ignoreWeb3NotificationPreference) {
        showWalletPanel(true, ignoreWeb3NotificationPreference);
    }

    public void showWalletPanel(
            final boolean showPendingTransactions, final boolean ignoreWeb3NotificationPreference) {
        final LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.showWalletIcon(true);
        if (!ignoreWeb3NotificationPreference
                && !LuxxleWalletPreferences.getPrefWeb3NotificationsEnabled()) {
            return;
        }
        assert mKeyringService != null;
        mKeyringService.isLocked(
                locked -> {
                    if (locked) {
                        if (showPendingTransactions) {
                            layout.showWalletPanel();
                        }
                        return;
                    }
                    mKeyringService.hasPendingUnlockRequest(
                            pending -> {
                                if (pending) {
                                    layout.showWalletPanel();
                                    return;
                                }
                                // Create a runnable that opens the Wallet
                                // if the pending requests reach the end of the chain
                                // without returning earlier.
                                final Runnable openWalletPanelRunnable =
                                        () -> {
                                            if (showPendingTransactions && mWalletBadgeVisible) {
                                                maybeShowPendingTransactions();
                                            } else {
                                                getLuxxleToolbarLayout().showWalletPanel();
                                            }
                                        };
                                maybeShowSignSolTransactionsRequestLayout(openWalletPanelRunnable);
                            });
                });
    }

    public void showWalletOnboarding() {
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.showWalletIcon(true);
        if (!LuxxleWalletPreferences.getPrefWeb3NotificationsEnabled()) {
            return;
        }
        layout.showWalletPanel();
    }

    public void walletInteractionDetected(WebContents webContents) {
        Tab tab = getActivityTab();
        if (tab == null
                || !webContents.getLastCommittedUrl().equals(
                        tab.getWebContents().getLastCommittedUrl())) {
            return;
        }
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.showWalletIcon(true);
        updateWalletBadgeVisibility();
    }

    public void showAccountCreation(@CoinType.EnumType int coinType) {
        if (mWalletModel != null) {
            mWalletModel.getDappsModel().addAccountCreationRequest(coinType);
        }
    }

    private void updateWalletBadgeVisibility() {
        if (mWalletModel != null) {
            mWalletModel.getDappsModel().updateWalletBadgeVisibility();
        }
    }

    private void verifySubscription() {
        MutableLiveData<PurchaseModel> _activePurchases = new MutableLiveData();
        LiveData<PurchaseModel> activePurchases = _activePurchases;
        InAppPurchaseWrapper.getInstance()
                .queryPurchases(_activePurchases, InAppPurchaseWrapper.SubscriptionProduct.VPN);
        LiveDataUtil.observeOnce(
                activePurchases,
                activePurchaseModel -> {
                    if (activePurchaseModel != null) {
                        LuxxleVpnNativeWorker.getInstance()
                                .verifyPurchaseToken(
                                        activePurchaseModel.getPurchaseToken(),
                                        activePurchaseModel.getProductId(),
                                        LuxxleVpnUtils.SUBSCRIPTION_PARAM_TEXT,
                                        getPackageName());
                    } else {
                        LuxxleVpnApiResponseUtils.queryPurchaseFailed(LuxxleActivity.this);
                        if (!mIsVerification) {
                            LuxxleVpnUtils.openLuxxleVpnPlansActivity(LuxxleActivity.this);
                        }
                    }
                });
    }

    @Override
    public boolean onOptionsItemSelected(int itemId, @Nullable Bundle menuItemData) {
        if (itemId == R.id.new_tab_menu_id) {
            LayoutManagerChrome layoutManager =
                    (LayoutManagerChrome)
                            LuxxleReflectionUtil.getField(
                                    ChromeTabbedActivity.class, "mLayoutManager", this);
            if (layoutManager != null
                    && layoutManager.getHubLayoutForTesting() != null
                    && !layoutManager.getHubLayoutForTesting().isActive()
                    && mMiscAndroidMetrics != null) {
                mMiscAndroidMetrics.recordAppMenuNewTab();
            }
        } else if (itemId == R.id.home_menu_id) {
            if (getToolbarManager() instanceof LuxxleToolbarManager) {
                ((LuxxleToolbarManager) getToolbarManager()).openHomepage();
            }
        }
        return super.onOptionsItemSelected(itemId, menuItemData);
    }

    @Override
    public void onVerifyPurchaseToken(
            String jsonResponse, String purchaseToken, String productId, boolean isSuccess) {
        if (isSuccess) {
            Long purchaseExpiry = LuxxleVpnUtils.getPurchaseExpiryDate(jsonResponse);
            int paymentState = LuxxleVpnUtils.getPaymentState(jsonResponse);
            if (purchaseExpiry > 0 && purchaseExpiry >= System.currentTimeMillis()) {
                LuxxleVpnPrefUtils.setPurchaseToken(purchaseToken);
                LuxxleVpnPrefUtils.setProductId(productId);
                LuxxleVpnPrefUtils.setPurchaseExpiry(purchaseExpiry);
                LuxxleVpnPrefUtils.setSubscriptionPurchase(true);
                LuxxleVpnPrefUtils.setPaymentState(paymentState);
                if (LuxxleVpnPrefUtils.isResetConfiguration()) {
                    LuxxleVpnUtils.dismissProgressDialog();
                    LuxxleVpnUtils.openLuxxleVpnProfileActivity(LuxxleActivity.this);
                } else {
                    if (!mIsVerification) {
                        checkForVpn();
                    } else {
                        mIsVerification = false;
                        if (LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(
                                    LuxxleActivity.this)
                                && !TextUtils.isEmpty(LuxxleVpnPrefUtils.getHostname())
                                && !TextUtils.isEmpty(LuxxleVpnPrefUtils.getClientId())
                                && !TextUtils.isEmpty(LuxxleVpnPrefUtils.getSubscriberCredential())
                                && !TextUtils.isEmpty(LuxxleVpnPrefUtils.getApiAuthToken())) {
                            LuxxleVpnNativeWorker.getInstance().verifyCredentials(
                                    LuxxleVpnPrefUtils.getHostname(),
                                    LuxxleVpnPrefUtils.getClientId(),
                                    LuxxleVpnPrefUtils.getSubscriberCredential(),
                                    LuxxleVpnPrefUtils.getApiAuthToken());
                        }
                    }
                    LuxxleVpnUtils.dismissProgressDialog();
                }
            } else {
                LuxxleVpnApiResponseUtils.queryPurchaseFailed(LuxxleActivity.this);
                if (!mIsVerification) {
                    LuxxleVpnUtils.openLuxxleVpnPlansActivity(LuxxleActivity.this);
                }
                mIsVerification = false;
            }
        } else {
            LuxxleVpnApiResponseUtils.queryPurchaseFailed(LuxxleActivity.this);
            if (!mIsVerification) {
                LuxxleVpnUtils.openLuxxleVpnPlansActivity(LuxxleActivity.this);
            }
            mIsVerification = false;
        }
    };

    private void checkForVpn() {
        LuxxleVpnNativeWorker.getInstance().reportForegroundP3A();
        new Thread() {
            @Override
            public void run() {
                Intent intent = GoBackend.VpnService.prepare(LuxxleActivity.this);
                if (intent != null
                        || !WireguardConfigUtils.isConfigExist(getApplicationContext())) {
                    LuxxleVpnUtils.dismissProgressDialog();
                    LuxxleVpnUtils.openLuxxleVpnProfileActivity(LuxxleActivity.this);
                    return;
                }
                LuxxleVpnProfileUtils.getInstance().startVpn(LuxxleActivity.this);
            }
        }.start();
    }

    @Override
    public void onVerifyCredentials(String jsonVerifyCredentials, boolean isSuccess) {
        if (!isSuccess) {
            if (LuxxleVpnProfileUtils.getInstance().isLuxxleVPNConnected(LuxxleActivity.this)) {
                LuxxleVpnProfileUtils.getInstance().stopVpn(LuxxleActivity.this);
            }
            Intent luxxleVpnProfileIntent =
                    new Intent(LuxxleActivity.this, LuxxleVpnProfileActivity.class);
            luxxleVpnProfileIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
            luxxleVpnProfileIntent.putExtra(LuxxleVpnUtils.VERIFY_CREDENTIALS_FAILED, true);
            luxxleVpnProfileIntent.setAction(Intent.ACTION_VIEW);
            startActivity(luxxleVpnProfileIntent);
        }
    }

    @Override
    public void initializeState() {
        super.initializeState();
        if (isNoRestoreState()) {
            CommandLine.getInstance().appendSwitch(ChromeSwitches.NO_RESTORE_STATE);
        }

        if (isClearBrowsingDataOnExit()) {
            List<Integer> dataTypes =
                    Arrays.asList(
                            BrowsingDataType.HISTORY,
                            BrowsingDataType.SITE_DATA,
                            BrowsingDataType.CACHE);

            int[] dataTypesArray = CollectionUtil.integerCollectionToIntArray(dataTypes);

            // has onBrowsingDataCleared() as an @Override callback from implementing
            // BrowsingDataBridge.OnClearBrowsingDataListener
            BrowsingDataBridge.getForProfile(getCurrentProfile())
                    .clearBrowsingData(this, dataTypesArray, TimePeriod.ALL_TIME);
        }

        setLoadedFeed(false);
        setComesFromNewTab(false);
        setNewsItemsFeedCards(null);
        LuxxleSearchEngineUtils.initializeLuxxleSearchEngineStates(getTabModelSelector());
        Intent intent = getIntent();
        if (intent != null
                && intent.getBooleanExtra(LuxxleWalletActivity.RESTART_WALLET_ACTIVITY, false)) {
            openLuxxleWallet(
                    false,
                    intent.getBooleanExtra(
                            LuxxleWalletActivity.RESTART_WALLET_ACTIVITY_SETUP, false),
                    intent.getBooleanExtra(
                            LuxxleWalletActivity.RESTART_WALLET_ACTIVITY_RESTORE, false));
        }
    }

    public int getLastTabId() {
        return mLastTabId;
    }

    public void setLastTabId(int lastTabId) {
        this.mLastTabId = lastTabId;
    }

    public boolean isLoadedFeed() {
        return mLoadedFeed;
    }

    public void setLoadedFeed(boolean loadedFeed) {
        this.mLoadedFeed = loadedFeed;
    }

    public CopyOnWriteArrayList<FeedItemsCard> getNewsItemsFeedCards() {
        return mNewsItemsFeedCards;
    }

    public void setNewsItemsFeedCards(CopyOnWriteArrayList<FeedItemsCard> newsItemsFeedCards) {
        this.mNewsItemsFeedCards = newsItemsFeedCards;
    }

    public void setComesFromNewTab(boolean comesFromNewTab) {
        this.mComesFromNewTab = comesFromNewTab;
    }

    public boolean isComesFromNewTab() {
        return mComesFromNewTab;
    }

    @Override
    public void onBrowsingDataCleared() {}

    @Override
    public void onResume() {
        super.onResume();
        mIsProcessingPendingDappsTxRequest = false;

        PostTask.postTask(
                TaskTraits.BEST_EFFORT_MAY_BLOCK, () -> { LuxxleStatsUtil.removeShareStatsFile(); });

        // We need to enable widget promo for later release
        /* int appOpenCountForWidgetPromo = SharedPreferencesManager.getInstance().readInt(
                LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT_FOR_WIDGET_PROMO);
        if (appOpenCountForWidgetPromo < APP_OPEN_COUNT_FOR_WIDGET_PROMO) {
            SharedPreferencesManager.getInstance().writeInt(
                    LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT_FOR_WIDGET_PROMO,
                    appOpenCountForWidgetPromo + 1);
        } */
        if (mUsageMonitor != null) {
            mUsageMonitor.start();
        }
    }

    @Override
    public void performPostInflationStartup() {
        super.performPostInflationStartup();

        createNotificationChannel();
    }

    @Override
    protected void initializeStartupMetrics() {
        super.initializeStartupMetrics();

        // Disable FRE for arm64 builds where ChromeActivity is the one that
        // triggers FRE instead of ChromeLauncherActivity on arm32 build.
        LuxxleHelper.disableFREDRP();
    }

    @Override
    public void onPreferenceChange() {
        String captchaID =
                UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                        .getString(LuxxlePref.SCHEDULED_CAPTCHA_ID);
        String paymentID =
                UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                        .getString(LuxxlePref.SCHEDULED_CAPTCHA_PAYMENT_ID);
        if (LuxxleQAPreferences.shouldVlogRewards()) {
            Log.e(
                    AdaptiveCaptchaHelper.TAG,
                    "captchaID : " + captchaID + " Payment ID : " + paymentID);
        }
        maybeSolveAdaptiveCaptcha();
    }

    @Override
    public void turnSafeBrowsingOff() {
        SafeBrowsingBridge safeBrowsingBridge = new SafeBrowsingBridge(getCurrentProfile());
        safeBrowsingBridge.setSafeBrowsingState(SafeBrowsingState.NO_SAFE_BROWSING);
    }

    // Shows SafeBrowsing errors if the switch in Developer Options is on
    @Override
    public void maybeShowSafeBrowsingError(String error) {
        if (ChromeSharedPreferences.getInstance()
                .readBoolean(LuxxlePreferenceKeys.LUXXLE_SAFE_BROWSING_ERRORS, false)) {
            Toast.makeText(LuxxleActivity.this, error, Toast.LENGTH_LONG).show();
        }
    }

    @Override
    public boolean isSafeBrowsingEnabled() {
        return mSafeBrowsingFlagEnabled;
    }

    @Override
    public Activity getActivity() {
        return this;
    }

    public void maybeSolveAdaptiveCaptcha() {
        String captchaID =
                UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                        .getString(LuxxlePref.SCHEDULED_CAPTCHA_ID);
        String paymentID =
                UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                        .getString(LuxxlePref.SCHEDULED_CAPTCHA_PAYMENT_ID);
        if (!TextUtils.isEmpty(captchaID) && !TextUtils.isEmpty(paymentID)) {
            AdaptiveCaptchaHelper.startAttestation(captchaID, paymentID);
        }
    }

    @Override
    public void finishNativeInitialization() {
        super.finishNativeInitialization();

        boolean isFirstInstall = PackageUtils.isFirstInstall(this);

        String countryCode = Locale.getDefault().getCountry();

        LuxxleVpnNativeWorker.getInstance().reloadPurchasedState();

        LuxxleHelper.maybeMigrateSettings();

        PrefChangeRegistrar mPrefChangeRegistrar = PrefServiceUtil.createFor(getCurrentProfile());
        mPrefChangeRegistrar.addObserver(LuxxlePref.SCHEDULED_CAPTCHA_ID, this);

        if (UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                        .getInteger(LuxxlePref.SCHEDULED_CAPTCHA_FAILED_ATTEMPTS)
                >= MAX_FAILED_CAPTCHA_ATTEMPTS) {
            UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                    .setBoolean(LuxxlePref.SCHEDULED_CAPTCHA_PAUSED, true);
        }

        if (LuxxleQAPreferences.shouldVlogRewards()) {
            Log.e(
                    AdaptiveCaptchaHelper.TAG,
                    "Failed attempts : "
                            + UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                                    .getInteger(LuxxlePref.SCHEDULED_CAPTCHA_FAILED_ATTEMPTS));
        }
        if (!UserPrefs.get(ProfileManager.getLastUsedRegularProfile())
                .getBoolean(LuxxlePref.SCHEDULED_CAPTCHA_PAUSED)) {
            maybeSolveAdaptiveCaptcha();
        }

        if (ChromeSharedPreferences.getInstance()
                .readBoolean(LuxxlePreferenceKeys.LUXXLE_DOUBLE_RESTART, false)) {
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(LuxxlePreferenceKeys.LUXXLE_DOUBLE_RESTART, false);
            LuxxleRelaunchUtils.restart();
            return;
        }

        // Make sure this option is disabled
        if (PreloadPagesSettingsBridge.getState(getCurrentProfile())
                != PreloadPagesState.NO_PRELOADING) {
            PreloadPagesSettingsBridge.setState(
                    getCurrentProfile(), PreloadPagesState.NO_PRELOADING);
        }

        if (LuxxleRewardsHelper.hasRewardsEnvChange()) {
            LuxxlePrefServiceBridge.getInstance().resetPromotionLastFetchStamp();
            LuxxleRewardsHelper.setRewardsEnvChange(false);
        }

        int appOpenCount =
                ChromeSharedPreferences.getInstance()
                        .readInt(LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT);
        ChromeSharedPreferences.getInstance()
                .writeInt(LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT, appOpenCount + 1);

        if (isFirstInstall && appOpenCount == 0) {
            checkForYandexSE();
            enableSearchSuggestions();
            setLuxxleAsDefaultPrivateMode();
        }

        if (!isFirstInstall
                && countryCode.equals(LuxxleConstants.JAPAN_COUNTRY_CODE)
                && !ChromeSharedPreferences.getInstance()
                        .readBoolean(
                                LuxxlePreferenceKeys.LUXXLE_DEFAULT_SEARCH_ENGINE_MIGRATED_JP,
                                false)) {
            applyChangesForYahooJp();
        }

        LuxxleSetDefaultBrowserUtils.checkForLuxxleSetDefaultBrowser(
                appOpenCount, LuxxleActivity.this);

        Context app = ContextUtils.getApplicationContext();
        if (null != app
                && LuxxleReflectionUtil.equalTypes(this.getClass(), ChromeTabbedActivity.class)) {
            // Trigger LuxxleSyncWorker CTOR to make migration from sync v1 if sync is enabled
            LuxxleSyncWorker.get();
        }

        initMiscAndroidMetrics();
        checkForNotificationData();

        if (RateUtils.getInstance().isLastSessionShown()) {
            RateUtils.getInstance().setPrefNextRateDate();
            RateUtils.getInstance().setLastSessionShown(false);
        }

        if (!RateUtils.getInstance().getPrefRateEnabled()) {
            RateUtils.getInstance().setPrefRateEnabled(true);
            RateUtils.getInstance().setPrefNextRateDate();
        }
        RateUtils.getInstance().setTodayDate();

        if (RateUtils.getInstance().shouldShowRateDialog(this)) {
            showLuxxleRateDialog();
            RateUtils.getInstance().setLastSessionShown(true);
        }

        // TODO commenting out below code as we may use it in next release

        // if (PackageUtils.isFirstInstall(this)
        //         &&
        //
        // SharedPreferencesManager.getInstance().readInt(LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT)
        //         == 1) {
        //     Calendar calender = Calendar.getInstance();
        //     calender.setTime(new Date());
        //     calender.add(Calendar.DATE, DAYS_4);
        //     OnboardingPrefManager.getInstance().setNextOnboardingDate(
        //         calender.getTimeInMillis());
        // }

        // OnboardingActivity onboardingActivity = null;
        // for (Activity ref : ApplicationStatus.getRunningActivities()) {
        //     if (!(ref instanceof OnboardingActivity)) continue;

        //     onboardingActivity = (OnboardingActivity) ref;
        // }

        // if (onboardingActivity == null
        //         && OnboardingPrefManager.getInstance().showOnboardingForSkip(this)) {
        //     OnboardingPrefManager.getInstance().showOnboarding(this);
        //     OnboardingPrefManager.getInstance().setOnboardingShownForSkip(true);
        // }

        LuxxleSyncAccountDeletedInformer.show();

        if (!OnboardingPrefManager.getInstance().isOneTimeNotificationStarted() && isFirstInstall) {
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.HOUR_3);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.HOUR_24);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DAY_6);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DAY_10);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DAY_30);
            RetentionNotificationUtil.scheduleNotification(this, RetentionNotificationUtil.DAY_35);
            OnboardingPrefManager.getInstance().setOneTimeNotificationStarted(true);
        }

        if (isFirstInstall
                && ChromeSharedPreferences.getInstance()
                                .readInt(LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT)
                        == 1) {
            Calendar calender = Calendar.getInstance();
            calender.setTime(new Date());
            calender.add(Calendar.DATE, DAYS_4);
            LuxxleRewardsHelper.setNextRewardsOnboardingModalDate(calender.getTimeInMillis());
        }

        checkFingerPrintingOnUpgrade(isFirstInstall);
        checkForVpnCallout();

        if (ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_VPN_LINK_SUBSCRIPTION_ANDROID_UI)
                && LuxxleVpnPrefUtils.isSubscriptionPurchase()
                && !LuxxleVpnPrefUtils.isLinkSubscriptionDialogShown()) {
            showLinkVpnSubscriptionDialog();
        }
        if (isFirstInstall
                && (OnboardingPrefManager.getInstance().isDormantUsersEngagementEnabled()
                        || getPackageName().equals(LuxxleConstants.LUXXLE_PRODUCTION_PACKAGE_NAME))) {
            OnboardingPrefManager.getInstance().setDormantUsersPrefs();
            if (!OnboardingPrefManager.getInstance().isDormantUsersNotificationsStarted()) {
                RetentionNotificationUtil.scheduleDormantUsersNotifications(this);
                OnboardingPrefManager.getInstance().setDormantUsersNotificationsStarted(true);
            }
        }
        initWalletNativeServices();

        mNativeInitialized = true;

        if (countryCode.equals(LuxxleConstants.INDIA_COUNTRY_CODE)
                && ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePreferenceKeys.LUXXLE_AD_FREE_CALLOUT_DIALOG, true)
                && getActivityTab() != null
                && getActivityTab().getUrl().getSpec() != null
                && UrlUtilities.isNtpUrl(getActivityTab().getUrl().getSpec())
                && (ChromeSharedPreferences.getInstance()
                                .readBoolean(LuxxlePreferenceKeys.LUXXLE_OPENED_YOUTUBE, false)
                        || ChromeSharedPreferences.getInstance()
                                        .readInt(LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT)
                                >= 7)) {
            showAdFreeCalloutDialog();
        }

        initLuxxleNews();
        if (ChromeSharedPreferences.getInstance()
                .readBoolean(LuxxlePreferenceKeys.LUXXLE_DEFERRED_DEEPLINK_PLAYLIST, false)) {
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(LuxxlePreferenceKeys.LUXXLE_DEFERRED_DEEPLINK_PLAYLIST, false);
            openPlaylist(false);
        } else if (ChromeSharedPreferences.getInstance()
                .readBoolean(LuxxlePreferenceKeys.LUXXLE_DEFERRED_DEEPLINK_VPN, false)) {
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(LuxxlePreferenceKeys.LUXXLE_DEFERRED_DEEPLINK_VPN, false);
            handleDeepLinkVpn();
        }

        // Added to reset app links settings for upgrade case
        if (!isFirstInstall
                && !ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePrivacySettings.PREF_APP_LINKS, true)
                && ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePrivacySettings.PREF_APP_LINKS_RESET, true)) {
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(LuxxlePrivacySettings.PREF_APP_LINKS, true);
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(LuxxlePrivacySettings.PREF_APP_LINKS_RESET, false);
        }

        if (isFirstInstall
                && ChromeSharedPreferences.getInstance()
                                .readInt(LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT)
                        == 1) {
            Calendar calendar = Calendar.getInstance();
            calendar.setTime(new Date());
            calendar.add(Calendar.DATE, DAYS_7);
            LuxxleRewardsHelper.setRewardsOnboardingIconTiming(calendar.getTimeInMillis());

            if (ENABLE_IN_APP_UPDATE) {
                setInAppUpdateTiming();
            }
        }

        // Check multiwindow toggle for upgrade case
        if (!isFirstInstall
                && !LuxxleMultiWindowUtils.isCheckUpgradeEnableMultiWindows()
                && MultiWindowUtils.getInstanceCount() > 1
                && !LuxxleMultiWindowUtils.shouldEnableMultiWindows()) {
            LuxxleMultiWindowUtils.setCheckUpgradeEnableMultiWindows(true);
            LuxxleMultiWindowUtils.updateEnableMultiWindows(true);
        } else if (!LuxxleMultiWindowUtils.isCheckUpgradeEnableMultiWindows()) {
            LuxxleMultiWindowUtils.setCheckUpgradeEnableMultiWindows(true);
        }

        if (ENABLE_IN_APP_UPDATE
                && System.currentTimeMillis()
                        > ChromeSharedPreferences.getInstance()
                                .readLong(LuxxlePreferenceKeys.LUXXLE_IN_APP_UPDATE_TIMING, 0)) {
            checkAppUpdate();
        }

        if (!isFirstInstall
                && !LuxxlePrefServiceBridge.getInstance().getPlayYTVideoInBrowserEnabled()
                && ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePreferenceKeys.OPEN_YT_IN_LUXXLE_DIALOG, true)) {
            openYtInLuxxleDialog();
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(LuxxlePreferenceKeys.OPEN_YT_IN_LUXXLE_DIALOG, false);
        }

        // Quick search engines views changes
        new KeyboardVisibilityHelper(LuxxleActivity.this, LuxxleActivity.this);
        AppCompatEditText urlBar = findViewById(R.id.url_bar);
        if (urlBar != null) {
            urlBar.addTextChangedListener(
                    new TextWatcher() {
                        @Override
                        public void beforeTextChanged(
                                CharSequence s, int start, int count, int after) {}

                        @Override
                        public void onTextChanged(
                                CharSequence query, int start, int before, int count) {
                            if (query.toString().isEmpty()) {
                                removeQuickActionSearchEnginesView();
                            } else {
                                if (getLuxxleToolbarLayout().isUrlBarFocused()
                                        && KeyboardUtils.isAndroidSoftKeyboardShowing(urlBar)) {
                                    View rootView = findViewById(android.R.id.content);
                                    Rect r = new Rect();
                                    rootView.getWindowVisibleDisplayFrame(r);
                                    int screenHeight = rootView.getRootView().getHeight();
                                    int visibleHeight = r.bottom;
                                    int heightDifference = screenHeight - visibleHeight;
                                    showQuickActionSearchEnginesView(heightDifference);
                                } else {
                                    removeQuickActionSearchEnginesView();
                                }
                            }
                        }

                        @Override
                        public void afterTextChanged(Editable s) {}
                    });
            if (ChromeSharedPreferences.getInstance()
                    .readBoolean(OnboardingPrefManager.SHOULD_SHOW_SEARCH_WIDGET_PROMO, false)) {
                mSearchWidgetPromoPanel = new SearchWidgetPromoPanel(LuxxleActivity.this);
                mSearchWidgetPromoPanel.showIfNeeded(urlBar);
                ChromeSharedPreferences.getInstance()
                        .writeBoolean(OnboardingPrefManager.SHOULD_SHOW_SEARCH_WIDGET_PROMO, false);
            }
        }

        ContextUtils.getAppSharedPreferences().registerOnSharedPreferenceChangeListener(this);
    }

    private void applyChangesForYahooJp() {
        boolean isDefaultSearchEngineChanged =
                ChromeSharedPreferences.getInstance()
                        .readBoolean(LuxxlePreferenceKeys.DEFAULT_SEARCH_ENGINE_CHANGED, false);
        TemplateUrlService templateUrlService =
                TemplateUrlServiceFactory.getForProfile(getCurrentProfile());
        Runnable onTemplateUrlServiceReady =
                () -> {
                    if (isActivityFinishingOrDestroyed()) return;
                    TemplateUrl yahooJpTemplateUrl =
                            LuxxleSearchEngineUtils.getTemplateUrlByShortName(
                                    getCurrentProfile(), OnboardingPrefManager.YAHOO_JP);
                    if (yahooJpTemplateUrl != null
                            && !isDefaultSearchEngineChanged
                            && templateUrlService.isDefaultSearchEngineGoogle()) {
                        LuxxleSearchEngineUtils.setDSEPrefs(yahooJpTemplateUrl, getCurrentProfile());
                        ChromeSharedPreferences.getInstance()
                                .writeBoolean(
                                        LuxxlePreferenceKeys.LUXXLE_DEFAULT_SEARCH_ENGINE_MIGRATED_JP,
                                        true);
                    }
                };
        templateUrlService.runWhenLoaded(onTemplateUrlServiceReady);
    }

    private void setLuxxleAsDefaultPrivateMode() {
        Runnable onTemplateUrlServiceReady =
                () -> {
                    if (isActivityFinishingOrDestroyed()) return;
                    TemplateUrl luxxleTemplateUrl =
                            LuxxleSearchEngineUtils.getTemplateUrlByShortName(
                                    getCurrentProfile(), OnboardingPrefManager.LUXXLE);
                    if (luxxleTemplateUrl != null) {
                        LuxxleSearchEngineUtils.setDSEPrefs(
                                luxxleTemplateUrl,
                                getCurrentProfile()
                                        .getPrimaryOtrProfile(/* createIfNeeded= */ true));
                    }
                };
        TemplateUrlServiceFactory.getForProfile(getCurrentProfile())
                .runWhenLoaded(onTemplateUrlServiceReady);
    }

    private void enableSearchSuggestions() {
        TemplateUrl defaultSearchEngineTemplateUrl =
                LuxxleSearchEngineUtils.getTemplateUrlByShortName(
                        getCurrentProfile(),
                        LuxxleSearchEngineUtils.getDSEShortName(getCurrentProfile(), false));
        if (defaultSearchEngineTemplateUrl != null
                && LUXXLE_SEARCH_ENGINE_KEYWORD.equals(
                        defaultSearchEngineTemplateUrl.getKeyword())) {
            UserPrefs.get(getCurrentProfile()).setBoolean(Pref.SEARCH_SUGGEST_ENABLED, true);
        }
    }

    private void setInAppUpdateTiming() {
        Calendar calendar = Calendar.getInstance();
        calendar.setTime(new Date());
        calendar.add(Calendar.MONTH, MONTH_1);
        ChromeSharedPreferences.getInstance()
                .writeLong(
                        LuxxlePreferenceKeys.LUXXLE_IN_APP_UPDATE_TIMING, calendar.getTimeInMillis());
    }

    private void completeUpdateSnackbar() {
        Snackbar snackbar =
                Snackbar.make(
                                getResources().getString(R.string.in_app_update_text),
                                new SnackbarController() {
                                    @Override
                                    public void onDismissNoAction(Object actionData) {}

                                    @Override
                                    public void onAction(Object actionData) {
                                        if (mAppUpdateManager != null) {
                                            mAppUpdateManager.completeUpdate();
                                            mAppUpdateManager.unregisterListener(
                                                    mInstallStateUpdatedListener);
                                        }
                                    }
                                },
                                Snackbar.TYPE_ACTION,
                                Snackbar.UMA_UNKNOWN)
                        .setAction(getResources().getString(R.string.update), null)
                        .setSingleLine(false)
                        .setDuration(10000);
        Tab currentTab = getActivityTabProvider().get();
        if (currentTab != null) {
            SnackbarManager snackbarManager =
                    SnackbarManagerProvider.from(currentTab.getWindowAndroid());
            snackbarManager.showSnackbar(snackbar);
        }
    }

    private final InstallStateUpdatedListener mInstallStateUpdatedListener =
            installState -> {
                if (installState.installStatus() == InstallStatus.DOWNLOADED) {
                    completeUpdateSnackbar();
                }
            };

    private void checkAppUpdate() {
        mAppUpdateManager = AppUpdateManagerFactory.create(LuxxleActivity.this);
        mAppUpdateManager.registerListener(mInstallStateUpdatedListener);

        Task<AppUpdateInfo> appUpdateInfoTask = mAppUpdateManager.getAppUpdateInfo();

        appUpdateInfoTask.addOnSuccessListener(
                appUpdateInfo -> {
                    if (appUpdateInfo.updateAvailability() == UpdateAvailability.UPDATE_AVAILABLE) {
                        if (appUpdateInfo.updatePriority() >= 4 /* high priority */
                                && appUpdateInfo.isUpdateTypeAllowed(AppUpdateType.IMMEDIATE)) {
                            startAppUpdateFlow(appUpdateInfo, AppUpdateType.IMMEDIATE);
                        } else {
                            startAppUpdateFlow(appUpdateInfo, AppUpdateType.FLEXIBLE);
                        }
                    }
                });
    }

    private void startAppUpdateFlow(AppUpdateInfo appUpdateInfo, int appUpdateType) {
        try {
            mAppUpdateManager.startUpdateFlowForResult(
                    appUpdateInfo, appUpdateType, LuxxleActivity.this, 1);
            setInAppUpdateTiming();
        } catch (IntentSender.SendIntentException e) {
            throw new RuntimeException(e);
        }
    }

    private void handleDeepLinkVpn() {
        mIsDeepLink = true;
        LuxxleVpnUtils.openLuxxleVpnPlansActivity(this);
    }

    private void checkForVpnCallout() {
        String countryCode = Locale.getDefault().getCountry();

        if (!countryCode.equals(LuxxleConstants.INDIA_COUNTRY_CODE)
                && LuxxleVpnUtils.isVpnFeatureSupported(LuxxleActivity.this)) {
            if (!TextUtils.isEmpty(LuxxleVpnPrefUtils.getPurchaseToken())
                    && !TextUtils.isEmpty(LuxxleVpnPrefUtils.getProductId())) {
                mIsVerification = true;
                LuxxleVpnNativeWorker.getInstance().verifyPurchaseToken(
                        LuxxleVpnPrefUtils.getPurchaseToken(), LuxxleVpnPrefUtils.getProductId(),
                        LuxxleVpnUtils.SUBSCRIPTION_PARAM_TEXT, getPackageName());
            }
        }
    }

    private void initLuxxleNews() {
        ThreadUtils.assertOnUiThread();
        if (LuxxlePrefServiceBridge.getInstance().getShowNews()
                && LuxxlePrefServiceBridge.getInstance().getNewsOptIn()) {
            LuxxleNewsUtils.getLuxxleNewsSettingsDataPerProfile(mTabModelProfileSupplier.get());
        }
    }

    public void setDormantUsersPrefs() {
        OnboardingPrefManager.getInstance().setDormantUsersPrefs();
        RetentionNotificationUtil.scheduleDormantUsersNotifications(this);
    }

    private void openPlaylist(boolean shouldHandlePlaylistActivity) {
        if (!shouldHandlePlaylistActivity) mIsDeepLink = true;

        if (ChromeSharedPreferences.getInstance()
                .readBoolean(PlaylistPreferenceUtils.SHOULD_SHOW_PLAYLIST_ONBOARDING, true)) {
            PlaylistUtils.openPlaylistMenuOnboardingActivity(LuxxleActivity.this);
            ChromeSharedPreferences.getInstance()
                    .writeBoolean(PlaylistPreferenceUtils.SHOULD_SHOW_PLAYLIST_ONBOARDING, false);
        } else if (shouldHandlePlaylistActivity) {
            openPlaylistActivity(LuxxleActivity.this, ConstantUtils.ALL_PLAYLIST);
        }
    }

    public void openPlaylistActivity(Context context, String playlistId) {
        Intent playlistActivityIntent = new Intent(context, PlaylistHostActivity.class);
        playlistActivityIntent.putExtra(ConstantUtils.PLAYLIST_ID, playlistId);
        playlistActivityIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
        playlistActivityIntent.setAction(Intent.ACTION_VIEW);
        context.startActivity(playlistActivityIntent);
    }

    private void showLinkVpnSubscriptionDialog() {
        LinkVpnSubscriptionDialogFragment linkVpnSubscriptionDialogFragment =
                new LinkVpnSubscriptionDialogFragment();
        linkVpnSubscriptionDialogFragment.setCancelable(false);
        linkVpnSubscriptionDialogFragment.show(
                getSupportFragmentManager(), "LinkVpnSubscriptionDialogFragment");
    }

    private void showAdFreeCalloutDialog() {
        ChromeSharedPreferences.getInstance()
                .writeBoolean(LuxxlePreferenceKeys.LUXXLE_AD_FREE_CALLOUT_DIALOG, false);

        LuxxleAdFreeCalloutDialogFragment luxxleAdFreeCalloutDialogFragment =
                new LuxxleAdFreeCalloutDialogFragment();
        luxxleAdFreeCalloutDialogFragment.show(
                getSupportFragmentManager(), "LuxxleAdFreeCalloutDialogFragment");
    }

    public void setNewTabPageManager(NewTabPageManager manager) {
        mNewTabPageManager = manager;
    }

    public void focusSearchBox() {
        if (mNewTabPageManager != null) {
            mNewTabPageManager.focusSearchBox(false, null);
        }
    }

    private void checkFingerPrintingOnUpgrade(boolean isFirstInstall) {
        if (!isFirstInstall
                && ChromeSharedPreferences.getInstance()
                                .readInt(LuxxlePreferenceKeys.LUXXLE_APP_OPEN_COUNT)
                        == 0) {
            boolean value =
                    ChromeSharedPreferences.getInstance()
                            .readBoolean(LuxxlePrivacySettings.PREF_FINGERPRINTING_PROTECTION, true);
            if (value) {
                LuxxleShieldsContentSettings.setShieldsValue(
                        ProfileManager.getLastUsedRegularProfile(),
                        "",
                        LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                        LuxxleShieldsContentSettings.DEFAULT,
                        false);
            } else {
                LuxxleShieldsContentSettings.setShieldsValue(
                        ProfileManager.getLastUsedRegularProfile(),
                        "",
                        LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                        LuxxleShieldsContentSettings.ALLOW_RESOURCE,
                        false);
            }
        }
    }

    public void openQuickSearchEnginesSettings() {
        SettingsNavigation settingsLauncher = SettingsNavigationFactory.createSettingsNavigation();
        settingsLauncher.startSettings(this, QuickSearchEnginesFragment.class);
    }

    public void openLuxxlePlaylistSettings() {
        SettingsNavigation settingsLauncher = SettingsNavigationFactory.createSettingsNavigation();
        settingsLauncher.startSettings(this, LuxxlePlaylistPreferences.class);
    }

    public void openLuxxleNewsSettings() {
        SettingsNavigation settingsLauncher = SettingsNavigationFactory.createSettingsNavigation();
        settingsLauncher.startSettings(this, LuxxleNewsPreferencesV2.class);
    }

    public void openLuxxleContentFilteringSettings() {
        SettingsNavigation settingsLauncher = SettingsNavigationFactory.createSettingsNavigation();
        settingsLauncher.startSettings(this, ContentFilteringFragment.class);
    }

    public int getLuxxleThemeBackgroundColor() {
        return ContextUtils.getApplicationContext()
                .getColor(R.color.toolbar_background_color_for_ntp);
    }

    public void openLuxxleCreateCustomFiltersSettings() {
        SettingsNavigation settingsLauncher = SettingsNavigationFactory.createSettingsNavigation();
        settingsLauncher.startSettings(this, CreateCustomFiltersFragment.class);
    }

    public void openLuxxleWalletSettings() {
        SettingsNavigation settingsLauncher = SettingsNavigationFactory.createSettingsNavigation();
        settingsLauncher.startSettings(this, LuxxleWalletPreferences.class);
    }

    public void openLuxxleConnectedSitesSettings() {
        SettingsNavigation settingsLauncher = SettingsNavigationFactory.createSettingsNavigation();
        settingsLauncher.startSettings(this, LuxxleWalletEthereumConnectedSites.class);
    }

    public void openLuxxleWallet(boolean fromDapp, boolean setupAction, boolean restoreAction) {
        Intent luxxleWalletIntent = new Intent(this, LuxxleWalletActivity.class);
        luxxleWalletIntent.putExtra(LuxxleWalletActivity.IS_FROM_DAPPS, fromDapp);
        luxxleWalletIntent.putExtra(LuxxleWalletActivity.RESTART_WALLET_ACTIVITY_SETUP, setupAction);
        luxxleWalletIntent.putExtra(
                LuxxleWalletActivity.RESTART_WALLET_ACTIVITY_RESTORE, restoreAction);
        luxxleWalletIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        luxxleWalletIntent.setAction(Intent.ACTION_VIEW);
        startActivity(luxxleWalletIntent);
    }

    public void openLuxxleWalletBackup() {
        Intent luxxleWalletIntent = new Intent(this, LuxxleWalletActivity.class);
        luxxleWalletIntent.putExtra(LuxxleWalletActivity.SHOW_WALLET_ACTIVITY_BACKUP, true);
        luxxleWalletIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        luxxleWalletIntent.setAction(Intent.ACTION_VIEW);
        startActivity(luxxleWalletIntent);
    }

    public void viewOnBlockExplorer(
            String address, @CoinType.EnumType int coinType, NetworkInfo networkInfo) {
        Utils.openAddress("/address/" + address, this, coinType, networkInfo);
    }

    public void openLuxxleWalletDAppsActivity(
            @NonNull final LuxxleWalletDAppsActivity.ActivityType activityType) {
        Intent luxxleWalletIntent = new Intent(this, LuxxleWalletDAppsActivity.class);
        luxxleWalletIntent.putExtra("activityType", activityType.getValue());
        luxxleWalletIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        luxxleWalletIntent.setAction(Intent.ACTION_VIEW);
        startActivity(luxxleWalletIntent);
    }

    public MiscAndroidMetrics getMiscAndroidMetrics() {
        return mMiscAndroidMetrics;
    }

    private void checkForYandexSE() {
        String countryCode = Locale.getDefault().getCountry();
        if (sYandexRegions.contains(countryCode)) {
            Profile lastUsedRegularProfile = ProfileManager.getLastUsedRegularProfile();
            TemplateUrl yandexTemplateUrl =
                    LuxxleSearchEngineUtils.getTemplateUrlByShortName(
                            lastUsedRegularProfile, OnboardingPrefManager.YANDEX);
            if (yandexTemplateUrl != null) {
                LuxxleSearchEngineUtils.setDSEPrefs(yandexTemplateUrl, lastUsedRegularProfile);
            }
        }
    }

    private void checkForNotificationData() {
        Intent notifIntent = getIntent();
        if (notifIntent != null && notifIntent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE) != null) {
            String notificationType = notifIntent.getStringExtra(RetentionNotificationUtil.NOTIFICATION_TYPE);
            switch (notificationType) {
                case RetentionNotificationUtil.HOUR_3:
                case RetentionNotificationUtil.HOUR_24:
                case RetentionNotificationUtil.EVERY_SUNDAY:
                    checkForLuxxleStats();
                    break;
                case RetentionNotificationUtil.DAY_6:
                    if (getActivityTab() != null
                            && getActivityTab().getUrl().getSpec() != null
                            && !UrlUtilities.isNtpUrl(getActivityTab().getUrl().getSpec())) {
                        getTabCreator(false).launchUrl(
                                UrlConstants.NTP_URL, TabLaunchType.FROM_CHROME_UI);
                    }
                    break;
                case RetentionNotificationUtil.DAY_10:
                case RetentionNotificationUtil.DAY_30:
                case RetentionNotificationUtil.DAY_35:
                    openRewardsPanel();
                    break;
                case RetentionNotificationUtil.DORMANT_USERS_DAY_14:
                case RetentionNotificationUtil.DORMANT_USERS_DAY_25:
                case RetentionNotificationUtil.DORMANT_USERS_DAY_40:
                    showDormantUsersEngagementDialog(notificationType);
                    break;
            }
        }
    }

    public void checkForLuxxleStats() {
        if (OnboardingPrefManager.getInstance().isLuxxleStatsEnabled()) {
            LuxxleStatsUtil.showLuxxleStats();
        } else {
            if (getActivityTab() != null
                    && getActivityTab().getUrl().getSpec() != null
                    && !UrlUtilities.isNtpUrl(getActivityTab().getUrl().getSpec())) {
                OnboardingPrefManager.getInstance().setFromNotification(true);
                if (getTabCreator(false) != null) {
                    getTabCreator(false).launchUrl(
                            UrlConstants.NTP_URL, TabLaunchType.FROM_CHROME_UI);
                }
            } else {
                showOnboardingV2(false);
            }
        }
    }

    public void showOnboardingV2(boolean fromStats) {
        try {
            OnboardingPrefManager.getInstance().setNewOnboardingShown(true);
            FragmentManager fm = getSupportFragmentManager();
            HighlightDialogFragment fragment = (HighlightDialogFragment) fm.findFragmentByTag(
                    HighlightDialogFragment.TAG_FRAGMENT);
            FragmentTransaction transaction = fm.beginTransaction();

            if (fragment != null) {
                transaction.remove(fragment);
            }

            fragment = new HighlightDialogFragment();
            Bundle fragmentBundle = new Bundle();
            fragmentBundle.putBoolean(OnboardingPrefManager.FROM_STATS, fromStats);
            fragment.setArguments(fragmentBundle);
            transaction.add(fragment, HighlightDialogFragment.TAG_FRAGMENT);
            transaction.commitAllowingStateLoss();
        } catch (IllegalStateException e) {
            Log.e("HighlightDialogFragment", e.getMessage());
        }
    }

    public void hideRewardsOnboardingIcon() {
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.hideRewardsOnboardingIcon();
    }

    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel(
                    CHANNEL_ID, getString(R.string.luxxle_browser), importance);
            channel.setDescription(
                    getString(R.string.luxxle_browser_notification_channel_description));
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }

    private boolean isNoRestoreState() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(LuxxlePreferenceKeys.LUXXLE_CLOSE_TABS_ON_EXIT, false);
    }

    private boolean isClearBrowsingDataOnExit() {
        return ChromeSharedPreferences.getInstance()
                .readBoolean(LuxxlePreferenceKeys.LUXXLE_CLEAR_ON_EXIT, false);
    }

    public void onRewardsPanelDismiss() {
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.onRewardsPanelDismiss();
    }

    public void dismissRewardsPanel() {
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.dismissRewardsPanel();
    }

    public void dismissShieldsTooltip() {
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.dismissShieldsTooltip();
    }

    public void openRewardsPanel() {
        LuxxleToolbarLayoutImpl layout = getLuxxleToolbarLayout();
        layout.openRewardsPanel();
    }

    public Profile getCurrentProfile() {
        Tab tab = getActivityTab();
        if (tab == null) {
            return ProfileManager.getLastUsedRegularProfile();
        }

        return Profile.fromWebContents(tab.getWebContents());
    }

    /** Close all tabs (including active tab) whose URL origin matches with a given origin. */
    public void closeAllTabsByOrigin(@NonNull final String origin) {
        final TabModel tabModel = getCurrentTabModel();

        Set<Integer> tabIndexes = getTabIndexesByUrlOrigin(tabModel, origin);
        for (Integer index : tabIndexes) {
            Tab tab = tabModel.getTabAt(index);
            if (tab != null) {
                tab.setClosing(true);
                tabModel.getTabRemover().closeTabs(TabClosureParams.closeTab(tab).build(), false);
            }
        }
    }

    /**
     * Selects an existing tab if it matches a given origin, marks it as active and returns it.
     *
     * @return Active tab if it exists, {@code null} otherwise.
     */
    public Tab selectExistingUrlOriginTab(@NonNull final String origin) {
        TabModel tabModel = getCurrentTabModel();
        Set<Integer> tabIndexes = getTabIndexesByUrlOrigin(tabModel, origin);

        // Find if tab exists, including tab already active.
        if (!tabIndexes.isEmpty()) {
            int index = tabIndexes.iterator().next();
            Tab tab = tabModel.getTabAt(tabIndexes.iterator().next());
            // Set active tab
            tabModel.setIndex(index, TabSelectionType.FROM_USER);
            return tab;
        } else {
            return null;
        }
    }

    /**
     * Find the {@link Tab} indexes whose URL starts with the specified base URL.
     *
     * @param model The {@link TabModel} to act on.
     * @param origin The URL origin to search for.
     * @return A set of indexes pointing to the matching {@link Tab}s or empty set if no matches are
     *     found.
     */
    @NonNull
    private static Set<Integer> getTabIndexesByUrlOrigin(
            @NonNull final TabList model, @NonNull final String origin) {
        final Set<Integer> result = new HashSet<>();
        int count = model.getCount();

        for (int i = 0; i < count; i++) {
            if (model.getTabAt(i).getUrl().getOrigin().getSpec().contentEquals(origin)) {
                result.add(i);
            }
        }
        return result;
    }

    public Tab selectExistingTab(String url) {
        Tab tab = getActivityTab();
        if (tab != null && tab.getUrl().getSpec().equals(url)) {
            return tab;
        }

        TabModel tabModel = getCurrentTabModel();
        int tabIndex = TabModelUtils.getTabIndexByUrl(tabModel, url);

        // Find if tab exists
        if (tabIndex != TabModel.INVALID_TAB_INDEX) {
            tab = tabModel.getTabAt(tabIndex);
            // Set active tab
            tabModel.setIndex(tabIndex, TabSelectionType.FROM_USER);
            return tab;
        } else {
            return null;
        }
    }

    public Tab openNewOrSelectExistingTab(String url, boolean refresh) {
        Tab tab = selectExistingTab(url);
        if (tab != null) {
            if (refresh) {
                tab.reload();
            }
            return tab;
        } else { // Open a new tab
            return getTabCreator(false).launchUrl(url, TabLaunchType.FROM_CHROME_UI);
        }
    }

    public void openNewOrRefreshExistingTab(
            @NonNull final String origin, @NonNull final String url) {
        Tab tab = selectExistingUrlOriginTab(origin);
        if (tab != null) {
            tab.reload();
        } else {
            // Open a new tab.
            getTabCreator(false).launchUrl(url, TabLaunchType.FROM_CHROME_UI);
        }
    }

    public Tab openNewOrSelectExistingTab(String url) {
        return openNewOrSelectExistingTab(url, false);
    }

    private void clearWalletModelServices() {
        if (mWalletModel == null) {
            return;
        }

        mWalletModel.resetServices(
                getApplicationContext(), null, null, null, null, null, null, null, null, null);
    }

    public void setupWalletModel() {
        PostTask.postTask(
                TaskTraits.UI_DEFAULT,
                () -> {
                    if (mWalletModel == null) {
                        mWalletModel =
                                new WalletModel(
                                        getApplicationContext(),
                                        mKeyringService,
                                        mBlockchainRegistry,
                                        mJsonRpcService,
                                        mTxService,
                                        mEthTxManagerProxy,
                                        mSolanaTxManagerProxy,
                                        mAssetRatioService,
                                        mLuxxleWalletService,
                                        mSwapService);
                    } else {
                        mWalletModel.resetServices(
                                getApplicationContext(),
                                mKeyringService,
                                mBlockchainRegistry,
                                mJsonRpcService,
                                mTxService,
                                mEthTxManagerProxy,
                                mSolanaTxManagerProxy,
                                mAssetRatioService,
                                mLuxxleWalletService,
                                mSwapService);
                    }
                    setupObservers();
                });
    }

    @MainThread
    private void setupObservers() {
        ThreadUtils.assertOnUiThread();
        clearObservers();
        mWalletModel.getCryptoModel().getPendingTxHelper().mSelectedPendingRequest.observe(
                this, transactionInfo -> {
                    if (transactionInfo == null) {
                        return;
                    }
                    // don't show dapps panel if the wallet is locked and requests are being
                    // processed by the approve dialog already
                    mKeyringService.isLocked(locked -> {
                        if (locked) {
                            return;
                        }

                        if (!mIsProcessingPendingDappsTxRequest) {
                            mIsProcessingPendingDappsTxRequest = true;
                            openLuxxleWalletDAppsActivity(
                                    LuxxleWalletDAppsActivity.ActivityType.CONFIRM_TRANSACTION);
                        }

                        // update badge if there's a pending tx
                        updateWalletBadgeVisibility();
                    });
                });

        mWalletModel.getDappsModel().mWalletIconNotificationVisible.observe(
                this, this::setWalletBadgeVisibility);

        mWalletModel.getDappsModel().mPendingWalletAccountCreationRequest.observe(this, request -> {
            if (request == null) return;
            mWalletModel.getKeyringModel().isWalletLocked(isLocked -> {
                if (!LuxxleWalletPreferences.getPrefWeb3NotificationsEnabled()) return;
                if (isLocked) {
                    Tab tab = getActivityTab();
                    if (tab != null) {
                        walletInteractionDetected(tab.getWebContents());
                    }
                    showWalletPanel(false);
                    return;
                }
                for (CryptoAccountTypeInfo info :
                        mWalletModel.getCryptoModel().getSupportedCryptoAccountTypes()) {
                    if (info.getCoinType() == request.getCoinType()) {
                        Intent intent = AddAccountActivity.createIntentToAddAccount(
                                this, info.getCoinType());
                        startActivity(intent);
                        mWalletModel.getDappsModel().removeProcessedAccountCreationRequest(request);
                        break;
                    }
                }
            });
        });

        mWalletModel.getCryptoModel().getNetworkModel().mNeedToCreateAccountForNetwork.observe(
                this, networkInfo -> {
                    if (networkInfo == null) return;

                    MaterialAlertDialogBuilder builder =
                            new MaterialAlertDialogBuilder(
                                    this, R.style.LuxxleWalletAlertDialogTheme)
                                    .setMessage(getString(
                                            R.string.luxxle_wallet_create_account_description,
                                            networkInfo.symbolName))
                                    .setPositiveButton(R.string.luxxle_action_yes,
                                            (dialog, which) -> {
                                                mWalletModel.createAccountAndSetDefaultNetwork(
                                                        networkInfo);
                                            })
                                    .setNegativeButton(
                                            R.string.luxxle_action_no, (dialog, which) -> {
                                                mWalletModel.getCryptoModel()
                                                        .getNetworkModel()
                                                        .clearCreateAccountState();
                                                dialog.dismiss();
                                            });
                    builder.show();
                });
    }

    @MainThread
    private void clearObservers() {
        ThreadUtils.assertOnUiThread();
        mWalletModel.getCryptoModel().getPendingTxHelper().mSelectedPendingRequest.removeObservers(
                this);
        mWalletModel.getDappsModel().mWalletIconNotificationVisible.removeObservers(this);
        mWalletModel.getCryptoModel()
                .getNetworkModel()
                .mNeedToCreateAccountForNetwork.removeObservers(this);
    }

    private void showLuxxleRateDialog() {
        LuxxleRateDialogFragment rateDialogFragment = LuxxleRateDialogFragment.newInstance(false);
        rateDialogFragment.show(getSupportFragmentManager(), LuxxleRateDialogFragment.TAG_FRAGMENT);
    }

    private void openYtInLuxxleDialog() {
        OpenYtInLuxxleDialogFragment mOpenYtInLuxxleDialogFragment =
                new OpenYtInLuxxleDialogFragment();
        mOpenYtInLuxxleDialogFragment.show(
                getSupportFragmentManager(), "OpenYtInLuxxleDialogFragment");
    }

    public void showDormantUsersEngagementDialog(String notificationType) {
        if (!LuxxleSetDefaultBrowserUtils.isLuxxleSetAsDefaultBrowser(LuxxleActivity.this)) {
            DormantUsersEngagementDialogFragment dormantUsersEngagementDialogFragment =
                    new DormantUsersEngagementDialogFragment();
            dormantUsersEngagementDialogFragment.setNotificationType(notificationType);
            dormantUsersEngagementDialogFragment.show(
                    getSupportFragmentManager(), "DormantUsersEngagementDialogFragment");
            setDormantUsersPrefs();
        }
    }

    private static Activity getActivityOfType(Class<?> classOfActivity) {
        for (Activity ref : ApplicationStatus.getRunningActivities()) {
            if (!classOfActivity.isInstance(ref)) continue;

            return ref;
        }

        return null;
    }

    private void enableSpeedreaderMode() {
        final Tab currentTab = getActivityTab();
        if (currentTab != null) {
            LuxxleSpeedReaderUtils.enableSpeedreaderMode(currentTab);
        }
    }

    public void openLuxxleLeo() {
        LuxxleLeoUtils.verifySubscription(null);
        Tab currentTab = getActivityTabProvider().get();
        if (currentTab != null) {
            LuxxleLeoUtils.openLeoUrlForTab(currentTab.getWebContents());
        }
    }

    public void showRewardsPage() {
        if (LuxxleRewardsHelper.shouldShowNewRewardsUI()) {
            getLuxxleToolbarLayout().showRewardsPage();
        } else {
            openNewOrSelectExistingTab(LUXXLE_REWARDS_SETTINGS_URL);
        }
    }

    public static ChromeTabbedActivity getChromeTabbedActivity() {
        return (ChromeTabbedActivity) getActivityOfType(ChromeTabbedActivity.class);
    }

    public static CustomTabActivity getCustomTabActivity() {
        return (CustomTabActivity) getActivityOfType(CustomTabActivity.class);
    }

    @NonNull
    public static LuxxleActivity getLuxxleActivity() throws LuxxleActivityNotFoundException {
        LuxxleActivity activity = (LuxxleActivity) getActivityOfType(LuxxleActivity.class);
        if (activity != null) {
            return activity;
        }

        throw new LuxxleActivityNotFoundException("LuxxleActivity Not Found");
    }

    @NonNull
    public static LuxxleActivity getLuxxleActivityFromTaskId(int taskId)
            throws LuxxleActivityNotFoundException {

        for (Activity ref : ApplicationStatus.getRunningActivities()) {
            if (!LuxxleActivity.class.isInstance(ref) || ref.getTaskId() != taskId) continue;

            return (LuxxleActivity) ref;
        }

        throw new LuxxleActivityNotFoundException("LuxxleActivity Not Found");
    }

    @Override
    public void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        if (intent != null) {
            String openUrl = intent.getStringExtra(LuxxleActivity.OPEN_URL);
            if (!TextUtils.isEmpty(openUrl)) {
                try {
                    openNewOrSelectExistingTab(openUrl);
                } catch (NullPointerException e) {
                    Log.e("LuxxleActivity", "opening new tab " + e.getMessage());
                }
            }
        }
        checkForNotificationData();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (resultCode == RESULT_OK
                && (requestCode == LuxxleConstants.VERIFY_WALLET_ACTIVITY_REQUEST_CODE
                        || requestCode == LuxxleConstants.USER_WALLET_ACTIVITY_REQUEST_CODE
                        || requestCode == LuxxleConstants.SITE_BANNER_REQUEST_CODE)) {
            dismissRewardsPanel();
            if (data != null) {
                String open_url = data.getStringExtra(LuxxleActivity.OPEN_URL);
                if (!TextUtils.isEmpty(open_url)) {
                    openNewOrSelectExistingTab(open_url);
                }
            }
        } else if (resultCode == RESULT_OK
                && requestCode == LuxxleConstants.MONTHLY_CONTRIBUTION_REQUEST_CODE) {
            dismissRewardsPanel();

        } else if (resultCode == RESULT_OK
                && requestCode == LuxxleConstants.DEFAULT_BROWSER_ROLE_REQUEST_CODE) {
            // We don't need to anything with the result here.
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onRequestPermissionsResult(
            int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        FragmentManager fm = getSupportFragmentManager();
        LuxxleStatsBottomSheetDialogFragment fragment =
                (LuxxleStatsBottomSheetDialogFragment) fm.findFragmentByTag(
                        LuxxleStatsUtil.STATS_FRAGMENT_TAG);
        if (fragment != null) {
            fragment.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }

        if (requestCode == LuxxleStatsUtil.SHARE_STATS_WRITE_EXTERNAL_STORAGE_PERM
                && grantResults.length != 0
                && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            LuxxleStatsUtil.shareStats(R.layout.luxxle_stats_share_layout);
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    @Override
    public void performPreInflationStartup() {
        LuxxleDbUtil dbUtil = LuxxleDbUtil.getInstance();
        if (dbUtil.dbOperationRequested()) {
            AlertDialog dialog =
                    new AlertDialog.Builder(this)
                            .setMessage(dbUtil.performDbExportOnStart()
                                            ? getString(
                                                    R.string.luxxle_db_processing_export_alert_info)
                                            : getString(
                                                    R.string.luxxle_db_processing_import_alert_info))
                            .setCancelable(false)
                            .create();
            dialog.setCanceledOnTouchOutside(false);
            if (dbUtil.performDbExportOnStart()) {
                dbUtil.setPerformDbExportOnStart(false);
                dbUtil.exportRewardsDb(dialog);
            } else if (dbUtil.performDbImportOnStart() && !dbUtil.dbImportFile().isEmpty()) {
                dbUtil.setPerformDbImportOnStart(false);
                dbUtil.importRewardsDb(dialog, dbUtil.dbImportFile());
            }
            dbUtil.cleanUpDbOperationRequest();
        }
        super.performPreInflationStartup();
    }

    @Override
    protected @LaunchIntentDispatcher.Action int maybeDispatchLaunchIntent(
            Intent intent, Bundle savedInstanceState) {
        boolean notificationUpdate = IntentUtils.safeGetBooleanExtra(
                intent, LuxxlePreferenceKeys.LUXXLE_UPDATE_EXTRA_PARAM, false);
        if (notificationUpdate) {
            setUpdatePreferences();
        }

        return super.maybeDispatchLaunchIntent(intent, savedInstanceState);
    }

    private void setUpdatePreferences() {
        Calendar currentTime = Calendar.getInstance();
        long milliSeconds = currentTime.getTimeInMillis();

        SharedPreferences sharedPref = getApplicationContext().getSharedPreferences(
                LuxxlePreferenceKeys.LUXXLE_NOTIFICATION_PREF_NAME, 0);
        SharedPreferences.Editor editor = sharedPref.edit();

        editor.putLong(LuxxlePreferenceKeys.LUXXLE_MILLISECONDS_NAME, milliSeconds);
        editor.apply();
    }

    public ObservableSupplier<BrowserControlsManager> getBrowserControlsManagerSupplier() {
        return mBrowserControlsManagerSupplier;
    }

    public int getToolbarShadowHeight() {
        View toolbarShadow = findViewById(R.id.toolbar_hairline);
        assert toolbarShadow != null;
        if (toolbarShadow != null) {
            return toolbarShadow.getHeight();
        }
        return 0;
    }

    public float getToolbarBottom() {
        View toolbarShadow = findViewById(R.id.toolbar_hairline);
        assert toolbarShadow != null;
        if (toolbarShadow != null) {
            return toolbarShadow.getY();
        }
        return 0;
    }

    public boolean isViewBelowToolbar(View view) {
        View toolbarShadow = findViewById(R.id.toolbar_hairline);
        assert toolbarShadow != null;
        assert view != null;
        if (toolbarShadow != null && view != null) {
            int[] coordinatesToolbar = new int[2];
            toolbarShadow.getLocationInWindow(coordinatesToolbar);
            int[] coordinatesView = new int[2];
            view.getLocationInWindow(coordinatesView);
            return coordinatesView[1] >= coordinatesToolbar[1];
        }

        return false;
    }

    @NativeMethods
    interface Natives {
        void restartStatsUpdater();
        String getSafeBrowsingApiKey();
    }

    private void initLuxxleWalletService() {
        if (mLuxxleWalletService != null) {
            return;
        }

        mLuxxleWalletService = LuxxleWalletServiceFactory.getInstance().getLuxxleWalletService(this);
    }

    private void initKeyringService() {
        if (mKeyringService != null) {
            return;
        }

        mKeyringService = LuxxleWalletServiceFactory.getInstance().getKeyringService(this);
    }

    private void initJsonRpcService() {
        if (mJsonRpcService != null) {
            return;
        }

        mJsonRpcService = LuxxleWalletServiceFactory.getInstance().getJsonRpcService(this);
    }

    private void initTxService() {
        if (mTxService != null) {
            return;
        }

        mTxService = LuxxleWalletServiceFactory.getInstance().getTxService(this);
    }

    private void initEthTxManagerProxy() {
        if (mEthTxManagerProxy != null) {
            return;
        }

        mEthTxManagerProxy = LuxxleWalletServiceFactory.getInstance().getEthTxManagerProxy(this);
    }

    private void initSolanaTxManagerProxy() {
        if (mSolanaTxManagerProxy != null) {
            return;
        }

        mSolanaTxManagerProxy =
                LuxxleWalletServiceFactory.getInstance().getSolanaTxManagerProxy(this);
    }

    private void initBlockchainRegistry() {
        if (mBlockchainRegistry != null) {
            return;
        }

        mBlockchainRegistry = BlockchainRegistryFactory.getInstance().getBlockchainRegistry(this);
    }

    private void initAssetRatioService() {
        if (mAssetRatioService != null) {
            return;
        }

        mAssetRatioService = AssetRatioServiceFactory.getInstance().getAssetRatioService(this);
    }

    @Override
    public void initMiscAndroidMetricsFromAWorkerThread() {
        runOnUiThread(
                () -> {
                    initMiscAndroidMetrics();
                });
    }

    private void initMiscAndroidMetrics() {
        ThreadUtils.assertOnUiThread();
        if (mMiscAndroidMetrics != null) {
            return;
        }
        if (mMiscAndroidMetricsConnectionErrorHandler == null) {
            mMiscAndroidMetricsConnectionErrorHandler =
                    new MiscAndroidMetricsConnectionErrorHandler(this);
        }

        MiscAndroidMetricsFactory.getInstance()
                .getMetricsService(mMiscAndroidMetricsConnectionErrorHandler)
                .then(
                        miscAndroidMetrics -> {
                            mMiscAndroidMetrics = miscAndroidMetrics;
                            mMiscAndroidMetrics.recordPrivacyHubEnabledStatus(
                                    OnboardingPrefManager.getInstance().isLuxxleStatsEnabled());
                            mMiscAndroidMetrics.recordSetAsDefault(
                                    LuxxleSetDefaultBrowserUtils.isAppSetAsDefaultBrowser(
                                            LuxxleActivity.this));
                            if (mUsageMonitor == null) {
                                mUsageMonitor = UsageMonitor.getInstance(mMiscAndroidMetrics);
                            }
                            mUsageMonitor.start();
                        });
    }

    private void initSwapService() {
        if (mSwapService != null) {
            return;
        }
        mSwapService = SwapServiceFactory.getInstance().getSwapService(this);
    }

    private void initWalletNativeServices() {
        initBlockchainRegistry();
        initTxService();
        initEthTxManagerProxy();
        initSolanaTxManagerProxy();
        initAssetRatioService();
        initLuxxleWalletService();
        initKeyringService();
        initJsonRpcService();
        initSwapService();
        setupWalletModel();
    }

    private void cleanUpWalletNativeServices() {
        clearWalletModelServices();
        if (mKeyringService != null) mKeyringService.close();
        if (mAssetRatioService != null) mAssetRatioService.close();
        if (mBlockchainRegistry != null) mBlockchainRegistry.close();
        if (mJsonRpcService != null) mJsonRpcService.close();
        if (mTxService != null) mTxService.close();
        if (mEthTxManagerProxy != null) mEthTxManagerProxy.close();
        if (mSolanaTxManagerProxy != null) mSolanaTxManagerProxy.close();
        if (mLuxxleWalletService != null) mLuxxleWalletService.close();
        mKeyringService = null;
        mBlockchainRegistry = null;
        mJsonRpcService = null;
        mTxService = null;
        mEthTxManagerProxy = null;
        mSolanaTxManagerProxy = null;
        mAssetRatioService = null;
        mLuxxleWalletService = null;
    }

    @Override
    public void cleanUpMiscAndroidMetrics() {
        if (mUsageMonitor != null) {
            mUsageMonitor.stop();
        }
        if (mMiscAndroidMetrics != null) mMiscAndroidMetrics.close();
        mMiscAndroidMetrics = null;
    }

    @NonNull
    private LuxxleToolbarLayoutImpl getLuxxleToolbarLayout() {
        LuxxleToolbarLayoutImpl layout = findViewById(R.id.toolbar);
        assert layout != null;
        return layout;
    }

    public void addOrEditBookmark(final Tab tabToBookmark) {
        RateUtils.getInstance().setPrefAddedBookmarkCount();
        ((TabBookmarker) mTabBookmarkerSupplier.get()).addOrEditBookmark(tabToBookmark);
    }

    public void showBookmarkManager(Profile profile) {
        if (mBookmarkManagerOpenerSupplier.get() != null) {
            mBookmarkManagerOpenerSupplier.get().showBookmarkManager(this, profile);
        }
    }

    // We call that method with an interval
    // LuxxleSafeBrowsingApiHandler.SAFE_BROWSING_INIT_INTERVAL_MS,
    // as upstream does, to keep the GmsCore process alive.
    private void executeInitSafeBrowsing(long delay) {
        // SafeBrowsingBridge.getSafeBrowsingState() has to be executed on a main thread
        PostTask.postDelayedTask(
                TaskTraits.UI_DEFAULT,
                () -> {
                    SafeBrowsingBridge safeBrowsingBridge =
                            new SafeBrowsingBridge(getCurrentProfile());
                    if (safeBrowsingBridge.getSafeBrowsingState()
                            != SafeBrowsingState.NO_SAFE_BROWSING) {
                        // initSafeBrowsing could be executed on a background thread
                        PostTask.postTask(
                                TaskTraits.USER_VISIBLE_MAY_BLOCK,
                                () -> {
                                    LuxxleSafeBrowsingApiHandler.getInstance().initSafeBrowsing();
                                });
                    }
                    executeInitSafeBrowsing(
                            LuxxleSafeBrowsingApiHandler.SAFE_BROWSING_INIT_INTERVAL_MS);
                },
                delay);
    }

    public void updateBottomSheetPosition(int orientation) {
        if (BottomToolbarConfiguration.isLuxxleBottomControlsEnabled()) {
            // Ensure the bottom sheet's container is adjusted to the height of the bottom toolbar.
            ViewGroup sheetContainer = findViewById(R.id.sheet_container);
            assert sheetContainer != null;

            if (sheetContainer != null) {
                CoordinatorLayout.LayoutParams params =
                        (CoordinatorLayout.LayoutParams) sheetContainer.getLayoutParams();
                params.bottomMargin =
                        orientation == Configuration.ORIENTATION_LANDSCAPE
                                ? 0
                                : getResources()
                                        .getDimensionPixelSize(R.dimen.bottom_controls_height);
                sheetContainer.setLayoutParams(params);
            }
        }
    }

    /**
     * Calls to {@link ChromeTabbedActivity#maybeHandleUrlIntent} will be redirected here via
     * bytecode changes.
     */
    public boolean maybeHandleUrlIntent(Intent intent) {
        String appLinkAction = intent.getAction();
        Uri appLinkData = intent.getData();

        if (Intent.ACTION_VIEW.equals(appLinkAction) && appLinkData != null) {
            String lastPathSegment = appLinkData.getLastPathSegment();
            if (lastPathSegment != null
                    && (lastPathSegment.equalsIgnoreCase(LuxxleConstants.DEEPLINK_ANDROID_PLAYLIST)
                            || lastPathSegment.equalsIgnoreCase(
                                    LuxxleConstants.DEEPLINK_ANDROID_VPN))) {
                return false;
            }
        }
        // Call ChromeTabbedActivity's version.
        return (boolean)
                LuxxleReflectionUtil.invokeMethod(
                        ChromeTabbedActivity.class,
                        this,
                        "maybeHandleUrlIntent",
                        Intent.class,
                        intent);
    }

    public RootUiCoordinator getRootUiCoordinator() {
        return mRootUiCoordinator;
    }

    public MultiInstanceManager getMultiInstanceManager() {
        return (MultiInstanceManager)
                LuxxleReflectionUtil.getField(
                        ChromeTabbedActivity.class, "mMultiInstanceManager", this);
    }

    private void exitLuxxle() {
        LayoutInflater inflater =
                (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = inflater.inflate(R.layout.luxxle_exit_confirmation, null);
        DialogInterface.OnClickListener onClickListener =
                (dialog, button) -> {
                    if (button == AlertDialog.BUTTON_POSITIVE) {
                        ApplicationLifetime.terminate(false);
                    } else {
                        dialog.dismiss();
                    }
                };

        AlertDialog.Builder alert =
                new AlertDialog.Builder(this, R.style.ThemeOverlay_BrowserUI_AlertDialog);
        AlertDialog alertDialog =
                alert.setTitle(R.string.menu_exit)
                        .setView(view)
                        .setPositiveButton(R.string.luxxle_action_yes, onClickListener)
                        .setNegativeButton(R.string.luxxle_action_no, onClickListener)
                        .create();
        alertDialog.getDelegate().setHandleNativeActionModesEnabled(false);
        alertDialog.show();
    }

    /*
     * Whether we want to pretend to be a custom tab. May be usefull to avoid certain patches,
     * when we want to have the same behaviour as in custom tabs.
     */
    public void spoofCustomTab(boolean spoof) {
        mSpoofCustomTab = spoof;
    }

    @Override
    public boolean isCustomTab() {
        if (mSpoofCustomTab) {
            return true;
        }

        return super.isCustomTab();
    }

    public void showQuickActionSearchEnginesView(int keypadHeight) {
        if (mQuickSearchEnginesView != null
                || !QuickSearchEnginesUtil.getQuickSearchEnginesFeature()) {
            return;
        }
        mQuickSearchEnginesView =
                getLayoutInflater().inflate(R.layout.quick_search_engines_view, null);
        RecyclerView recyclerView =
                (RecyclerView)
                        mQuickSearchEnginesView.findViewById(
                                R.id.quick_search_engines_recyclerview);
        LinearLayoutManager linearLayoutManager =
                new LinearLayoutManager(LuxxleActivity.this, LinearLayoutManager.HORIZONTAL, false);
        recyclerView.setLayoutManager(linearLayoutManager);

        ImageView quickSearchEnginesSettings =
                (ImageView)
                        mQuickSearchEnginesView.findViewById(R.id.quick_search_engines_settings);
        quickSearchEnginesSettings.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        openQuickSearchEnginesSettings();
                    }
                });

        Runnable onQuickSearchEnginesReady =
                () -> {
                    if (isActivityFinishingOrDestroyed()) return;

                    quickSearchEnginesReady(recyclerView, keypadHeight);
                };
        TemplateUrlServiceFactory.getForProfile(getCurrentProfile())
                .runWhenLoaded(onQuickSearchEnginesReady);
    }

    private void quickSearchEnginesReady(RecyclerView recyclerView, int keypadHeight) {
        List<QuickSearchEnginesModel> searchEngines =
                QuickSearchEnginesUtil.getQuickSearchEnginesForView(getCurrentProfile());

        QuickSearchEnginesModel defaultQuickSearchEnginesModel =
                QuickSearchEnginesUtil.getDefaultSearchEngine(getCurrentProfile());
        searchEngines.add(0, defaultQuickSearchEnginesModel);

        if (!getCurrentProfile().isOffTheRecord()
                && LuxxleLeoPrefUtils.shouldShowLeoQuickSearchEngine()) {
            QuickSearchEnginesModel leoQuickSearchEnginesModel =
                    new QuickSearchEnginesModel(
                            "",
                            "",
                            "",
                            true,
                            QuickSearchEnginesModel.QuickSearchEnginesModelType.AI_ASSISTANT);
            searchEngines.add(0, leoQuickSearchEnginesModel);
        }

        QuickSearchEnginesViewAdapter adapter =
                new QuickSearchEnginesViewAdapter(LuxxleActivity.this, searchEngines, this);
        recyclerView.setAdapter(adapter);
        if (mQuickSearchEnginesView.getParent() == null) {
            WindowManager.LayoutParams params =
                    new WindowManager.LayoutParams(
                            WindowManager.LayoutParams.MATCH_PARENT,
                            WindowManager.LayoutParams.WRAP_CONTENT,
                            WindowManager.LayoutParams.TYPE_APPLICATION_PANEL,
                            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                            WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS);
            params.gravity = Gravity.BOTTOM;
            params.y = keypadHeight; // Position the view above the keyboard

            WindowManager windowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
            windowManager.addView(mQuickSearchEnginesView, params);
        }
    }

    public void removeQuickActionSearchEnginesView() {
        if (mQuickSearchEnginesView != null && mQuickSearchEnginesView.getParent() != null) {
            WindowManager windowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
            windowManager.removeView(mQuickSearchEnginesView);
            mQuickSearchEnginesView = null;
        }
    }

    // QuickSearchCallback
    @Override
    public void onSearchEngineClick(int position, QuickSearchEnginesModel quickSearchEnginesModel) {
        if (getActivityTab() == null) {
            return;
        }
        String query = getLuxxleToolbarLayout().getLocationBarQuery();
        if (position == 0
                && quickSearchEnginesModel.getType()
                        == QuickSearchEnginesModel.QuickSearchEnginesModelType.AI_ASSISTANT) {
            LuxxleLeoUtils.openLeoQuery(getActivityTab().getWebContents(), "", query, true);
        } else {
            String quickSearchEngineUrl =
                    GOOGLE_SEARCH_ENGINE_KEYWORD.equals(quickSearchEnginesModel.getKeyword())
                            ? QuickSearchEnginesUtil.GOOGLE_SEARCH_ENGINE_URL
                            : quickSearchEnginesModel.getUrl();
            LoadUrlParams loadUrlParams =
                    new LoadUrlParams(
                            quickSearchEngineUrl
                                    .replace("{searchTerms}", query)
                                    .replace("{inputEncoding}", "UTF-8"));
            getActivityTab().loadUrl(loadUrlParams);
        }
        getLuxxleToolbarLayout().clearOmniboxFocus();
    }

    @Override
    public void loadSearchEngineLogo(
            ImageView logoView, QuickSearchEnginesModel quickSearchEnginesModel) {
        QuickSearchEnginesUtil.loadSearchEngineLogo(
                getCurrentProfile(), logoView, quickSearchEnginesModel.getKeyword());
    }

    @Override
    public void onKeyboardOpened(int keyboardHeight) {
        runOnUiThread(
                () -> {
                    if (!isFinishing()
                            && !isDestroyed()
                            && getLuxxleToolbarLayout().isUrlBarFocused()
                            && !getLuxxleToolbarLayout().getLocationBarQuery().isEmpty()) {
                        showQuickActionSearchEnginesView(keyboardHeight);
                    }
                });
    }

    @Override
    public void onKeyboardClosed() {
        removeQuickActionSearchEnginesView();
    }

    @Override
    public void onSharedPreferenceChanged(
            SharedPreferences sharedPreferences, @Nullable String key) {
        if (ChromePreferenceKeys.TOOLBAR_TOP_ANCHORED.equals(key)) {
            Activity currentActivity = ApplicationStatus.getLastTrackedFocusedActivity();
            if (currentActivity == null) {
                currentActivity = this;
            }
            LuxxleRelaunchUtils.askForRelaunch(currentActivity);
        }
    }

    @Override
    public void onNewIntentWithNative(Intent intent) {
        // If intent comes from our own package, check if we need to redirect upstream's urls (for
        // help, support, etc.).
        if (intent != null
                && intent.getAction() != null
                && Intent.ACTION_VIEW.equals(intent.getAction())
                && intent.getPackage() != null
                && intent.getPackage().equals(getPackageName())) {
            String url = IntentHandler.getUrlFromIntent(intent);
            if (url != null) {
                if (url.equals(LuxxleIntentHandler.CONNECTION_INFO_HELP_URL)) {
                    intent.setData(Uri.parse(LuxxleIntentHandler.LUXXLE_CONNECTION_INFO_HELP_URL));
                } else if (url.equals(LuxxleIntentHandler.FALLBACK_SUPPORT_URL)) {
                    intent.setData(Uri.parse(LuxxleIntentHandler.LUXXLE_FALLBACK_SUPPORT_URL));
                }
            }
        }
    }
}
