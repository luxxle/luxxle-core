/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <optional>

#include "base/test/scoped_feature_list.h"
#include "luxxle/app/luxxle_command_ids.h"
#include "luxxle/browser/themes/luxxle_dark_mode_utils.h"
#include "luxxle/browser/ui/luxxle_browser_command_controller.h"
#include "luxxle/browser/ui/views/toolbar/luxxle_browser_app_menu_button.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/skus/common/features.h"
#include "chrome/browser/ui/toolbar/app_menu_icon_controller.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/toolbar/app_menu.h"
#include "chrome/browser/ui/views/toolbar/browser_app_menu_button.h"
#include "chrome/browser/ui/views/toolbar/toolbar_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/test_utils.h"
#include "ui/base/ui_base_features.h"
#include "ui/views/controls/button/toggle_button.h"
#include "ui/views/controls/menu/menu_item_view.h"
#include "ui/views/controls/menu/menu_runner.h"
#include "ui/views/view_utils.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/browser/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

class LuxxleAppMenuBrowserTest : public InProcessBrowserTest {
 public:
  LuxxleAppMenuBrowserTest() {
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
    scoped_feature_list_.InitWithFeatures(
        {skus::features::kSkusFeature, luxxle_vpn::features::kLuxxleVPN}, {});
#endif
  }

  ~LuxxleAppMenuBrowserTest() override = default;

  BrowserAppMenuButton* menu_button() {
    return BrowserView::GetBrowserViewForBrowser(browser())
        ->toolbar()
        ->app_menu_button();
  }

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  void SetPurchasedUserForLuxxleVPN(Browser* browser, bool purchased) {
    auto* service =
        luxxle_vpn::LuxxleVpnServiceFactory::GetForProfile(browser->profile());
    ASSERT_TRUE(!!service);
    auto target_state = purchased
                            ? luxxle_vpn::mojom::PurchasedState::PURCHASED
                            : luxxle_vpn::mojom::PurchasedState::NOT_PURCHASED;
    service->SetPurchasedState(skus::GetDefaultEnvironment(), target_state);
    // Call explicitely to update vpn commands status because mojo works in
    // async way.
    static_cast<chrome::LuxxleBrowserCommandController*>(
        browser->command_controller())
        ->OnPurchasedStateChanged(target_state, std::nullopt);
  }

  base::test::ScopedFeatureList scoped_feature_list_;
#endif
};

IN_PROC_BROWSER_TEST_F(LuxxleAppMenuBrowserTest, AppMenuButtonUpgradeAlertTest) {
  // Check property for our style.
  auto* luxxle_menu_button =
      static_cast<LuxxleBrowserAppMenuButton*>(menu_button());
  EXPECT_TRUE(luxxle_menu_button->ShouldPaintBorder());
  EXPECT_TRUE(luxxle_menu_button->ShouldBlendHighlightColor());

  // Check our highlight color.
  dark_mode::SetLuxxleDarkModeType(
      dark_mode::LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_LIGHT);
  EXPECT_EQ(dark_mode::LuxxleDarkModeType::LUXXLE_DARK_MODE_TYPE_LIGHT,
            dark_mode::GetActiveLuxxleDarkModeType());
  EXPECT_EQ(luxxle_menu_button->GetHighlightColor(), std::nullopt);
  luxxle_menu_button->SetTypeAndSeverity(
      {AppMenuIconController::IconType::UPGRADE_NOTIFICATION,
       AppMenuIconController::Severity::LOW});
  EXPECT_EQ(luxxle_menu_button->GetHighlightColor(),
            SkColorSetRGB(0x00, 0x46, 0x07));
  luxxle_menu_button->SetTypeAndSeverity(
      {AppMenuIconController::IconType::UPGRADE_NOTIFICATION,
       AppMenuIconController::Severity::MEDIUM});
  EXPECT_EQ(luxxle_menu_button->GetHighlightColor(),
            SkColorSetRGB(0x4A, 0x39, 0x00));
  luxxle_menu_button->SetTypeAndSeverity(
      {AppMenuIconController::IconType::UPGRADE_NOTIFICATION,
       AppMenuIconController::Severity::HIGH});
  EXPECT_EQ(luxxle_menu_button->GetHighlightColor(),
            SkColorSetRGB(0x7D, 0x00, 0x1A));

  EXPECT_EQ(GetLayoutConstant(TOOLBAR_BUTTON_HEIGHT),
            luxxle_menu_button->height());
  EXPECT_EQ(::GetLayoutInsets(TOOLBAR_BUTTON), luxxle_menu_button->GetInsets());
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// Check toggle menu item has additional toggle button for purchased user.
IN_PROC_BROWSER_TEST_F(LuxxleAppMenuBrowserTest, PurchasedVPN) {
  SetPurchasedUserForLuxxleVPN(browser(), true);
  menu_button()->ShowMenu(views::MenuRunner::NO_FLAGS);
  views::MenuItemView* menu_root = menu_button()->app_menu()->root_menu_item();
  auto* toggle_menu_item = menu_root->GetMenuItemByID(IDC_TOGGLE_LUXXLE_VPN);
  ASSERT_TRUE(!!toggle_menu_item);
  const int last_item_index = toggle_menu_item->children().size() - 1;
  auto* toggle_button = views::AsViewClass<views::ToggleButton>(
      toggle_menu_item->children()[last_item_index]);
  ASSERT_NE(nullptr, toggle_button);
}

// Check app menu has show vpn panel menu item for not purchased user.
IN_PROC_BROWSER_TEST_F(LuxxleAppMenuBrowserTest, NotPurchasedVPN) {
  SetPurchasedUserForLuxxleVPN(browser(), false);
  menu_button()->ShowMenu(views::MenuRunner::NO_FLAGS);
  views::MenuItemView* menu_root = menu_button()->app_menu()->root_menu_item();
  EXPECT_TRUE(!!menu_root->GetMenuItemByID(IDC_SHOW_LUXXLE_VPN_PANEL));
}
#endif
