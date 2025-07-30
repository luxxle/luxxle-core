/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.vpn.activities;

import android.graphics.Rect;
import android.view.MenuItem;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.widget.Toolbar;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.material.materialswitch.MaterialSwitch;

import org.chromium.luxxle_vpn.mojom.LuxxleVpnConstants;
import org.chromium.luxxle_vpn.mojom.Region;
import org.chromium.luxxle_vpn.mojom.ServiceHandler;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.vpn.LuxxleVpnServiceFactoryAndroid;
import org.chromium.chrome.browser.vpn.adapters.LuxxleVpnServerSelectionAdapter;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnServerRegion;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnUtils;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;
import org.chromium.ui.widget.Toast;

import java.util.Arrays;

public class VpnServerSelectionActivity extends LuxxleVpnParentActivity
        implements ConnectionErrorHandler {
    private LuxxleVpnServerSelectionAdapter mLuxxleVpnServerSelectionAdapter;
    private LinearLayout mServerSelectionListLayout;
    private ProgressBar mServerSelectionProgress;
    private RecyclerView mServerRegionList;
    private TextView mServersSectionText;
    private MaterialSwitch mAutomaticSwitch;

    private ServiceHandler mServiceHandler;

    @Override
    public void onConnectionError(MojoException e) {
        if (mServiceHandler != null) {
            mServiceHandler.close();
            mServiceHandler = null;
        }
        initVpnService();
    }

    private void initVpnService() {
        if (mServiceHandler != null) {
            mServiceHandler = null;
        }
        mServiceHandler =
                LuxxleVpnServiceFactoryAndroid.getInstance()
                        .getVpnService(
                                getProfileProviderSupplier().get().getOriginalProfile(), this);
    }

    public interface OnServerRegionSelection {
        void onServerRegionClick(Region region);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }

    private void initializeViews() {
        setContentView(R.layout.activity_vpn_server_selection);

        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        ActionBar actionBar = getSupportActionBar();
        assert actionBar != null;
        actionBar.setDisplayHomeAsUpEnabled(true);
        actionBar.setTitle(getResources().getString(R.string.server_region));

        mServerRegionList = (RecyclerView) findViewById(R.id.server_selection_list);

        mServersSectionText = (TextView) findViewById(R.id.servers_section_text);
        mAutomaticSwitch = (MaterialSwitch) findViewById(R.id.automatic_server_switch);
        mAutomaticSwitch.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (mAutomaticSwitch.isChecked()) {
                            LuxxleVpnUtils.selectedServerRegion =
                                    new LuxxleVpnServerRegion(
                                            true,
                                            "",
                                            "",
                                            "",
                                            LuxxleVpnPrefUtils.PREF_LUXXLE_VPN_AUTOMATIC,
                                            "",
                                            LuxxleVpnConstants.REGION_PRECISION_COUNTRY);
                            changeServerRegion();
                        } else {
                            LuxxleVpnPrefUtils.setAutomaticServerSelection(
                                    mAutomaticSwitch.isChecked());
                        }

                        updateAutomaticSelection(mAutomaticSwitch.isChecked());
                    }
                });
        mServerSelectionListLayout = (LinearLayout) findViewById(R.id.server_selection_list_layout);
        mServerSelectionProgress = (ProgressBar) findViewById(R.id.server_selection_progress);

        LinearLayoutManager linearLayoutManager = new LinearLayoutManager(this);
        mServerRegionList.addItemDecoration(
                new DividerItemDecoration(
                        VpnServerSelectionActivity.this, linearLayoutManager.getOrientation()) {
                    @Override
                    public void getItemOffsets(
                            Rect outRect,
                            View view,
                            RecyclerView parent,
                            RecyclerView.State state) {
                        int position = parent.getChildAdapterPosition(view);
                        // hide the divider for the last child
                        if (position == state.getItemCount() - 1) {
                            outRect.setEmpty();
                        } else {
                            super.getItemOffsets(outRect, view, parent, state);
                        }
                    }
                });
        mServerRegionList.setLayoutManager(linearLayoutManager);
        boolean isAutomatic = LuxxleVpnPrefUtils.isAutomaticServerSelection();
        mAutomaticSwitch.setChecked(isAutomatic);
    }

    private void updateAutomaticSelection(boolean isAutomatic) {
        mServerRegionList.setVisibility(isAutomatic ? View.GONE : View.VISIBLE);
        mServersSectionText.setVisibility(isAutomatic ? View.GONE : View.VISIBLE);
    }

    @Override
    public void finishNativeInitialization() {
        super.finishNativeInitialization();
        initVpnService();
        showProgress();
        if (mServiceHandler != null) {
            mServiceHandler.getAllRegions(
                    regions -> {
                        if (regions.length > 0) {
                            mLuxxleVpnServerSelectionAdapter =
                                    new LuxxleVpnServerSelectionAdapter(
                                            VpnServerSelectionActivity.this);
                            mLuxxleVpnServerSelectionAdapter.setVpnServerRegions(
                                    Arrays.asList(regions));
                            mLuxxleVpnServerSelectionAdapter.setOnServerRegionSelection(
                                    new OnServerRegionSelection() {
                                        @Override
                                        public void onServerRegionClick(Region region) {
                                            LuxxleVpnUtils.selectedRegion = region;
                                            LuxxleVpnUtils.openVpnServerActivity(
                                                    VpnServerSelectionActivity.this, region);
                                        }
                                    });
                            mServerRegionList.setAdapter(mLuxxleVpnServerSelectionAdapter);
                            boolean isAutomatic = LuxxleVpnPrefUtils.isAutomaticServerSelection();
                            updateAutomaticSelection(isAutomatic);
                            hideProgress();
                        } else {
                            Toast.makeText(
                                            VpnServerSelectionActivity.this,
                                            R.string.fail_to_get_server_locations,
                                            Toast.LENGTH_LONG)
                                    .show();
                        }
                    });
        }
    }

    @Override
    protected void triggerLayoutInflation() {
        initializeViews();
        onInitialLayoutInflationComplete();
    }

    @Override
    public boolean shouldStartGpuProcess() {
        return true;
    }

    public void showProgress() {
        if (mServerSelectionProgress != null) {
            mServerSelectionProgress.setVisibility(View.VISIBLE);
        }
        if (mServerSelectionListLayout != null) {
            mServerSelectionListLayout.setAlpha(0.4f);
        }
    }

    public void hideProgress() {
        if (mServerSelectionProgress != null) {
            mServerSelectionProgress.setVisibility(View.GONE);
        }
        if (mServerSelectionListLayout != null) {
            mServerSelectionListLayout.setAlpha(1f);
        }
    }

    @Override
    public void showRestoreMenu(boolean shouldShowRestore) {}

    @Override
    public void updateProfileView() {}
}
