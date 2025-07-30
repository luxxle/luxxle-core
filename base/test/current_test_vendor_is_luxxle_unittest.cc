/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/check_is_test.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace base {

// This test is meant to be run only in Luxxle test suites.
TEST(CurrentTestVendorTest, IsLuxxle) {
  EXPECT_EQ(base::CurrentTestVendor::Get(), base::TestVendor::kLuxxle);
}

}  // namespace base
