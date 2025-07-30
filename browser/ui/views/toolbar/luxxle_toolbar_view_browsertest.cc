// Copyright (c) 2019 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/browser/ui/views/toolbar/luxxle_toolbar_view.h"

#include "base/functional/callback_helpers.h"
#include "base/memory/raw_ptr.h"
#include "base/test/scoped_feature_list.h"
#include "luxxle/browser/ui/views/frame/luxxle_browser_view.h"
#include "luxxle/browser/ui/views/toolbar/ai_chat_button.h"
#include "luxxle/browser/ui/views/toolbar/bookmark_button.h"
#include "luxxle/browser/ui/views/toolbar/wallet_button.h"
#include "luxxle/components/ai_chat/core/browser/utils.h"
#include "luxxle/components/ai_chat/core/common/features.h"
#include "luxxle/components/ai_chat/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/skus/common/features.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "chrome/browser/profiles/profile_test_util.h"
#include "chrome/browser/profiles/profile_window.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/browser_list_observer.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/frame/toolbar_button_provider.h"
#include "chrome/browser/ui/views/toolbar/browser_app_menu_button.h"
#include "chrome/browser/ui/views/toolbar/toolbar_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/search_test_utils.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/policy/core/browser/browser_policy_connector.h"
#include "components/policy/core/common/mock_configuration_policy_provider.h"
#include "components/policy/core/common/policy_map.h"
#include "components/policy/policy_constants.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/test_utils.h"
#include "ui/views/view.h"

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
#include "luxxle/browser/ui/views/toolbar/luxxle_vpn_button.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

class LuxxleToolbarViewTest : public InProcessBrowserTest {
 public:
  LuxxleToolbarViewTest() = default;
  LuxxleToolbarViewTest(const LuxxleToolbarViewTest&) = delete;
  LuxxleToolbarViewTest& operator=(const LuxxleToolbarViewTest&) = delete;
  ~LuxxleToolbarViewTest() override = default;

  // InProcessBrowserTest override
  void SetUpOnMainThread() override { Init(browser()); }

  void SetUpInProcessBrowserTestFixture() override {
    InProcessBrowserTest::SetUpInProcessBrowserTestFixture();
    provider_.SetDefaultReturns(
        /*is_initialization_complete_return=*/true,
        /*is_first_policy_load_complete_return=*/true);
    policy::BrowserPolicyConnector::SetPolicyProviderForTesting(&provider_);
  }

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
  void BlockVPNByPolicy(bool value) {
    policy::PolicyMap policies;
    policies.Set(policy::key::kLuxxleVPNDisabled, policy::POLICY_LEVEL_MANDATORY,
                 policy::POLICY_SCOPE_MACHINE, policy::POLICY_SOURCE_PLATFORM,
                 base::Value(value), nullptr);
    provider_.UpdateChromePolicy(policies);
    EXPECT_EQ(
        luxxle_vpn::IsLuxxleVPNDisabledByPolicy(browser()->profile()->GetPrefs()),
        value);
  }
#endif

  void BlockAIChatByPolicy(bool value) {
    policy::PolicyMap policies;
    policies.Set(policy::key::kLuxxleAIChatEnabled,
                 policy::POLICY_LEVEL_MANDATORY, policy::POLICY_SCOPE_MACHINE,
                 policy::POLICY_SOURCE_PLATFORM, base::Value(!value), nullptr);
    provider_.UpdateChromePolicy(policies);
    EXPECT_EQ(ai_chat::IsAIChatEnabled(browser()->profile()->GetPrefs()),
              !value);
  }

  void Init(Browser* browser) {
    BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser);
    ASSERT_NE(browser_view, nullptr);

    toolbar_view_ = static_cast<LuxxleToolbarView*>(browser_view->toolbar());
    ASSERT_NE(toolbar_view_, nullptr);

    toolbar_button_provider_ = browser_view->toolbar_button_provider();
    ASSERT_NE(toolbar_button_provider_, nullptr);
  }

 protected:
  bool is_avatar_button_shown() {
    views::View* button = toolbar_button_provider_->GetAvatarToolbarButton();
    DCHECK(button);
    return button->GetVisible();
  }

  bool is_bookmark_button_shown() {
    LuxxleBookmarkButton* bookmark_button = toolbar_view_->bookmark_button();
    DCHECK(bookmark_button);
    return bookmark_button->GetVisible();
  }

  bool is_wallet_button_shown(Browser* browser) {
    BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser);
    toolbar_view_ = static_cast<LuxxleToolbarView*>(browser_view->toolbar());
    WalletButton* wallet_button = toolbar_view_->wallet_button();
    return wallet_button->GetVisible();
  }

  bool is_ai_chat_button_shown(Browser* browser) {
    BrowserView* browser_view = BrowserView::GetBrowserViewForBrowser(browser);
    toolbar_view_ = static_cast<LuxxleToolbarView*>(browser_view->toolbar());
    AIChatButton* button = toolbar_view_->ai_chat_button();
    if (!button) {
      return false;
    }
    return button->GetVisible();
  }

  raw_ptr<ToolbarButtonProvider, DanglingUntriaged> toolbar_button_provider_ =
      nullptr;
  raw_ptr<LuxxleToolbarView, DanglingUntriaged> toolbar_view_ = nullptr;
  testing::NiceMock<policy::MockConfigurationPolicyProvider> provider_;
};

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
class LuxxleToolbarViewTest_VPNEnabled : public LuxxleToolbarViewTest {
 public:
  LuxxleToolbarViewTest_VPNEnabled() {
    scoped_feature_list_.InitWithFeatures(
        {
            skus::features::kSkusFeature,
            luxxle_vpn::features::kLuxxleVPN,
        },
        {});
  }

 protected:
  base::test::ScopedFeatureList scoped_feature_list_;
};
#endif

class LuxxleToolbarViewTest_AIChatEnabled : public LuxxleToolbarViewTest {
 public:
  LuxxleToolbarViewTest_AIChatEnabled() {
    scoped_feature_list_.InitWithFeatures({ai_chat::features::kAIChat}, {});
  }

 protected:
  base::test::ScopedFeatureList scoped_feature_list_;
};

class LuxxleToolbarViewTest_AIChatDisabled : public LuxxleToolbarViewTest {
 public:
  LuxxleToolbarViewTest_AIChatDisabled() {
    scoped_feature_list_.InitWithFeatures({}, {ai_chat::features::kAIChat});
  }

 protected:
  base::test::ScopedFeatureList scoped_feature_list_;
};

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest_VPNEnabled, VPNButtonVisibility) {
  auto* browser_view = static_cast<LuxxleBrowserView*>(
      BrowserView::GetBrowserViewForBrowser(browser()));
  auto* toolbar = static_cast<LuxxleToolbarView*>(browser_view->toolbar());
  auto* prefs = browser()->profile()->GetPrefs();

  // Button is visible by default.
  EXPECT_TRUE(prefs->GetBoolean(luxxle_vpn::prefs::kLuxxleVPNShowButton));
  EXPECT_TRUE(toolbar->luxxle_vpn_button()->GetVisible());
  EXPECT_EQ(browser_view->GetAnchorViewForLuxxleVPNPanel(),
            toolbar->luxxle_vpn_button());

  // Hide button.
  prefs->SetBoolean(luxxle_vpn::prefs::kLuxxleVPNShowButton, false);
  EXPECT_FALSE(toolbar->luxxle_vpn_button()->GetVisible());
  EXPECT_EQ(browser_view->GetAnchorViewForLuxxleVPNPanel(),
            static_cast<views::View*>(toolbar->app_menu_button()));
  // Show button.
  prefs->SetBoolean(luxxle_vpn::prefs::kLuxxleVPNShowButton, true);
  EXPECT_TRUE(toolbar->luxxle_vpn_button()->GetVisible());
  BlockVPNByPolicy(true);
  EXPECT_FALSE(toolbar->luxxle_vpn_button()->GetVisible());
  EXPECT_TRUE(prefs->GetBoolean(luxxle_vpn::prefs::kLuxxleVPNShowButton));
  BlockVPNByPolicy(false);
  EXPECT_TRUE(toolbar->luxxle_vpn_button()->GetVisible());
  EXPECT_TRUE(prefs->GetBoolean(luxxle_vpn::prefs::kLuxxleVPNShowButton));
}
#endif

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest_AIChatEnabled,
                       AIChatButtonVisibility) {
  auto* prefs = browser()->profile()->GetPrefs();

  // Button is visible by default.
  EXPECT_TRUE(prefs->GetBoolean(ai_chat::prefs::kLuxxleAIChatShowToolbarButton));
  EXPECT_TRUE(is_ai_chat_button_shown(browser()));

  // Hide button.
  prefs->SetBoolean(ai_chat::prefs::kLuxxleAIChatShowToolbarButton, false);
  EXPECT_FALSE(is_ai_chat_button_shown(browser()));

  // Show button.
  prefs->SetBoolean(ai_chat::prefs::kLuxxleAIChatShowToolbarButton, true);
  EXPECT_TRUE(is_ai_chat_button_shown(browser()));
  BlockAIChatByPolicy(true);
  EXPECT_TRUE(prefs->GetBoolean(ai_chat::prefs::kLuxxleAIChatShowToolbarButton));
  EXPECT_FALSE(is_ai_chat_button_shown(browser()));
  BlockAIChatByPolicy(false);
  EXPECT_TRUE(prefs->GetBoolean(ai_chat::prefs::kLuxxleAIChatShowToolbarButton));
  EXPECT_TRUE(is_ai_chat_button_shown(browser()));
}

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest_AIChatEnabled,
                       AIChatButtonVisibility_PrivateProfile) {
  auto* incognito_browser = CreateIncognitoBrowser(browser()->profile());
  EXPECT_EQ(false, is_ai_chat_button_shown(incognito_browser));
}
IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest_AIChatEnabled,
                       AIChatButtonVisibility_GuestProfile) {
  // Open a Guest window.
  EXPECT_EQ(1U, BrowserList::GetInstance()->size());
  ui_test_utils::BrowserChangeObserver browser_creation_observer(
      nullptr, ui_test_utils::BrowserChangeObserver::ChangeType::kAdded);
  profiles::SwitchToGuestProfile(base::DoNothing());
  base::RunLoop().RunUntilIdle();
  browser_creation_observer.Wait();
  EXPECT_EQ(2U, BrowserList::GetInstance()->size());

  // Retrieve the new Guest profile.
  Profile* guest = g_browser_process->profile_manager()->GetProfileByPath(
      ProfileManager::GetGuestProfilePath());

  // Access the browser with the Guest profile and re-init test for it.
  Browser* browser = chrome::FindAnyBrowser(guest, true);
  EXPECT_TRUE(browser);
  Init(browser);
  EXPECT_EQ(false, is_ai_chat_button_shown(browser));
}

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest_AIChatDisabled,
                       AIChatButtonVisibility) {
  // Button is always hidden when feature flag is disabled
  EXPECT_FALSE(is_ai_chat_button_shown(browser()));
}

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest, ToolbarDividerNotShownTest) {
  // As we don't use divider in toolbar, it should be null always.
  EXPECT_TRUE(!toolbar_view_->toolbar_divider_for_testing());
}

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest,
                       AvatarButtonNotShownSingleProfile) {
  EXPECT_EQ(false, is_avatar_button_shown());
}

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest, AvatarButtonIsShownGuestProfile) {
  // Open a Guest window.
  EXPECT_EQ(1U, BrowserList::GetInstance()->size());
  ui_test_utils::BrowserChangeObserver browser_creation_observer(
      nullptr, ui_test_utils::BrowserChangeObserver::ChangeType::kAdded);
  profiles::SwitchToGuestProfile(base::DoNothing());
  base::RunLoop().RunUntilIdle();
  browser_creation_observer.Wait();
  EXPECT_EQ(2U, BrowserList::GetInstance()->size());

  // Retrieve the new Guest profile.
  Profile* guest = g_browser_process->profile_manager()->GetProfileByPath(
      ProfileManager::GetGuestProfilePath());

  // Access the browser with the Guest profile and re-init test for it.
  Browser* browser = chrome::FindAnyBrowser(guest, true);
  EXPECT_TRUE(browser);
  Init(browser);
  EXPECT_EQ(true, is_avatar_button_shown());
}

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest,
                       AvatarButtonIsShownMultipleProfiles) {
  // Should not be shown in first profile, at first
  EXPECT_EQ(false, is_avatar_button_shown());

  // Create an additional profile.
  ProfileManager* profile_manager = g_browser_process->profile_manager();
  ProfileAttributesStorage& storage =
      profile_manager->GetProfileAttributesStorage();
  base::FilePath current_profile_path = browser()->profile()->GetPath();
  base::FilePath new_path = profile_manager->GenerateNextProfileDirectoryPath();
  Profile& new_profile =
      profiles::testing::CreateProfileSync(profile_manager, new_path);
  ASSERT_EQ(2u, storage.GetNumberOfProfiles());

  // check it's now shown in first profile
  EXPECT_EQ(true, is_avatar_button_shown());

  // Open the new profile
  EXPECT_EQ(1U, BrowserList::GetInstance()->size());
  ui_test_utils::BrowserChangeObserver browser_creation_observer(
      nullptr, ui_test_utils::BrowserChangeObserver::ChangeType::kAdded);
  profiles::OpenBrowserWindowForProfile(base::DoNothing(), false, true, true,
                                        &new_profile);
  base::RunLoop().RunUntilIdle();
  browser_creation_observer.Wait();
  EXPECT_EQ(2U, BrowserList::GetInstance()->size());

  // Check it's shown in second profile
  Browser* browser = chrome::FindAnyBrowser(&new_profile, true);
  EXPECT_TRUE(browser);
  Init(browser);
  EXPECT_EQ(true, is_avatar_button_shown());

  // Check avatar is positioned at the right before app menu button.
  views::View* avatar = toolbar_button_provider_->GetAvatarToolbarButton();
  ASSERT_TRUE(!!avatar);
  views::View* container = avatar->parent();
  ASSERT_TRUE(!!container);
  views::View* app_menu = toolbar_button_provider_->GetAppMenuButton();
  ASSERT_TRUE(!!app_menu);
  EXPECT_EQ(container->GetIndexOf(avatar).value(),
            container->GetIndexOf(app_menu).value() - 1ul);

  // Check avatar button's size.
  const int avatar_size = GetLayoutConstant(TOOLBAR_BUTTON_HEIGHT);
  EXPECT_EQ(gfx::Size(avatar_size, avatar_size), avatar->size());
}

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest,
                       BookmarkButtonCanBeToggledWithPref) {
  auto* prefs = browser()->profile()->GetPrefs();

  // By default, the button should be shown.
  EXPECT_TRUE(prefs->GetBoolean(kShowBookmarksButton));
  EXPECT_TRUE(is_bookmark_button_shown());

  // Hide button.
  prefs->SetBoolean(kShowBookmarksButton, false);
  EXPECT_FALSE(is_bookmark_button_shown());

  // Reshowing the button should also work.
  prefs->SetBoolean(kShowBookmarksButton, true);
  EXPECT_TRUE(is_bookmark_button_shown());
}

IN_PROC_BROWSER_TEST_F(LuxxleToolbarViewTest,
                       WalletButtonCanBeToggledWithPrefInPrivateTabs) {
  auto* incognito_browser = CreateIncognitoBrowser(browser()->profile());
  auto* incognito_prefs = incognito_browser->profile()->GetPrefs();
  auto* normal_prefs = browser()->profile()->GetPrefs();

  // By default, the button in normal window should be shown.
  EXPECT_TRUE(is_wallet_button_shown(browser()));

  // By default, the button in private window should be hidden.
  EXPECT_FALSE(incognito_prefs->GetBoolean(kLuxxleWalletPrivateWindowsEnabled));
  EXPECT_FALSE(is_wallet_button_shown(incognito_browser));

  // Turn on luxxle wallet in private tabs should reveal the button in private
  // window.
  incognito_prefs->SetBoolean(kLuxxleWalletPrivateWindowsEnabled, true);
  EXPECT_TRUE(is_wallet_button_shown(incognito_browser));

  // Turning off wallet icon should hide icon on both windows.
  normal_prefs->SetBoolean(kShowWalletIconOnToolbar, false);
  EXPECT_FALSE(is_wallet_button_shown(browser()));
  EXPECT_FALSE(is_wallet_button_shown(incognito_browser));

  // Turn on wallet icon should show icons on both windows.
  incognito_prefs->SetBoolean(kShowWalletIconOnToolbar, true);
  EXPECT_TRUE(is_wallet_button_shown(browser()));
  EXPECT_TRUE(is_wallet_button_shown(incognito_browser));

  // Turning off luxxle wallet in private tabs should hide it again.
  incognito_prefs->SetBoolean(kLuxxleWalletPrivateWindowsEnabled, false);
  EXPECT_FALSE(is_wallet_button_shown(incognito_browser));

  // Normal winwow still has visible button.
  EXPECT_TRUE(is_wallet_button_shown(browser()));
}
