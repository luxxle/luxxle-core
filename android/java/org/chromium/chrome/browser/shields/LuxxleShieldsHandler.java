/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.shields;

import android.app.Activity;
import android.app.Dialog;
import android.content.Context;
import android.content.ContextWrapper;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.graphics.drawable.ColorDrawable;
import android.net.Uri;
import android.os.Build;
import android.text.SpannableString;
import android.text.method.LinkMovementMethod;
import android.text.method.ScrollingMovementMethod;
import android.text.style.StyleSpan;
import android.view.ContextThemeWrapper;
import android.view.LayoutInflater;
import android.view.Surface;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;

import androidx.appcompat.widget.SwitchCompat;

import org.chromium.base.LuxxleFeatureList;
import org.chromium.base.Log;
import org.chromium.base.SysUtils;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.LuxxleRewardsHelper;
import org.chromium.chrome.browser.LuxxleRewardsNativeWorker;
import org.chromium.chrome.browser.app.LuxxleActivity;
import org.chromium.chrome.browser.luxxle_stats.LuxxleStatsUtil;
import org.chromium.chrome.browser.cosmetic_filters.LuxxleCosmeticFiltersUtils;
import org.chromium.chrome.browser.customtabs.CustomTabActivity;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.onboarding.OnboardingPrefManager;
import org.chromium.chrome.browser.preferences.website.LuxxleShieldsContentSettings;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.toolbar.bottom.BottomToolbarConfiguration;
import org.chromium.chrome.browser.ui.edge_to_edge.EdgeToEdgeUtils;
import org.chromium.chrome.browser.util.ConfigurationUtils;
import org.chromium.chrome.browser.webcompat_reporter.WebcompatReporterServiceFactory;
import org.chromium.components.browser_ui.widget.ChromeDialog;
import org.chromium.components.version_info.LuxxleVersionConstants;
import org.chromium.ui.widget.ChromeImageButton;
import org.chromium.webcompat_reporter.mojom.ReportInfo;
import org.chromium.webcompat_reporter.mojom.WebcompatReporterHandler;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

/** Object responsible for handling the creation, showing, hiding of the LuxxleShields menu. */
public class LuxxleShieldsHandler implements LuxxleRewardsHelper.LargeIconReadyCallback {
    private static final String TAG = "LuxxleShieldsHandler";
    private static final String CHROME_ERROR = "chrome-error://";

    private static class BlockersInfo {
        public BlockersInfo() {
            mAdsBlocked = 0;
            mTrackersBlocked = 0;
            mScriptsBlocked = 0;
            mFingerprintsBlocked = 0;
            mBlockerNames = new ArrayList<String>();
        }

        public int mAdsBlocked;
        public int mTrackersBlocked;
        public int mScriptsBlocked;
        public int mFingerprintsBlocked;
        public ArrayList<String> mBlockerNames;
    }

    private Context mContext;
    private PopupWindow mPopupWindow;
    private LuxxleShieldsMenuObserver mMenuObserver;
    private View mHardwareButtonMenuAnchor;
    private final Map<Integer, BlockersInfo> mTabsStat =
            Collections.synchronizedMap(new HashMap<Integer, BlockersInfo>());
    private SwitchCompat mLuxxleShieldsBlockingScriptsSwitch;
    private OnCheckedChangeListener mLuxxleShieldsBlockingScriptsChangeListener;
    private SwitchCompat mLuxxleShieldsForgetFirstPartyStorageSwitch;
    private OnCheckedChangeListener mLuxxleShieldsForgetFirstPartyStorageChangeListener;
    private SwitchCompat mFingerprintingSwitch;
    private OnCheckedChangeListener mLuxxleShieldsFingerprintingChangeListener;

    private View mPopupView;
    private View mAnchorView;
    private LinearLayout mMainLayout;
    private LinearLayout mSecondaryLayout;
    private LinearLayout mThirdLayout;
    private LinearLayout mAboutLayout;
    private LinearLayout mToggleLayout;
    private LinearLayout mThankYouLayout;
    private LinearLayout mPrivacyReportLayout;
    private LinearLayout mReportBrokenSiteLayout;
    private LinearLayout mReportErrorPageLayout;
    private TextView mSiteBlockCounterText;
    private View mBottomDivider;
    private ImageView mToggleIcon;

    private LuxxleRewardsNativeWorker mLuxxleRewardsNativeWorker;
    private LuxxleRewardsHelper mIconFetcher;
    private TextView mReportBrokenSiteDisclaimerView;

    private String mUrlSpec;
    private String mHost;
    private int mTabId;
    private Profile mProfile;
    public boolean isDisconnectEntityLoaded;
    private CheckBox mCheckBoxScreenshot;
    private EditText mEditTextDetails;
    private EditText mEditTextContact;
    private TextView mTextContactInfoApopup;
    private View mDialogView;
    private Dialog mDialog;
    private ImageView mImageView;
    private TextView mViewScreenshot;
    private byte[] mScreenshotBytes;

    private WebcompatReporterHandler mWebcompatReporterHandler;

    private static Context scanForActivity(Context cont) {
        if (cont == null) {
            return null;
        } else if (cont instanceof Activity) {
            return cont;
        } else if (cont instanceof ContextWrapper) {
            return scanForActivity(((ContextWrapper)cont).getBaseContext());
        }

        return cont;
    }

    /**
     * Constructs a LuxxleShieldsHandler object.
     *
     * @param context Context that is using the LuxxleShieldsMenu.
     */
    public LuxxleShieldsHandler(Context context) {
        Context contextCandidate = scanForActivity(context);
        mHardwareButtonMenuAnchor = null;
        mContext = (contextCandidate != null && (contextCandidate instanceof Activity))
                ? contextCandidate
                : null;

        if (mContext != null) {
            mHardwareButtonMenuAnchor = ((Activity)mContext).findViewById(R.id.menu_anchor_stub);
        }
    }

    public void addStat(int tabId, String blockType, String subResource) {
        if (!mTabsStat.containsKey(tabId)) {
            mTabsStat.put(tabId, new BlockersInfo());
        }
        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        if (blockType.equals(LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_ADS)) {
            blockersInfo.mAdsBlocked++;
        } else if (blockType.equals(LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS)) {
            blockersInfo.mTrackersBlocked++;
        } else if (blockType.equals(LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
            blockersInfo.mScriptsBlocked++;
        } else if (blockType.equals(
                           LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING)) {
            blockersInfo.mFingerprintsBlocked++;
        }
    }

    public void removeStat(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return;
        }
        mTabsStat.remove(tabId);
    }

    public void clearLuxxleShieldsCount(int tabId) {
        mTabsStat.put(tabId, new BlockersInfo());
    }

    public void addObserver(LuxxleShieldsMenuObserver menuObserver) {
        mMenuObserver = menuObserver;
    }

    private void ensureInitializedForCustomTabs() {
        if (mHardwareButtonMenuAnchor == null && mContext == null) {
            mContext = LuxxleActivity.getCustomTabActivity();
            mHardwareButtonMenuAnchor = ((Activity) mContext).findViewById(R.id.menu_anchor_stub);
        }
    }

    public void show(View anchorView, Tab tab) {
        // Current class can  be initialized by WarmupManager.inflateViewHierarchy
        // prior to creation of proper activity. In such case activity is available later.
        // Try to find it, and give up if somehow we won't be able.
        ensureInitializedForCustomTabs();

        if (mHardwareButtonMenuAnchor == null || mContext == null) {
            return;
        }

        mUrlSpec = tab.getUrl().getSpec();
        mHost = tab.getUrl().getHost();
        mTabId = tab.getId();
        mProfile = Profile.fromWebContents(tab.getWebContents());

        mLuxxleRewardsNativeWorker = LuxxleRewardsNativeWorker.getInstance();
        mIconFetcher = new LuxxleRewardsHelper(tab);
        mPopupWindow = showPopupMenu(anchorView);

        updateValues(mTabId);
    }

    private PopupWindow showPopupMenu(View anchorView) {
        assert (mContext != null);
        assert (mHardwareButtonMenuAnchor != null);

        int rotation = ((Activity)mContext).getWindowManager().getDefaultDisplay().getRotation();
        // This fixes the bug where the bottom of the menu starts at the top of
        // the keyboard, instead of overlapping the keyboard as it should.
        int displayHeight = mContext.getResources().getDisplayMetrics().heightPixels;
        int widthHeight = mContext.getResources().getDisplayMetrics().widthPixels;

        // In appcompat 23.2.1, DisplayMetrics are not updated after rotation change. This is a
        // workaround for it. See crbug.com/599048.
        // TODO(ianwen): Remove the rotation check after we roll to 23.3.0.
        if (rotation == Surface.ROTATION_0 || rotation == Surface.ROTATION_180) {
            displayHeight = Math.max(displayHeight, widthHeight);
        } else if (rotation == Surface.ROTATION_90 || rotation == Surface.ROTATION_270) {
            displayHeight = Math.min(displayHeight, widthHeight);
        } else {
            assert false : "Rotation unexpected";
        }
        if (anchorView == null) {
            Rect rect = new Rect();
            ((Activity)mContext).getWindow().getDecorView().getWindowVisibleDisplayFrame(rect);
            int statusBarHeight = rect.top;
            mHardwareButtonMenuAnchor.setY((displayHeight - statusBarHeight));

            anchorView = mHardwareButtonMenuAnchor;
        }

        ContextThemeWrapper wrapper = new ContextThemeWrapper(mContext, R.style.OverflowMenuThemeOverlay);
        Point pt = new Point();
        ((Activity)mContext).getWindowManager().getDefaultDisplay().getSize(pt);
        // Get the height and width of the display.
        Rect appRect = new Rect();
        ((Activity)mContext).getWindow().getDecorView().getWindowVisibleDisplayFrame(appRect);

        // Use full size of window for abnormal appRect.
        if (appRect.left < 0 && appRect.top < 0) {
            appRect.left = 0;
            appRect.top = 0;
            appRect.right = ((Activity)mContext).getWindow().getDecorView().getWidth();
            appRect.bottom = ((Activity)mContext).getWindow().getDecorView().getHeight();
        }

        LayoutInflater inflater = (LayoutInflater) anchorView.getContext().getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        mPopupView = inflater.inflate(R.layout.luxxle_shields_main_layout, null);
        setUpViews();

        // Specify the length and width through constants
        int width;
        if (ConfigurationUtils.isLandscape(mContext)) {
            width = (int) (mContext.getResources().getDisplayMetrics().widthPixels * 0.50);
        } else {
            width = (int) (mContext.getResources().getDisplayMetrics().widthPixels * 0.75);
        }
        int height = LinearLayout.LayoutParams.WRAP_CONTENT;

        //Make Inactive Items Outside Of PopupWindow
        boolean focusable = true;

        //Create a window with our parameters
        PopupWindow popupWindow = new PopupWindow(mPopupView, width, height, focusable);
        popupWindow.setBackgroundDrawable(new ColorDrawable(Color.WHITE));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            popupWindow.setElevation(20);
        }
        // mPopup.setBackgroundDrawable(mContext.getResources().getDrawable(android.R.drawable.picture_frame));
        // Set the location of the window on the screen
        mAnchorView = anchorView;
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
                                    popupWindow.update(mAnchorView, 0, -newHeight, width, height);
                                }
                            });
        }
        popupWindow.showAsDropDown(mAnchorView, 0, -1 * mesuredHeight);
        popupWindow.setAnimationStyle(
                BottomToolbarConfiguration.isToolbarTopAnchored()
                        ? R.style.AnchoredPopupAnimEndTop
                        : R.style.AnchoredPopupAnimEndBottom);

        // Turn off window animations for low end devices, and on Android M, which has built-in menu
        // animations.
        if (SysUtils.isLowEndDevice() || Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            popupWindow.setAnimationStyle(0);
        }

        Rect bgPadding = new Rect();
        int popupWidth = wrapper.getResources().getDimensionPixelSize(R.dimen.menu_width)
                         + bgPadding.left + bgPadding.right;
        popupWindow.setWidth(popupWidth);

        return popupWindow;
    }

    public void updateUrlSpec(String urlSpec) {
        mUrlSpec = urlSpec;
    }

    public void updateValues(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return;
        }
        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        updateValues(
                blockersInfo.mAdsBlocked + blockersInfo.mTrackersBlocked,
                blockersInfo.mScriptsBlocked,
                blockersInfo.mFingerprintsBlocked);
    }

    public int getAdsBlockedCount(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return 0;
        }

        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        return blockersInfo.mAdsBlocked;
    }

    public int getTrackersBlockedCount(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return 0;
        }

        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        return blockersInfo.mTrackersBlocked;
    }

    public ArrayList<String> getBlockerNamesList(int tabId) {
        if (!mTabsStat.containsKey(tabId)) {
            return new ArrayList<String>();
        }

        BlockersInfo blockersInfo = mTabsStat.get(tabId);
        return blockersInfo.mBlockerNames;
    }

    public void updateValues(int adsAndTrackers, int scriptsBlocked, int fingerprintsBlocked) {
        if (mContext == null) {
            return;
        }
        final int fadsAndTrackers = adsAndTrackers;
        final int fscriptsBlocked = scriptsBlocked;
        final int ffingerprintsBlocked = fingerprintsBlocked;
        ((Activity) mContext)
                .runOnUiThread(
                        new Runnable() {
                            @Override
                            public void run() {
                                if (!isShowing()) {
                                    return;
                                }
                                try {
                                    mSiteBlockCounterText.setText(
                                            String.valueOf(
                                                    fadsAndTrackers
                                                            + fscriptsBlocked
                                                            + ffingerprintsBlocked));
                                } catch (NullPointerException exc) {
                                    // It means that the Bravery Panel was destroyed during the
                                    // update, we just do nothing
                                }
                            }
                        });
    }

    public boolean isShowing() {
        if (null == mPopupWindow) {
            return false;
        }

        return mPopupWindow.isShowing();
    }

    public void hideLuxxleShieldsMenu() {
        if (isShowing()) {
            mPopupWindow.dismiss();
        }
        if (mWebcompatReporterHandler != null) {
            mWebcompatReporterHandler.close();
            mWebcompatReporterHandler = null;
        }
    }

    private void initViews() {
        mMainLayout = mPopupView.findViewById(R.id.main_layout);
        mSecondaryLayout = mPopupView.findViewById(R.id.luxxle_shields_secondary_layout_id);
        mThirdLayout = mPopupView.findViewById(R.id.luxxle_shields_third_layout_id);
        mAboutLayout = mPopupView.findViewById(R.id.luxxle_shields_about_layout_id);
        mToggleLayout = mPopupView.findViewById(R.id.luxxle_shields_toggle_layout_id);
        mSiteBlockCounterText = mPopupView.findViewById(R.id.site_block_count_text);

        mReportBrokenSiteLayout = mPopupView.findViewById(R.id.luxxle_shields_report_site_layout_id);
        mReportErrorPageLayout =
                mPopupView.findViewById(R.id.luxxle_shields_report_error_page_layout_id);
        mThankYouLayout = mPopupView.findViewById(R.id.luxxle_shields_thank_you_layout_id);
        mPrivacyReportLayout = mPopupView.findViewById(R.id.luxxle_shields_privacy_report_layout_id);

        mBottomDivider = mToggleLayout.findViewById(R.id.bottom_divider);
        mToggleIcon = mToggleLayout.findViewById(R.id.toggle_favicon);
    }

    private void setUpMainLayout() {
        if (mContext == null) return;

        String favIconURL = mLuxxleRewardsNativeWorker.getPublisherFavIconURL(mTabId);
        Tab currentActiveTab = mIconFetcher.getTab();
        String url = currentActiveTab.getUrl().getSpec();
        final String favicon_url = favIconURL.isEmpty() ? url : favIconURL;
        mIconFetcher.retrieveLargeIcon(favicon_url, this);

        TextView mSiteText = mMainLayout.findViewById(R.id.site_text);
        mSiteText.setText(mHost.replaceFirst("^(http[s]?://www\\.|http[s]?://|www\\.)", ""));

        SwitchCompat mShieldMainSwitch = mMainLayout.findViewById(R.id.site_switch);

        ImageView helpImage = (ImageView) mMainLayout.findViewById(R.id.help);
        ImageView shareImage = (ImageView) mMainLayout.findViewById(R.id.share);

        helpImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mMainLayout.setVisibility(View.GONE);
                mAboutLayout.setVisibility(View.VISIBLE);
                setUpAboutLayout();
            }
        });

        shareImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mMainLayout.setVisibility(View.GONE);
                try {
                    if (LuxxleStatsUtil.hasWritePermission(LuxxleActivity.getLuxxleActivity())) {
                        LuxxleStatsUtil.shareStats(R.layout.luxxle_stats_share_layout);
                    }
                } catch (LuxxleActivity.LuxxleActivityNotFoundException e) {
                    Log.e(TAG, "setUpMainLayout shareImage click " + e);
                }
            }
        });

        mToggleIcon.setColorFilter(mContext.getColor(R.color.shield_toggle_button_tint));
        mToggleLayout.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        setToggleView(!mSecondaryLayout.isShown());
                    }
                });
        mThirdLayout.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        LuxxleShieldsContentSettings.resetCosmeticFilter(mUrlSpec);
                        hideLuxxleShieldsMenu();

                        Tab currentActiveTab = mIconFetcher.getTab();
                        currentActiveTab.reload();
                    }
                });

        ImageView mPrivacyReportIcon = mPrivacyReportLayout.findViewById(R.id.toggle_favicon);
        mPrivacyReportIcon.setImageResource(R.drawable.ic_arrow_forward);
        mPrivacyReportIcon.setColorFilter(mContext.getColor(R.color.default_icon_color_baseline));
        TextView mViewPrivacyReportText = mPrivacyReportLayout.findViewById(R.id.toggle_text);
        mViewPrivacyReportText.setText(R.string.view_full_privacy_report);
        mPrivacyReportLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                LuxxleStatsUtil.showLuxxleStats();
                hideLuxxleShieldsMenu();
            }
        });
        if (OnboardingPrefManager.getInstance().isLuxxleStatsEnabled()) {
            mPrivacyReportLayout.setVisibility(View.VISIBLE);
        } else {
            mPrivacyReportLayout.setVisibility(View.GONE);
        }

        setUpSecondaryLayout();

        setupMainSwitchClick(mShieldMainSwitch);

        mWebcompatReporterHandler =
                WebcompatReporterServiceFactory.getInstance()
                        .getWebcompatReporterHandler(mProfile, null);
    }

    private void setToggleView(boolean shouldShow) {
        if (shouldShow) {
            mSecondaryLayout.setVisibility(View.VISIBLE);
            mBottomDivider.setVisibility(View.VISIBLE);
            mToggleIcon.setImageResource(R.drawable.ic_toggle_up);
        } else {
            mSecondaryLayout.setVisibility(View.GONE);
            mBottomDivider.setVisibility(View.GONE);
            mToggleIcon.setImageResource(R.drawable.ic_toggle_down);
        }
    }

    private void setUpSecondaryLayout() {
        TextView shieldsText = mSecondaryLayout.findViewById(R.id.luxxle_shields_text);
        shieldsText.setText(mHost.replaceFirst("^(http[s]?://www\\.|http[s]?://|www\\.)", ""));

        setUpSwitchLayouts();

        setupDetailsLayouts();
    }

    private void setupDetailsLayouts() {
        if (mContext == null) return;

        ArrayList<String> detailsLayouts = new ArrayList<>();
        detailsLayouts.add(LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS);
        detailsLayouts.add(LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES);
        if (ChromeFeatureList.isEnabled(LuxxleFeatureList.HTTPS_BY_DEFAULT)) {
            detailsLayouts.add(LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_HTTPS_UPGRADE);
        } else {
            mPopupView
                    .findViewById(R.id.luxxle_shields_secondary_https_upgrade_layout_id)
                    .setVisibility(View.GONE);
        }
        if (ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_SHOW_STRICT_FINGERPRINTING_MODE)) {
            detailsLayouts.add(LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING);
        } else {
            mPopupView
                    .findViewById(R.id.luxxle_shields_fingerprinting_layout_id)
                    .setVisibility(View.GONE);
        }

        int layoutId = 0;
        int mSecondaryLayoutId = 0;
        int titleStringId = 0;
        int subtitleStringId = 0;
        int option1StringId = 0;
        int option2StringId = 0;
        int option3StringId = 0;

        for (final String layout : detailsLayouts) {
            switch (layout) {
                case LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_HTTPS_UPGRADE:
                    layoutId = R.id.luxxle_shields_https_upgrade_layout_id;
                    mSecondaryLayoutId = R.id.luxxle_shields_secondary_https_upgrade_layout_id;
                    titleStringId = R.string.https_upgrade_title;
                    subtitleStringId = R.string.https_upgrade_summary;
                    option1StringId = R.string.https_upgrade_option_1;
                    option2StringId = R.string.https_upgrade_option_2;
                    option3StringId = R.string.https_upgrade_option_3;
                    break;
                case LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS:
                    layoutId = R.id.luxxle_shields_block_cross_trackers_layout_id;
                    mSecondaryLayoutId = R.id.luxxle_shields_cross_site_trackers_layout_id;
                    titleStringId = R.string.block_trackers_ads_title;
                    subtitleStringId = R.string.block_trackers_ads_summary;
                    option1StringId = R.string.block_trackers_ads_option_1;
                    option2StringId = R.string.block_trackers_ads_option_2;
                    option3StringId = R.string.block_trackers_ads_option_3;
                    break;
                case LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING:
                    layoutId = R.id.luxxle_shields_block_fingerprinting_layout_id;
                    mSecondaryLayoutId = R.id.luxxle_shields_fingerprinting_layout_id;
                    titleStringId = R.string.block_fingerprinting;
                    subtitleStringId = R.string.block_fingerprinting_text;
                    option1StringId = R.string.block_fingerprinting_option_1;
                    option2StringId = R.string.block_fingerprinting_option_2;
                    option3StringId = R.string.block_fingerprinting_option_3;
                    break;
                case LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES:
                    layoutId = R.id.luxxle_shields_block_cookies_layout_id;
                    mSecondaryLayoutId = R.id.luxxle_shields_cookies_layout_id;
                    titleStringId = R.string.block_cookies;
                    subtitleStringId = R.string.block_cookies_text;
                    option1StringId = R.string.block_cookies_option_1;
                    option2StringId = R.string.block_cookies_option_2;
                    option3StringId = R.string.block_cookies_option_3;
                    break;
            }

            LinearLayout mBlockShieldsLayout = mPopupView.findViewById(layoutId);
            TextView mBlockShieldsOptionTitle = mBlockShieldsLayout.findViewById(R.id.option_title);
            mBlockShieldsOptionTitle.setText(titleStringId);
            TextView mBlockShieldsOptionText = mBlockShieldsLayout.findViewById(R.id.option_text);
            mBlockShieldsOptionText.setText(subtitleStringId);
            RadioButton mBlockShieldsOption1 = mBlockShieldsLayout.findViewById(R.id.option1);
            mBlockShieldsOption1.setText(option1StringId);
            RadioButton mBlockShieldsOption2 = mBlockShieldsLayout.findViewById(R.id.option2);
            mBlockShieldsOption2.setText(option2StringId);
            RadioButton mBlockShieldsOption3 = mBlockShieldsLayout.findViewById(R.id.option3);
            mBlockShieldsOption3.setText(option3StringId);
            Button mBlockShieldsDoneButton = mBlockShieldsLayout.findViewById(R.id.done_button);
            mBlockShieldsDoneButton.setOnClickListener(mDoneClickListener);
            ImageView mBlockShieldsBackButton = mBlockShieldsLayout.findViewById(R.id.back_button);
            mBlockShieldsBackButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    mBlockShieldsLayout.setVisibility(View.GONE);
                    mMainLayout.setVisibility(View.VISIBLE);
                }
            });

            LinearLayout mShieldsLayout = mSecondaryLayout.findViewById(mSecondaryLayoutId);
            mShieldsLayout.setBackground(null);
            mShieldsLayout.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    mMainLayout.setVisibility(View.GONE);
                    mBlockShieldsLayout.setVisibility(View.VISIBLE);
                }
            });
            ImageView mBlockShieldsIcon = mShieldsLayout.findViewById(R.id.toggle_favicon);
            mBlockShieldsIcon.setImageResource(R.drawable.ic_chevron_right);
            mBlockShieldsIcon.setColorFilter(
                    mContext.getColor(R.color.default_icon_color_baseline));
            TextView mBlockShieldsText = mShieldsLayout.findViewById(R.id.toggle_text);
            mBlockShieldsText.setText(titleStringId);

            String settingOption =
                    LuxxleShieldsContentSettings.getShieldsValue(mProfile, mUrlSpec, layout);
            if (settingOption.equals(LuxxleShieldsContentSettings.BLOCK_RESOURCE)) {
                mBlockShieldsOption1.setChecked(true);
            } else if (settingOption.equals(LuxxleShieldsContentSettings.ALLOW_RESOURCE)) {
                mBlockShieldsOption3.setChecked(true);
            } else {
                boolean checkOption2 = false;
                switch (layout) {
                    case LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_HTTPS_UPGRADE:
                    // fall through
                    case LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_TRACKERS:
                        checkOption2 = settingOption.equals(LuxxleShieldsContentSettings.DEFAULT)
                                || settingOption.equals(
                                        LuxxleShieldsContentSettings.BLOCK_THIRDPARTY_RESOURCE);
                        break;
                    case LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING:
                        checkOption2 = settingOption.equals(LuxxleShieldsContentSettings.DEFAULT);
                        break;
                    case LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_COOKIES:
                        checkOption2 = settingOption.equals(
                                LuxxleShieldsContentSettings.BLOCK_THIRDPARTY_RESOURCE);
                        break;
                }
                if (checkOption2) mBlockShieldsOption2.setChecked(true);
            }

            RadioGroup mBlockShieldsOptionGroup =
                    mBlockShieldsLayout.findViewById(R.id.options_radio_group);
            mBlockShieldsOptionGroup.setOnCheckedChangeListener(
                    new RadioGroup.OnCheckedChangeListener() {
                        @Override
                        public void onCheckedChanged(RadioGroup group, int checkedId) {
                            RadioButton checkedRadioButton =
                                    (RadioButton) group.findViewById(checkedId);
                            boolean isChecked = checkedRadioButton.isChecked();
                            if (isChecked) {
                                if (checkedId == R.id.option1) {
                                    LuxxleShieldsContentSettings.setShieldsValue(mProfile, mUrlSpec,
                                            layout, LuxxleShieldsContentSettings.BLOCK_RESOURCE,
                                            false);
                                } else if (checkedId == R.id.option2) {
                                    LuxxleShieldsContentSettings.setShieldsValue(mProfile, mUrlSpec,
                                            layout,
                                            layout.equals(
                                                    LuxxleShieldsContentSettings
                                                            .RESOURCE_IDENTIFIER_FINGERPRINTING)
                                                    ? LuxxleShieldsContentSettings.DEFAULT
                                                    : LuxxleShieldsContentSettings
                                                              .BLOCK_THIRDPARTY_RESOURCE,
                                            false);
                                } else if (checkedId == R.id.option3) {
                                    LuxxleShieldsContentSettings.setShieldsValue(mProfile, mUrlSpec,
                                            layout, LuxxleShieldsContentSettings.ALLOW_RESOURCE,
                                            false);
                                }
                                if (null != mMenuObserver) {
                                    mMenuObserver.onMenuTopShieldsChanged(isChecked, false);
                                }
                            }
                        }
                    });
        }
    }

    private void setUpSwitchLayouts() {
        LinearLayout upgradeHttpsLayout =
                mSecondaryLayout.findViewById(R.id.luxxle_shields_upgrade_https_id);
        if (ChromeFeatureList.isEnabled(LuxxleFeatureList.HTTPS_BY_DEFAULT)) {
            upgradeHttpsLayout.setVisibility(View.GONE);
        }

        LinearLayout mBlockScriptsLayout = mSecondaryLayout.findViewById(R.id.luxxle_shields_block_scripts_id);
        TextView mBlockScriptsText = mBlockScriptsLayout.findViewById(R.id.luxxle_shields_switch_text);
        mLuxxleShieldsBlockingScriptsSwitch = mBlockScriptsLayout.findViewById(R.id.luxxle_shields_switch);
        mBlockScriptsText.setText(R.string.luxxle_shields_blocks_scripts_switch);
        setupBlockingScriptsSwitchClick(mLuxxleShieldsBlockingScriptsSwitch);

        LinearLayout forgetFirstPartyStorageLayout =
                mSecondaryLayout.findViewById(R.id.luxxle_shields_forget_first_party_storage_id);
        if (ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_FORGET_FIRST_PARTY_STORAGE)) {
            TextView forgetFirstPartyStorageText =
                    forgetFirstPartyStorageLayout.findViewById(R.id.luxxle_shields_switch_text);
            forgetFirstPartyStorageText.setText(R.string.luxxle_forget_first_party_storage_switch);
            mLuxxleShieldsForgetFirstPartyStorageSwitch =
                    forgetFirstPartyStorageLayout.findViewById(R.id.luxxle_shields_switch);
            setupForgetFirstPartyStorageSwitchClick(mLuxxleShieldsForgetFirstPartyStorageSwitch);
        } else {
            forgetFirstPartyStorageLayout.setVisibility(View.GONE);
        }

        LinearLayout fingerprintingSwitchLayout =
                mSecondaryLayout.findViewById(R.id.luxxle_shields_fingerprinting_switch_id);
        if (!ChromeFeatureList.isEnabled(LuxxleFeatureList.LUXXLE_SHOW_STRICT_FINGERPRINTING_MODE)) {
            TextView fingerprintingSwitchText =
                    fingerprintingSwitchLayout.findViewById(R.id.luxxle_shields_switch_text);
            mFingerprintingSwitch =
                    fingerprintingSwitchLayout.findViewById(R.id.luxxle_shields_switch);
            setupFingerprintingSwitchClick(mFingerprintingSwitch);
            fingerprintingSwitchText.setText(R.string.block_fingerprinting);
        } else {
            fingerprintingSwitchLayout.setVisibility(View.GONE);
        }

        Tab currentActiveTab = mIconFetcher.getTab();
        final boolean isPrivateWindow =
                currentActiveTab != null ? currentActiveTab.isIncognito() : false;

        TextView blockElementsText =
                mSecondaryLayout.findViewById(R.id.luxxle_shields_block_element_text);
        blockElementsText.setVisibility(
                !isPrivateWindow
                                && ChromeFeatureList.isEnabled(
                                        LuxxleFeatureList.LUXXLE_SHIELDS_ELEMENT_PICKER)
                        ? View.VISIBLE
                        : View.GONE);
        blockElementsText.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        hideLuxxleShieldsMenu();
                        Tab currentActiveTab = mIconFetcher.getTab();
                        LuxxleCosmeticFiltersUtils.launchContentPickerForWebContent(
                                currentActiveTab);
                    }
                });
    }

    private void setUpAboutLayout() {
        TextView mAboutText = mAboutLayout.findViewById(R.id.about_text);
        mAboutText.setVisibility(View.VISIBLE);
        TextView mOptionTitle = mAboutLayout.findViewById(R.id.option_title);
        mOptionTitle.setText(R.string.about_luxxle_shields_text);
        TextView mOptionText = mAboutLayout.findViewById(R.id.option_text);
        mOptionText.setVisibility(View.GONE);
        RadioGroup mOptionGroup = mAboutLayout.findViewById(R.id.options_radio_group);
        mOptionGroup.setVisibility(View.GONE);
        Button mDoneButton = mAboutLayout.findViewById(R.id.done_button);
        mDoneButton.setOnClickListener(mDoneClickListener);
        ImageView mBackButton = mAboutLayout.findViewById(R.id.back_button);
        mBackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mAboutLayout.setVisibility(View.GONE);
                mMainLayout.setVisibility(View.VISIBLE);
            }
        });
    }

    private void setUpReportBrokenSiteLayout() {
        TextView mReportSiteUrlText = mReportBrokenSiteLayout.findViewById(R.id.report_site_url);
        final Uri reportUri = Uri.parse(mUrlSpec).buildUpon().clearQuery().build();
        final String host = mHost.replaceFirst("^(http[s]?://www\\.|http[s]?://|www\\.)", "");
        final String siteUrl = mUrlSpec.startsWith(CHROME_ERROR) ? host : reportUri.toString();
        mReportSiteUrlText.setText(siteUrl);
        mReportSiteUrlText.setMovementMethod(new ScrollingMovementMethod());

        mReportBrokenSiteDisclaimerView =
                mReportBrokenSiteLayout.findViewById(R.id.report_broken_site_disclaimer_text);
        String reportBrokenSiteDisclaimerText =
                String.format(
                        mContext.getResources().getString(R.string.report_broken_site_text_2),
                        mContext.getResources().getString(R.string.report_broken_site_text_link));

        mReportBrokenSiteDisclaimerView.setText(
                LuxxleRewardsHelper.toSpannableString(
                        reportBrokenSiteDisclaimerText,
                        R.color.luxxle_link,
                        R.string.report_broken_site_text_link,
                        (context) -> {
                            CustomTabActivity.showInfoPage(
                                    context, LuxxleActivity.LUXXLE_WEBCOMPAT_INFO_WIKI_URL);
                        }));
        mReportBrokenSiteDisclaimerView.setMovementMethod(LinkMovementMethod.getInstance());

        Button mCancelButton = mReportBrokenSiteLayout.findViewById(R.id.btn_cancel);
        mCancelButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        hideLuxxleShieldsMenu();
                    }
                });

        mViewScreenshot = mReportBrokenSiteLayout.findViewById(R.id.view_screenshot_link);

        LuxxleShieldsScreenshotUtil luxxleShieldsScreenshotUtil =
                new LuxxleShieldsScreenshotUtil(
                        mContext,
                        (byte[] pngBytes) -> {
                            try {
                                if (pngBytes == null || pngBytes.length == 0) {
                                    mScreenshotBytes = null;
                                    mViewScreenshot.setVisibility(View.GONE);
                                    return;
                                }
                                mScreenshotBytes = pngBytes;
                                setUpViewScreenshot(pngBytes);
                            } catch (Exception e) {
                                Log.e(TAG, "LuxxleShieldsScreenshot failed " + e);
                            } finally {
                                mCheckBoxScreenshot.setEnabled(true);
                            }
                        });

        mCheckBoxScreenshot =
                mReportBrokenSiteLayout.findViewById(R.id.checkbox_include_screenshot);
        mCheckBoxScreenshot.setOnCheckedChangeListener(
                (buttonView, isChecked) -> {
                    mScreenshotBytes = null;
                    if (isChecked) {
                        mCheckBoxScreenshot.setEnabled(false);
                        luxxleShieldsScreenshotUtil.capture();
                    } else {
                        mViewScreenshot.setVisibility(View.GONE);
                    }
                });
        mEditTextDetails = mReportBrokenSiteLayout.findViewById(R.id.details_info_text);
        mEditTextContact = mReportBrokenSiteLayout.findViewById(R.id.contact_info_text);
        mTextContactInfoApopup =
                mReportBrokenSiteLayout.findViewById(R.id.contact_info_apopup_label);

        Button mSubmitButton = mReportBrokenSiteLayout.findViewById(R.id.btn_submit);
        mSubmitButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        if (mWebcompatReporterHandler != null) {
                            mWebcompatReporterHandler.submitWebcompatReport(getReportInfo(siteUrl));
                        }
                        mReportBrokenSiteLayout.setVisibility(View.GONE);
                        mThankYouLayout.setVisibility(View.VISIBLE);
                    }
                });
        if (mWebcompatReporterHandler != null) {
            mWebcompatReporterHandler.getContactInfo(
                    (contactInfo, contactInfoSaveFlag) -> {
                        if (contactInfo != null && !contactInfo.isEmpty()) {
                            mEditTextContact.setText(contactInfo);
                        }
                        mTextContactInfoApopup.setVisibility(
                                contactInfoSaveFlag ? View.VISIBLE : View.GONE);
                    });
        }
    }

    private ReportInfo getReportInfo(String siteUrl) {
        ReportInfo reportInfo = new ReportInfo();
        reportInfo.channel = LuxxleVersionConstants.CHANNEL;
        reportInfo.luxxleVersion = LuxxleVersionConstants.VERSION;
        reportInfo.reportUrl = siteUrl;
        reportInfo.screenshotPng = isScreenshotAvailable() ? mScreenshotBytes : null;
        reportInfo.details = mEditTextDetails.getText().toString();
        reportInfo.contact = mEditTextContact.getText().toString();
        return reportInfo;
    }

    private void setUpViewScreenshot(byte[] pngBytes) {
        mViewScreenshot.setVisibility(View.VISIBLE);
        mViewScreenshot.setText(getScreenshotSpinnableString(pngBytes));
        mViewScreenshot.setMovementMethod(LinkMovementMethod.getInstance());
    }

    private SpannableString getScreenshotSpinnableString(byte[] pngBytes) {
        return LuxxleRewardsHelper.toSpannableString(
                mContext.getResources()
                        .getString(R.string.report_broken_site_text_view_screenshot_label),
                R.color.luxxle_link,
                R.string.report_broken_site_text_view_screenshot_label,
                (context) -> showPreviewDialog(pngBytes));
    }

    private void showPreviewDialog(byte[] pngBytes) {
        mDialogView =
                ((Activity) mContext)
                        .getLayoutInflater()
                        .inflate(R.layout.report_broken_site_screenshot_view, null);

        ChromeImageButton okButton = mDialogView.findViewById(R.id.ok_button);
        okButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        mDialog.cancel();
                    }
                });

        mDialog =
                new ChromeDialog(
                        (Activity) mContext,
                        R.style.ThemeOverlay_BrowserUI_Fullscreen,
                        EdgeToEdgeUtils.isEdgeToEdgeEverywhereEnabled());
        mDialog.addContentView(
                mDialogView,
                new LinearLayout.LayoutParams(
                        LinearLayout.LayoutParams.MATCH_PARENT,
                        LinearLayout.LayoutParams.MATCH_PARENT));
        mImageView = mDialogView.findViewById(R.id.screenshot_image);
        mImageView.setScaleType(ImageView.ScaleType.FIT_START);
        mImageView.setImageBitmap(BitmapFactory.decodeByteArray(pngBytes, 0, pngBytes.length));
        mDialog.show();
    }

    private boolean isScreenshotAvailable() {
        return mViewScreenshot != null
                && mViewScreenshot.getVisibility() == View.VISIBLE
                && mCheckBoxScreenshot != null
                && mCheckBoxScreenshot.isChecked()
                && mScreenshotBytes != null
                && mScreenshotBytes.length > 0;
    }

    private void setupErrorPageLayout() {
        Button closeButton = mReportErrorPageLayout.findViewById(R.id.btn_close);
        closeButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        hideLuxxleShieldsMenu();
                    }
                });
    }

    private void setUpMainSwitchLayout(boolean isChecked) {
        if (mContext == null) return;

        TextView mShieldDownText = mMainLayout.findViewById(R.id.shield_down_text);
        Button mReportBrokenSiteButton = mMainLayout.findViewById(R.id.btn_report_broken_site);
        mReportBrokenSiteButton.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View view) {
                        Tab currentActiveTab = mIconFetcher.getTab();
                        if (currentActiveTab == null) {
                            return;
                        }

                        mMainLayout.setVisibility(View.GONE);
                        if (!currentActiveTab.isShowingErrorPage()) {
                            mReportBrokenSiteLayout.setVisibility(View.VISIBLE);
                            setUpReportBrokenSiteLayout();
                        } else {
                            mReportErrorPageLayout.setVisibility(View.VISIBLE);
                            setupErrorPageLayout();
                        }
                    }
                });

        LinearLayout mSiteBlockLayout = mMainLayout.findViewById(R.id.site_block_layout);
        TextView siteBrokenWarningText = mMainLayout.findViewById(R.id.site_broken_warning_text);

        TextView mShieldsUpText = mMainLayout.findViewById(R.id.shield_up_text);
        String mLuxxleShieldsText =
                mContext.getResources().getString(R.string.luxxle_shields_onboarding_title);

        if (isChecked) {
            mShieldDownText.setVisibility(View.GONE);
            mReportBrokenSiteButton.setVisibility(View.GONE);

            mSiteBlockLayout.setVisibility(View.VISIBLE);
            siteBrokenWarningText.setVisibility(View.VISIBLE);
            mToggleLayout.setVisibility(View.VISIBLE);
            mThirdLayout.setVisibility(
                    LuxxleShieldsContentSettings.areAnyBlockedElementsPresent(mUrlSpec)
                            ? View.VISIBLE
                            : View.GONE);

            String mUpText = mContext.getResources().getString(R.string.up);
            SpannableString mSpanString = new SpannableString(mLuxxleShieldsText + " " + mUpText);
            mSpanString.setSpan(new StyleSpan(Typeface.BOLD), mSpanString.length() - mUpText.length(), mSpanString.length(), 0);
            mShieldsUpText.setText(mSpanString);
        } else {
            mShieldDownText.setVisibility(View.VISIBLE);
            mReportBrokenSiteButton.setVisibility(View.VISIBLE);

            mSiteBlockLayout.setVisibility(View.GONE);
            siteBrokenWarningText.setVisibility(View.GONE);
            mToggleLayout.setVisibility(View.GONE);
            mThirdLayout.setVisibility(View.GONE);
            setToggleView(false);

            String mDownText = mContext.getResources().getString(R.string.down);
            SpannableString mSpanString = new SpannableString(mLuxxleShieldsText + " " + mDownText);
            mSpanString.setSpan(new StyleSpan(Typeface.BOLD), mSpanString.length() - mDownText.length(), mSpanString.length(), 0);
            mShieldsUpText.setText(mSpanString);
        }
    }

    private void setUpViews() {
        initViews();

        setUpMainLayout();
    }

    private void setupBlockingScriptsSwitchClick(SwitchCompat luxxleShieldsBlockingScriptsSwitch) {
        if (null == luxxleShieldsBlockingScriptsSwitch) {
            return;
        }
        setupBlockingScriptsSwitch(luxxleShieldsBlockingScriptsSwitch, false);

        mLuxxleShieldsBlockingScriptsChangeListener = new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {
                if (0 != mUrlSpec.length()) {
                    LuxxleShieldsContentSettings.setShields(mProfile, mUrlSpec,
                            LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS, isChecked,
                            false);
                    if (null != mMenuObserver) {
                        mMenuObserver.onMenuTopShieldsChanged(isChecked, false);
                    }
                }
            }
        };

        luxxleShieldsBlockingScriptsSwitch.setOnCheckedChangeListener(mLuxxleShieldsBlockingScriptsChangeListener);
    }

    private void setupBlockingScriptsSwitch(
            SwitchCompat luxxleShieldsBlockingScriptsSwitch, boolean fromTopSwitch) {
        if (null == luxxleShieldsBlockingScriptsSwitch) {
            return;
        }
        if (fromTopSwitch) {
            // Prevents to fire an event when top shields changed
            luxxleShieldsBlockingScriptsSwitch.setOnCheckedChangeListener(null);
        }
        if (0 != mUrlSpec.length()) {
            if (LuxxleShieldsContentSettings.getShields(mProfile, mUrlSpec,
                        LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_LUXXLE_SHIELDS)) {
                if (LuxxleShieldsContentSettings.getShields(mProfile, mUrlSpec,
                            LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_JAVASCRIPTS)) {
                    luxxleShieldsBlockingScriptsSwitch.setChecked(true);
                } else {
                    luxxleShieldsBlockingScriptsSwitch.setChecked(false);
                }
                luxxleShieldsBlockingScriptsSwitch.setEnabled(true);
            } else {
                luxxleShieldsBlockingScriptsSwitch.setChecked(false);
                luxxleShieldsBlockingScriptsSwitch.setEnabled(false);
            }
        }
        if (fromTopSwitch) {
            luxxleShieldsBlockingScriptsSwitch.setOnCheckedChangeListener(mLuxxleShieldsBlockingScriptsChangeListener);
        }
    }

    private void setupForgetFirstPartyStorageSwitchClick(
            SwitchCompat luxxleShieldsForgetFirstPartyStorageSwitch) {
        if (null == luxxleShieldsForgetFirstPartyStorageSwitch) {
            return;
        }
        setupForgetFirstPartyStorageSwitch(luxxleShieldsForgetFirstPartyStorageSwitch, false);

        mLuxxleShieldsForgetFirstPartyStorageChangeListener = new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (0 != mUrlSpec.length()) {
                    LuxxleShieldsContentSettings.setShields(mProfile, mUrlSpec,
                            LuxxleShieldsContentSettings
                                    .RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE,
                            isChecked, false);
                }
            }
        };

        luxxleShieldsForgetFirstPartyStorageSwitch.setOnCheckedChangeListener(
                mLuxxleShieldsForgetFirstPartyStorageChangeListener);
    }

    private void setupForgetFirstPartyStorageSwitch(
            SwitchCompat luxxleShieldsForgetFirstPartyStorageSwitch, boolean fromTopSwitch) {
        if (null == luxxleShieldsForgetFirstPartyStorageSwitch) {
            return;
        }
        if (fromTopSwitch) {
            // Prevents to fire an event when top shields changed
            luxxleShieldsForgetFirstPartyStorageSwitch.setOnCheckedChangeListener(null);
        }
        if (0 != mUrlSpec.length()) {
            if (LuxxleShieldsContentSettings.getShields(mProfile, mUrlSpec,
                        LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_LUXXLE_SHIELDS)) {
                if (LuxxleShieldsContentSettings.getShields(mProfile, mUrlSpec,
                            LuxxleShieldsContentSettings
                                    .RESOURCE_IDENTIFIER_FORGET_FIRST_PARTY_STORAGE)) {
                    luxxleShieldsForgetFirstPartyStorageSwitch.setChecked(true);
                } else {
                    luxxleShieldsForgetFirstPartyStorageSwitch.setChecked(false);
                }
                luxxleShieldsForgetFirstPartyStorageSwitch.setEnabled(true);
            } else {
                luxxleShieldsForgetFirstPartyStorageSwitch.setChecked(false);
                luxxleShieldsForgetFirstPartyStorageSwitch.setEnabled(false);
            }
        }
        if (fromTopSwitch) {
            luxxleShieldsForgetFirstPartyStorageSwitch.setOnCheckedChangeListener(
                    mLuxxleShieldsForgetFirstPartyStorageChangeListener);
        }
    }

    private void setupFingerprintingSwitchClick(SwitchCompat fingerprintingSwitch) {
        if (null == fingerprintingSwitch) {
            return;
        }
        setupFingerprintingSwitch(fingerprintingSwitch, false);

        mLuxxleShieldsFingerprintingChangeListener =
                new OnCheckedChangeListener() {
                    @Override
                    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                        if (0 != mUrlSpec.length()) {
                            LuxxleShieldsContentSettings.setShieldsValue(
                                    mProfile,
                                    mUrlSpec,
                                    LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING,
                                    isChecked
                                            ? LuxxleShieldsContentSettings.DEFAULT
                                            : LuxxleShieldsContentSettings.ALLOW_RESOURCE,
                                    false);
                        }
                    }
                };

        fingerprintingSwitch.setOnCheckedChangeListener(mLuxxleShieldsFingerprintingChangeListener);
    }

    private void setupFingerprintingSwitch(
            SwitchCompat luxxleShieldsFingerprintingSwitch, boolean fromTopSwitch) {
        if (null == luxxleShieldsFingerprintingSwitch) {
            return;
        }
        if (fromTopSwitch) {
            // Prevents to fire an event when top shields changed
            luxxleShieldsFingerprintingSwitch.setOnCheckedChangeListener(null);
        }
        if (0 != mUrlSpec.length()) {
            if (LuxxleShieldsContentSettings.getShields(
                    mProfile,
                    mUrlSpec,
                    LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_LUXXLE_SHIELDS)) {
                if (!LuxxleShieldsContentSettings.getShieldsValue(
                                mProfile,
                                mUrlSpec,
                                LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_FINGERPRINTING)
                        .equals(LuxxleShieldsContentSettings.ALLOW_RESOURCE)) {
                    luxxleShieldsFingerprintingSwitch.setChecked(true);
                } else {
                    luxxleShieldsFingerprintingSwitch.setChecked(false);
                }
                luxxleShieldsFingerprintingSwitch.setEnabled(true);
            } else {
                luxxleShieldsFingerprintingSwitch.setChecked(false);
                luxxleShieldsFingerprintingSwitch.setEnabled(false);
            }
        }
        if (fromTopSwitch) {
            luxxleShieldsFingerprintingSwitch.setOnCheckedChangeListener(
                    mLuxxleShieldsFingerprintingChangeListener);
        }
    }

    private void setupMainSwitchClick(SwitchCompat luxxleShieldsSwitch) {
        if (null == luxxleShieldsSwitch) {
            return;
        }
        if (0 != mUrlSpec.length()) {
            if (LuxxleShieldsContentSettings.getShields(mProfile, mUrlSpec,
                        LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_LUXXLE_SHIELDS)) {
                luxxleShieldsSwitch.setChecked(true);
                setUpMainSwitchLayout(true);
            } else {
                luxxleShieldsSwitch.setChecked(false);
                setUpMainSwitchLayout(false);
            }
        }
        luxxleShieldsSwitch.setOnCheckedChangeListener(new OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView,
                                         boolean isChecked) {
                if (0 != mUrlSpec.length()) {
                    LuxxleShieldsContentSettings.setShields(mProfile, mUrlSpec,
                            LuxxleShieldsContentSettings.RESOURCE_IDENTIFIER_LUXXLE_SHIELDS,
                            isChecked, false);
                    setupBlockingScriptsSwitch(mLuxxleShieldsBlockingScriptsSwitch, true);
                    setupForgetFirstPartyStorageSwitch(
                            mLuxxleShieldsForgetFirstPartyStorageSwitch, true);
                    if (null != mMenuObserver) {
                        mMenuObserver.onMenuTopShieldsChanged(isChecked, true);
                    }
                }

                setUpMainSwitchLayout(isChecked);
            }
        });
    }

    @Override
    public void onLargeIconReady(Bitmap icon) {
        setFavIcon(icon);
    }

    private void setFavIcon(Bitmap bmp) {
        if (bmp != null && mContext != null) {
            ((Activity)mContext).runOnUiThread(
            new Runnable() {
                @Override
                public void run() {
                    ImageView iv = (ImageView) mPopupView.findViewById(R.id.site_favicon);
                    if (iv != null) iv.setImageBitmap(LuxxleRewardsHelper.getCircularBitmap(bmp));
                }
            });
        }
    }

    private View.OnClickListener mDoneClickListener = new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            hideLuxxleShieldsMenu();
        }
    };
}
