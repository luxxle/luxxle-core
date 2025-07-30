/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/api/luxxle_theme_api.h"
#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"
#include "luxxle/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "extensions/browser/api_test_utils.h"
#include "extensions/common/extension_builder.h"

using extensions::api::LuxxleThemeGetLuxxleThemeTypeFunction;
using extensions::api_test_utils::RunFunctionAndReturnSingleResult;

class LuxxleThemeAPIBrowserTest : public InProcessBrowserTest {
 public:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();
    extension_ = extensions::ExtensionBuilder("Test").Build();
  }

  scoped_refptr<const extensions::Extension> extension() {
    return extension_;
  }

 private:
  scoped_refptr<const extensions::Extension> extension_;
};

IN_PROC_BROWSER_TEST_F(LuxxleThemeAPIBrowserTest,
                       LuxxleThemeGetLuxxleThemeTypeTest) {
  // Change to Light type and check it from api.
  dark_mode::SetLuxxleDarkModeType(
      dark_mode::LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_LIGHT);
  EXPECT_EQ(dark_mode::LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_LIGHT,
            dark_mode::GetActiveLuxxleDarkModeType());

  scoped_refptr<LuxxleThemeGetLuxxleThemeTypeFunction> get_function(
      new LuxxleThemeGetLuxxleThemeTypeFunction());
  get_function->set_extension(extension().get());
  auto value = RunFunctionAndReturnSingleResult(
      get_function.get(), std::string("[]"), browser()->profile());
  ASSERT_TRUE(value);
  EXPECT_EQ(value->GetString(), "Light");
}
