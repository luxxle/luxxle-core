/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/luxxle_wallet/wallet_common_ui.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace luxxle_wallet {

TEST(LuxxleWalletCommonUIUnitTest, IsLuxxleWalletOrigin) {
  ASSERT_TRUE(
      IsLuxxleWalletOrigin(url::Origin::Create(GURL(kLuxxleUIWalletPanelURL))));
  ASSERT_TRUE(
      IsLuxxleWalletOrigin(url::Origin::Create(GURL(kLuxxleUIWalletPageURL))));
  ASSERT_FALSE(IsLuxxleWalletOrigin(url::Origin::Create(GURL("https://a.com"))));
  ASSERT_FALSE(IsLuxxleWalletOrigin(url::Origin::Create(GURL())));
}

}  // namespace luxxle_wallet
