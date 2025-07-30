/*  Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/memory/raw_ptr.h"
#include "luxxle/browser/luxxle_shields/luxxle_shields_tab_helper.h"
#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_util.h"
#include "luxxle/components/luxxle_shields/core/common/luxxle_shield_constants.h"
#include "luxxle/components/luxxle_shields/core/common/luxxle_shields_panel.mojom.h"
#include "luxxle/components/luxxle_shields/core/common/features.h"
#include "build/build_config.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/favicon/content/content_favicon_driver.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_task_environment.h"
#include "content/public/test/test_renderer_host.h"
#include "content/public/test/web_contents_tester.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using luxxle_shields::LuxxleShieldsTabHelper;
using luxxle_shields::ControlType;
using luxxle_shields::mojom::AdBlockMode;

namespace {
constexpr char kTestProfileName[] = "TestProfile";

class MockObserver : public LuxxleShieldsTabHelper::Observer {
 public:
  MOCK_METHOD(void, OnResourcesChanged, (), (override));
  MOCK_METHOD(void, OnShieldsEnabledChanged, (), (override));
};
}  // namespace

class LuxxleShieldsDataControllerTest : public testing::Test {
 public:
  LuxxleShieldsDataControllerTest() = default;
  ~LuxxleShieldsDataControllerTest() override = default;

  LuxxleShieldsDataControllerTest(const LuxxleShieldsDataControllerTest&) =
      delete;
  LuxxleShieldsDataControllerTest& operator=(
      const LuxxleShieldsDataControllerTest&) = delete;

  void SetUp() override {
    TestingBrowserProcess* browser_process = TestingBrowserProcess::GetGlobal();
    profile_manager_ = std::make_unique<TestingProfileManager>(browser_process);
    ASSERT_TRUE(profile_manager_->SetUp());
    profile_ = profile_manager_->CreateTestingProfile(kTestProfileName);

    test_web_contents_ =
        content::WebContentsTester::CreateTestWebContents(profile_, nullptr);
    favicon::ContentFaviconDriver::CreateForWebContents(
        test_web_contents_.get(), nullptr);
    LuxxleShieldsTabHelper::CreateForWebContents(test_web_contents_.get());
  }

  void TearDown() override {
    test_web_contents_.reset();
    profile_ = nullptr;
    profile_manager_->DeleteTestingProfile(kTestProfileName);
  }

  Profile* profile() { return profile_; }
  content::WebContents* web_contents() { return test_web_contents_.get(); }

  void SetLastCommittedUrl(const GURL& url) {
    content::WebContentsTester::For(web_contents())->SetLastCommittedURL(url);
  }

  LuxxleShieldsTabHelper* GetShieldsDataController() {
    return LuxxleShieldsTabHelper::FromWebContents(web_contents());
  }

  ContentSetting GetContentSettingFor(ContentSettingsType type,
                                      GURL secondary_url = GURL()) {
    auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
    auto* controller = GetShieldsDataController();

    return map->GetContentSetting(controller->GetCurrentSiteURL(),
                                  secondary_url, type);
  }

  void SetContentSettingFor(ContentSettingsType type,
                            ContentSetting setting,
                            GURL secondary_url = GURL()) {
    auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
    auto* controller = GetShieldsDataController();

    map->SetContentSettingCustomScope(
        luxxle_shields::GetPatternFromURL(controller->GetCurrentSiteURL()),
        luxxle_shields::GetPatternFromURL(secondary_url), type, setting);
  }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<content::WebContents> test_web_contents_;
  content::RenderViewHostTestEnabler render_view_host_test_enabler_;
  raw_ptr<Profile> profile_;
  std::unique_ptr<TestingProfileManager> profile_manager_;
};

TEST_F(LuxxleShieldsDataControllerTest, SetAdBlockMode_ForOrigin_1) {
  auto* controller = GetShieldsDataController();
  SetLastCommittedUrl(GURL("http://luxxle.com"));

  /* DEFAULT */
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_DEFAULT);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_DEFAULT);

  /* ALLOW */
  controller->SetAdBlockMode(AdBlockMode::ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);

  /* STANDARD */
  controller->SetAdBlockMode(AdBlockMode::STANDARD);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);

  /* ALLOW */
  controller->SetAdBlockMode(AdBlockMode::ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);

  /* AGGRESSIVE */
  controller->SetAdBlockMode(AdBlockMode::AGGRESSIVE);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_BLOCK);

  /* ALLOW */
  controller->SetAdBlockMode(AdBlockMode::ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);
}

TEST_F(LuxxleShieldsDataControllerTest, SetAdBlockMode_ForOrigin_2) {
  auto* controller = GetShieldsDataController();
  SetLastCommittedUrl(GURL("http://luxxle.com"));

  /* DEFAULT */
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_DEFAULT);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_DEFAULT);

  /* STANDARD */
  controller->SetAdBlockMode(AdBlockMode::STANDARD);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);

  /* ALLOW */
  controller->SetAdBlockMode(AdBlockMode::ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);

  /* STANDARD */
  controller->SetAdBlockMode(AdBlockMode::STANDARD);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);

  /* AGGRESSIVE */
  controller->SetAdBlockMode(AdBlockMode::AGGRESSIVE);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_BLOCK);

  /* STANDARD */
  controller->SetAdBlockMode(AdBlockMode::STANDARD);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);
}

TEST_F(LuxxleShieldsDataControllerTest, SetAdBlockMode_ForOrigin_3) {
  auto* controller = GetShieldsDataController();
  SetLastCommittedUrl(GURL("http://luxxle.com"));

  /* DEFAULT */
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_DEFAULT);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_DEFAULT);

  /* AGGRESSIVE */
  controller->SetAdBlockMode(AdBlockMode::AGGRESSIVE);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_BLOCK);

  /* ALLOW */
  controller->SetAdBlockMode(AdBlockMode::ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_ALLOW);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);

  /* AGGRESSIVE */
  controller->SetAdBlockMode(AdBlockMode::AGGRESSIVE);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_BLOCK);

  /* STANDARD */
  controller->SetAdBlockMode(AdBlockMode::STANDARD);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_ALLOW);

  /* AGGRESSIVE */
  controller->SetAdBlockMode(AdBlockMode::AGGRESSIVE);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_ADS),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING),
            CONTENT_SETTING_BLOCK);
  EXPECT_EQ(GetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                                 GURL("https://firstParty/")),
            CONTENT_SETTING_BLOCK);
}

TEST_F(LuxxleShieldsDataControllerTest, GetAdBlockMode_ForOrigin) {
  auto* controller = GetShieldsDataController();
  SetLastCommittedUrl(GURL("http://luxxle.com"));

  /* DEFAULT */
  EXPECT_EQ(controller->GetAdBlockMode(), AdBlockMode::STANDARD);

  /* ALLOW */
  SetContentSettingFor(ContentSettingsType::LUXXLE_ADS, CONTENT_SETTING_ALLOW);
  EXPECT_EQ(controller->GetAdBlockMode(), AdBlockMode::ALLOW);

  /* STANDARD */
  SetContentSettingFor(ContentSettingsType::LUXXLE_ADS, CONTENT_SETTING_BLOCK);
  SetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                       CONTENT_SETTING_BLOCK);
  SetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                       CONTENT_SETTING_ALLOW, GURL("https://firstParty/"));
  EXPECT_EQ(controller->GetAdBlockMode(), AdBlockMode::STANDARD);

  /* AGGRESSIVE */
  SetContentSettingFor(ContentSettingsType::LUXXLE_ADS, CONTENT_SETTING_BLOCK);
  SetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                       CONTENT_SETTING_BLOCK);
  SetContentSettingFor(ContentSettingsType::LUXXLE_COSMETIC_FILTERING,
                       CONTENT_SETTING_BLOCK, GURL("https://firstParty/"));
  EXPECT_EQ(controller->GetAdBlockMode(), AdBlockMode::AGGRESSIVE);
}

TEST_F(LuxxleShieldsDataControllerTest, Observer_OnShieldsEnabledChangedTest) {
  // Set url for default web contents.
  SetLastCommittedUrl(GURL("http://luxxle.com"));

  // Create another web contents for testing whether its
  // OnShieldsEnabledChanged() callback is called when shields enabled is
  // changed by another web contents when both loaded same url.
  MockObserver observer_2;
  EXPECT_CALL(observer_2, OnShieldsEnabledChanged).Times(1);
  auto web_contents_2 =
      content::WebContentsTester::CreateTestWebContents(profile(), nullptr);
  favicon::ContentFaviconDriver::CreateForWebContents(web_contents_2.get(),
                                                      nullptr);
  LuxxleShieldsTabHelper::CreateForWebContents(web_contents_2.get());
  auto* ctrl_2 = LuxxleShieldsTabHelper::FromWebContents(web_contents_2.get());
  ctrl_2->AddObserver(&observer_2);
  content::WebContentsTester::For(web_contents_2.get())
      ->SetLastCommittedURL(GURL("http://luxxle.com"));

  // Create another web contents for testing whether its
  // OnShieldsEnabledChanged() callback is *not* called when shields enabled is
  // changed by another web contents when both loaded *different* url.
  MockObserver observer_3;
  EXPECT_CALL(observer_3, OnShieldsEnabledChanged).Times(0);
  auto web_contents_3 =
      content::WebContentsTester::CreateTestWebContents(profile(), nullptr);
  favicon::ContentFaviconDriver::CreateForWebContents(web_contents_3.get(),
                                                      nullptr);
  LuxxleShieldsTabHelper::CreateForWebContents(web_contents_3.get());
  auto* ctrl_3 = LuxxleShieldsTabHelper::FromWebContents(web_contents_3.get());
  ctrl_3->AddObserver(&observer_3);
  content::WebContentsTester::For(web_contents_3.get())
      ->SetLastCommittedURL(GURL("http://github.com"));

  // Change default web contents' shields enabled setting.
  // And this changes will affect |web_contents_2| as both loaded same url.
  GetShieldsDataController()->SetLuxxleShieldsEnabled(false);

  ctrl_2->RemoveObserver(&observer_2);
  ctrl_3->RemoveObserver(&observer_3);
}

TEST_F(LuxxleShieldsDataControllerTest, SetLuxxleShieldsEnabledAsDefaultValue) {
  // Set url for default web contents.
  SetLastCommittedUrl(GURL("http://luxxle.com"));
  auto* map = HostContentSettingsMapFactory::GetForProfile(profile());
  EXPECT_EQ(map->GetDefaultContentSetting(ContentSettingsType::LUXXLE_SHIELDS,
                                          nullptr),
            CONTENT_SETTING_ALLOW);

  EXPECT_TRUE(GetShieldsDataController()->GetLuxxleShieldsEnabled());
  GetShieldsDataController()->SetLuxxleShieldsEnabled(false);
  EXPECT_FALSE(GetShieldsDataController()->GetLuxxleShieldsEnabled());
  EXPECT_FALSE(profile()
                   ->GetPrefs()
                   ->GetDict("profile.content_settings.exceptions.luxxleShields")
                   .empty());

  GetShieldsDataController()->SetLuxxleShieldsEnabled(true);
  EXPECT_TRUE(GetShieldsDataController()->GetLuxxleShieldsEnabled());
  EXPECT_TRUE(profile()
                  ->GetPrefs()
                  ->GetDict("profile.content_settings.exceptions.luxxleShields")
                  .empty());
}

TEST_F(LuxxleShieldsDataControllerTest, AllowedOnceScripts) {
  EXPECT_EQ(GetShieldsDataController()->GetAllowedJsList().size(), 0u);
  GetShieldsDataController()->HandleItemAllowedOnce(
      luxxle_shields::kJavaScript, "https://url1.com/script.js");
  EXPECT_EQ(GetShieldsDataController()->GetAllowedJsList().size(), 1u);
  GetShieldsDataController()->HandleItemAllowedOnce(
      luxxle_shields::kJavaScript, "https://url2.com/script.js");
  EXPECT_EQ(GetShieldsDataController()->GetAllowedJsList().size(), 2u);
  GetShieldsDataController()->HandleItemAllowedOnce(
      luxxle_shields::kJavaScript, "https://url3.com/script.js");
  EXPECT_EQ(GetShieldsDataController()->GetAllowedJsList().size(), 3u);

  // Making sure we exclude duplicates
  GetShieldsDataController()->HandleItemAllowedOnce(
      luxxle_shields::kJavaScript, "https://url2.com/script.js");
  GetShieldsDataController()->HandleItemAllowedOnce(
      luxxle_shields::kJavaScript, "https://url3.com/script.js");
  EXPECT_EQ(GetShieldsDataController()->GetAllowedJsList().size(), 3u);

  GetShieldsDataController()->ClearAllResourcesList();
  EXPECT_EQ(GetShieldsDataController()->GetAllowedJsList().size(), 0u);
}
