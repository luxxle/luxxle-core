/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.util;

import org.chromium.luxxle_wallet.mojom.AccountId;
import org.chromium.luxxle_wallet.mojom.AccountInfo;
import org.chromium.luxxle_wallet.mojom.LuxxleWalletService;

import java.util.Arrays;
import java.util.HashSet;
import java.util.stream.Collectors;

public class AccountsPermissionsHelper {
    private LuxxleWalletService mLuxxleWalletService;
    private AccountInfo[] mAccounts;
    private HashSet<AccountInfo> mAccountsWithPermissions;

    public AccountsPermissionsHelper(
            LuxxleWalletService luxxleWalletService, AccountInfo[] accounts) {
        assert luxxleWalletService != null;
        assert accounts != null;
        mLuxxleWalletService = luxxleWalletService;
        mAccounts = accounts;
        mAccountsWithPermissions = new HashSet<AccountInfo>();
    }

    public HashSet<AccountInfo> getAccountsWithPermissions() {
        return mAccountsWithPermissions;
    }

    @SuppressWarnings("NoStreams")
    private static boolean containsAccount(AccountId[] accounts, AccountId searchFor) {
        return Arrays.stream(accounts)
                .anyMatch(
                        acc -> {
                            return WalletUtils.accountIdsEqual(acc, searchFor);
                        });
    }

    @SuppressWarnings("NoStreams")
    public void checkAccounts(Runnable runWhenDone) {
        AccountId[] allAccountIds =
                Arrays.stream(mAccounts).map(acc -> acc.accountId).toArray(AccountId[]::new);
        mLuxxleWalletService.hasPermission(
                allAccountIds,
                (success, filteredAccounts) -> {
                    mAccountsWithPermissions =
                            Arrays.stream(mAccounts)
                                    .filter(acc -> containsAccount(filteredAccounts, acc.accountId))
                                    .collect(Collectors.toCollection(HashSet::new));

                    runWhenDone.run();
                });
    }
}
