// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/luxxle_scheme_utils.h"

#include "testing/gtest/include/gtest/gtest.h"

TEST(LuxxleSchemeUtilsTest, ReplaceChromeToLuxxleScheme) {
  std::u16string url_string = u"chrome://settings";
  EXPECT_TRUE(luxxle_utils::ReplaceChromeToLuxxleScheme(url_string));
  EXPECT_EQ(url_string, u"luxxle://settings");

  url_string = u"chrome://flags";
  EXPECT_TRUE(luxxle_utils::ReplaceChromeToLuxxleScheme(url_string));
  EXPECT_EQ(url_string, u"luxxle://flags");

  url_string = u"https://search.luxxle.com";
  EXPECT_FALSE(luxxle_utils::ReplaceChromeToLuxxleScheme(url_string));
  EXPECT_EQ(url_string, u"https://search.luxxle.com");
}
