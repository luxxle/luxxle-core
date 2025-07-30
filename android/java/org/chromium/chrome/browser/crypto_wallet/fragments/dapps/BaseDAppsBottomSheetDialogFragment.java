/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.fragments.dapps;

import android.app.Activity;

import org.chromium.luxxle_wallet.mojom.LuxxleWalletService;
import org.chromium.luxxle_wallet.mojom.JsonRpcService;
import org.chromium.luxxle_wallet.mojom.KeyringService;
import org.chromium.chrome.browser.crypto_wallet.activities.LuxxleWalletBaseActivity;
import org.chromium.chrome.browser.crypto_wallet.fragments.WalletBottomSheetDialogFragment;

public class BaseDAppsBottomSheetDialogFragment extends WalletBottomSheetDialogFragment {
    public LuxxleWalletService getLuxxleWalletService() {
        Activity activity = getActivity();
        if (activity instanceof LuxxleWalletBaseActivity) {
            return ((LuxxleWalletBaseActivity) activity).getLuxxleWalletService();
        }

        return null;
    }

    public KeyringService getKeyringService() {
        Activity activity = getActivity();
        if (activity instanceof LuxxleWalletBaseActivity) {
            return ((LuxxleWalletBaseActivity) activity).getKeyringService();
        }

        return null;
    }

    public JsonRpcService getJsonRpcService() {
        Activity activity = getActivity();
        if (activity instanceof LuxxleWalletBaseActivity) {
            return ((LuxxleWalletBaseActivity) activity).getJsonRpcService();
        }

        return null;
    }
}
