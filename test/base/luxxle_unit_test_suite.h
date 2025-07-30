/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_TEST_BASE_LUXXLE_UNIT_TEST_SUITE_H_
#define LUXXLE_TEST_BASE_LUXXLE_UNIT_TEST_SUITE_H_

#include <memory>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/test/test_discardable_memory_allocator.h"
#include "chrome/test/base/chrome_unit_test_suite.h"

class LuxxleUnitTestSuite : public ChromeUnitTestSuite {
 public:
  LuxxleUnitTestSuite(int argc, char** argv);
  LuxxleUnitTestSuite(const LuxxleUnitTestSuite&) = delete;
  LuxxleUnitTestSuite& operator=(const LuxxleUnitTestSuite&) = delete;

 protected:
  // base::TestSuite overrides:
  void Initialize() override;
};

#endif  // LUXXLE_TEST_BASE_LUXXLE_UNIT_TEST_SUITE_H_
