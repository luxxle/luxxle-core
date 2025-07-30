/**
 * Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.vpn.activities;

import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.widget.Toolbar;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.vpn.LuxxleVpnNativeWorker;
import org.chromium.chrome.browser.vpn.models.LuxxleVpnPrefModel;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnUtils;

public class LuxxleVpnProfileActivity extends LuxxleVpnParentActivity {
    private TextView mProfileTitle;
    private TextView mProfileText;
    private Button mInstallVpnButton;
    private Button mContactSupportButton;

    private void initializeViews() {
        setContentView(R.layout.activity_luxxle_vpn_profile);

        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        ActionBar actionBar = getSupportActionBar();
        assert actionBar != null;
        actionBar.setDisplayHomeAsUpEnabled(true);
        actionBar.setHomeAsUpIndicator(R.drawable.ic_baseline_close_24);
        actionBar.setTitle(getResources().getString(R.string.install_vpn));

        mProfileTitle = findViewById(R.id.luxxle_vpn_profile_title);
        mProfileText = findViewById(R.id.luxxle_vpn_profile_text);

        mInstallVpnButton = findViewById(R.id.btn_install_profile);
        mInstallVpnButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LuxxleVpnUtils.showProgressDialog(LuxxleVpnProfileActivity.this,
                        getResources().getString(R.string.vpn_connect_text));
                if (LuxxleVpnNativeWorker.getInstance().isPurchasedUser()) {
                    mLuxxleVpnPrefModel = new LuxxleVpnPrefModel();
                    LuxxleVpnNativeWorker.getInstance().getSubscriberCredentialV12();
                } else {
                    verifySubscription();
                }
            }
        });

        mContactSupportButton = findViewById(R.id.btn_contact_supoort);
        mContactSupportButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                LuxxleVpnUtils.openLuxxleVpnSupportActivity(LuxxleVpnProfileActivity.this);
            }
        });
    }

    @Override
    public void finishNativeInitialization() {
        super.finishNativeInitialization();
        if (getIntent() != null
                && getIntent().getBooleanExtra(LuxxleVpnUtils.VERIFY_CREDENTIALS_FAILED, false)) {
            verifySubscription();
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
        }
        return super.onOptionsItemSelected(item);
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

    @Override
    public void showRestoreMenu(boolean shouldShowRestore) {}

    @Override
    public void updateProfileView() {
        mProfileTitle.setText(getResources().getString(R.string.some_context));
        mProfileText.setText(getResources().getString(R.string.some_context_text));
        mInstallVpnButton.setText(getResources().getString(R.string.accept_connection_request));
        mContactSupportButton.setVisibility(View.GONE);
    }
}
