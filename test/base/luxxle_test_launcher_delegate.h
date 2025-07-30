/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_TEST_BASE_LUXXLE_TEST_LAUNCHER_DELEGATE_H_
#define LUXXLE_TEST_BASE_LUXXLE_TEST_LAUNCHER_DELEGATE_H_

#include "build/build_config.h"
#include "chrome/test/base/chrome_test_launcher.h"

class LuxxleTestLauncherDelegate : public ChromeTestLauncherDelegate {
 public:
  // Does not take ownership of ChromeTestSuiteRunner.
  explicit LuxxleTestLauncherDelegate(ChromeTestSuiteRunner* runner);
  LuxxleTestLauncherDelegate(const LuxxleTestLauncherDelegate&) = delete;
  LuxxleTestLauncherDelegate& operator=(const LuxxleTestLauncherDelegate&) =
      delete;
  ~LuxxleTestLauncherDelegate() override;
};

#endif  // LUXXLE_TEST_BASE_LUXXLE_TEST_LAUNCHER_DELEGATE_H_
