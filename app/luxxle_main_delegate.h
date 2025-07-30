/* Copyright (c) 2020 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_APP_LUXXLE_MAIN_DELEGATE_H_
#define LUXXLE_APP_LUXXLE_MAIN_DELEGATE_H_

#include <optional>

#include "base/gtest_prod_util.h"
#include "chrome/app/chrome_main_delegate.h"

class LuxxleMainDelegateUnitTest;

// Chrome implementation of ContentMainDelegate.
class LuxxleMainDelegate : public ChromeMainDelegate {
 public:
  LuxxleMainDelegate(const LuxxleMainDelegate&) = delete;
  LuxxleMainDelegate& operator=(const LuxxleMainDelegate&) = delete;
#if BUILDFLAG(IS_ANDROID)
  LuxxleMainDelegate();
#endif

  // `timestamps.exe_entry_point_ticks` is the time at which the main function
  // of the executable was entered. On Windows, StartupTimestamps contains
  // timing information for calls to base::PreReadFile. `timestamps`' lifetime
  // does not need to last beyond the constructor call.
  explicit LuxxleMainDelegate(const StartupTimestamps& timestamps);
  ~LuxxleMainDelegate() override;

 protected:
  // content::ContentMainDelegate implementation:
  content::ContentBrowserClient* CreateContentBrowserClient() override;
  content::ContentRendererClient* CreateContentRendererClient() override;
  content::ContentUtilityClient* CreateContentUtilityClient() override;
  std::optional<int> BasicStartupComplete() override;
  void PreSandboxStartup() override;
  std::optional<int> PostEarlyInitialization(
      ChromeMainDelegate::InvokedIn invoked_in) override;

 private:
  FRIEND_TEST_ALL_PREFIXES(LuxxleMainDelegateUnitTest,
                           DefaultCommandLineOverrides);
  FRIEND_TEST_ALL_PREFIXES(LuxxleMainDelegateUnitTest,
                           OverrideSwitchFromCommandLine);

  static void AppendCommandLineOptions();
};

#endif  // LUXXLE_APP_LUXXLE_MAIN_DELEGATE_H_
