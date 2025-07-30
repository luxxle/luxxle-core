/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.vpn.settings;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.TextView;

import androidx.preference.Preference;
import androidx.preference.PreferenceViewHolder;

import org.chromium.luxxle_vpn.mojom.LuxxleVpnConstants;
import org.chromium.chrome.R;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnPrefUtils;
import org.chromium.chrome.browser.vpn.utils.LuxxleVpnUtils;

public class VpnRegionPreference extends Preference {
    private Context mContext;

    public VpnRegionPreference(Context context, AttributeSet attrs) {
        super(context, attrs);
        mContext = context;
    }

    @Override
    public void onBindViewHolder(PreferenceViewHolder holder) {
        super.onBindViewHolder(holder);

        TextView regionFlag = (TextView) holder.findViewById(R.id.region_flag);
        if (!LuxxleVpnPrefUtils.getRegionIsoCode().isEmpty()) {
            regionFlag.setText(
                    LuxxleVpnUtils.countryCodeToEmoji(LuxxleVpnPrefUtils.getRegionIsoCode()));
        }

        String serverLocationTitle = LuxxleVpnPrefUtils.getRegionCountry();
        String optimalString = "%s - %s";
        String serverLocationSummary =
                LuxxleVpnPrefUtils.getRegionPrecision()
                                .equals(LuxxleVpnConstants.REGION_PRECISION_COUNTRY)
                        ? String.format(
                                optimalString,
                                mContext.getString(R.string.optimal_text),
                                LuxxleVpnPrefUtils.getHostnameDisplay())
                        : LuxxleVpnPrefUtils.getHostnameDisplay();

        TextView regionTitle = (TextView) holder.findViewById(R.id.region_title);
        regionTitle.setText(serverLocationTitle);
        TextView regionSummary = (TextView) holder.findViewById(R.id.region_summary);
        regionSummary.setText(serverLocationSummary);

        holder.itemView.setOnClickListener(
                new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        LuxxleVpnUtils.openVpnServerSelectionActivity(mContext);
                    }
                });
    }
}
