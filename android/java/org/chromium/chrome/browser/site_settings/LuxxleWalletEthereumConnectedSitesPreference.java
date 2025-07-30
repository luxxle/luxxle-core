/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.site_settings;

import android.annotation.SuppressLint;
import android.content.Context;
import android.util.AttributeSet;

import androidx.preference.Preference;
import androidx.preference.PreferenceViewHolder;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.luxxle_wallet.mojom.LuxxleWalletService;
import org.chromium.luxxle_wallet.mojom.CoinType;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.crypto_wallet.LuxxleWalletServiceFactory;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;

public class LuxxleWalletEthereumConnectedSitesPreference
        extends Preference implements ConnectionErrorHandler,
                                      LuxxleWalletEthereumConnectedSitesListAdapter
                                              .LuxxleEthereumPermissionConnectedSitesDelegate {
    private RecyclerView mRecyclerView;
    private LuxxleWalletService mLuxxleWalletService;
    private LuxxleWalletEthereumConnectedSitesListAdapter mAdapter;

    public LuxxleWalletEthereumConnectedSitesPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);

        initLuxxleWalletService();

        mRecyclerView = (RecyclerView) holder.findViewById(R.id.connected_sites_list);
        updateWebSitestList();
    }

    public void destroy() {
        mLuxxleWalletService.close();
        mLuxxleWalletService = null;
    }

    @SuppressLint("NotifyDataSetChanged")
    private void updateWebSitestList() {
        mLuxxleWalletService.getWebSitesWithPermission(CoinType.ETH, webSites -> {
            if (mAdapter == null) {
                mAdapter = new LuxxleWalletEthereumConnectedSitesListAdapter(webSites, this);
                mRecyclerView.setAdapter(mAdapter);
            } else {
                mAdapter.setWebSites(webSites);
                mAdapter.notifyDataSetChanged();
            }
        });
    }

    @Override
    public void removePermission(String webSite) {
        mLuxxleWalletService.resetWebSitePermission(CoinType.ETH, webSite, success -> {
            if (success) {
                updateWebSitestList();
            }
        });
    }

    @Override
    public void onConnectionError(MojoException e) {
        mLuxxleWalletService.close();
        mLuxxleWalletService = null;
        initLuxxleWalletService();
    }

    private void initLuxxleWalletService() {
        if (mLuxxleWalletService != null) {
            return;
        }

        mLuxxleWalletService = LuxxleWalletServiceFactory.getInstance().getLuxxleWalletService(this);
    }
}
