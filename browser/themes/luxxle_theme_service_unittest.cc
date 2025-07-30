/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(LuxxleThemeServiceTest, GetLuxxleThemeListTest) {
  dark_mode::SetUseSystemDarkModeEnabledForTest(true);
  base::Value::List list = dark_mode::GetLuxxleDarkModeTypeList();
  EXPECT_EQ(3UL, list.size());

  dark_mode::SetUseSystemDarkModeEnabledForTest(false);
  list = dark_mode::GetLuxxleDarkModeTypeList();
  EXPECT_EQ(2UL, list.size());
}
