/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/luxxle_pages.h"

#include "base/strings/strcat.h"
#include "luxxle/browser/ui/webui/webcompat_reporter/webcompat_reporter_dialog.h"
#include "luxxle/components/ai_chat/core/common/features.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/constants/webui_url_constants.h"
#include "luxxle/components/sidebar/browser/constants.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_navigator_params.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "chrome/browser/ui/singleton_tabs.h"
#include "chrome/common/webui_url_constants.h"
#include "url/gurl.h"

namespace luxxle {

void ShowLuxxleRewards(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kRewardsPageURL));
}

void ShowLuxxleAdblock(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kLuxxleUIAdblockURL));
}

void ShowSync(Browser* browser) {
  ShowSingletonTabOverwritingNTP(
      browser, chrome::GetSettingsUrl(chrome::kSyncSetupSubPage));
}

void ShowLuxxleNewsConfigure(Browser* browser) {
  ShowSingletonTabOverwritingNTP(
      browser, GURL("luxxle://newtab/?openSettings=LuxxleNews"));
}

void ShowShortcutsPage(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kShortcutsURL));
}

void ShowLuxxleTalk(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(sidebar::kLuxxleTalkURL));
}

void ShowFullpageChat(Browser* browser) {
  if (!ai_chat::features::IsAIChatHistoryEnabled()) {
    return;
  }
  ShowSingletonTabOverwritingNTP(browser, GURL(kAIChatUIURL));
}

void ShowWebcompatReporter(Browser* browser) {
  content::WebContents* web_contents =
      browser->tab_strip_model()->GetActiveWebContents();
  if (!web_contents) {
    return;
  }

  webcompat_reporter::OpenReporterDialog(
      web_contents, webcompat_reporter::UISource::kAppMenu);
}

void ShowLuxxleWallet(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kLuxxleUIWalletURL));
}

void ShowLuxxleWalletOnboarding(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kLuxxleUIWalletOnboardingURL));
}

void ShowLuxxleWalletAccountCreation(Browser* browser,
                                    luxxle_wallet::mojom::CoinType coin_type) {
  // Only solana is supported.
  CHECK(coin_type == luxxle_wallet::mojom::CoinType::SOL);
  ShowSingletonTabOverwritingNTP(
      browser,
      GURL(base::StrCat({kLuxxleUIWalletAccountCreationURL, "Solana"})));
}

void ShowExtensionSettings(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kExtensionSettingsURL));
}

void ShowWalletSettings(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(kWalletSettingsURL));
}

void ShowAppsPage(Browser* browser) {
  ShowSingletonTabOverwritingNTP(browser, GURL(chrome::kChromeUIAppsURL));
}

}  // namespace luxxle
