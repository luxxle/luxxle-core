/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/extensions/luxxle_theme_event_router.h"
#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"
#include "luxxle/browser/themes/luxxle_theme_service.h"
#include "luxxle/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "testing/gmock/include/gmock/gmock.h"

using LuxxleThemeEventRouterBrowserTest = InProcessBrowserTest;

namespace extensions {

class MockLuxxleThemeEventRouter : public LuxxleThemeEventRouter {
 public:
  using LuxxleThemeEventRouter::LuxxleThemeEventRouter;
  ~MockLuxxleThemeEventRouter() override = default;

  MOCK_METHOD0(Notify, void());
};

}  // namespace extensions

IN_PROC_BROWSER_TEST_F(LuxxleThemeEventRouterBrowserTest,
                       ThemeChangeTest) {
  dark_mode::SetLuxxleDarkModeType(
      dark_mode::LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_DARK);

  extensions::MockLuxxleThemeEventRouter* mock_router =
      new extensions::MockLuxxleThemeEventRouter(browser()->profile());
  LuxxleThemeService* service = static_cast<LuxxleThemeService*>(
      ThemeServiceFactory::GetForProfile(browser()->profile()));
  service->SetLuxxleThemeEventRouterForTesting(mock_router);

  EXPECT_CALL(*mock_router, Notify()).Times(1);
  dark_mode::SetLuxxleDarkModeType(
      dark_mode::LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_LIGHT);

  EXPECT_CALL(*mock_router, Notify()).Times(1);
  dark_mode::SetLuxxleDarkModeType(
      dark_mode::LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_DARK);

  EXPECT_CALL(*mock_router, Notify()).Times(0);
  dark_mode::SetLuxxleDarkModeType(
      dark_mode::LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_DARK);
}
