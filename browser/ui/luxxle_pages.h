/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_LUXXLE_PAGES_H_
#define LUXXLE_BROWSER_UI_LUXXLE_PAGES_H_

// REMOVED: #include "luxxle/components/luxxle_wallet/.*"

class Browser;

namespace luxxle {

void ShowLuxxleAdblock(Browser* browser);
void ShowWebcompatReporter(Browser* browser);
void ShowLuxxleRewards(Browser* browser);
void ShowLuxxleWallet(Browser* browser);
void ShowLuxxleWalletOnboarding(Browser* browser);
void ShowLuxxleWalletAccountCreation(Browser* browser,
                                    luxxle_wallet::mojom::CoinType coin_type);
void ShowExtensionSettings(Browser* browser);
void ShowWalletSettings(Browser* browser);
void ShowSync(Browser* browser);
void ShowLuxxleNewsConfigure(Browser* browser);
void ShowShortcutsPage(Browser* browser);
void ShowLuxxleTalk(Browser* browser);
void ShowFullpageChat(Browser* browser);

void ShowAppsPage(Browser* browser);

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_UI_LUXXLE_PAGES_H_
