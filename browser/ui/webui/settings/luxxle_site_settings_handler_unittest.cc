/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/webui/settings/luxxle_site_settings_handler.h"

#include <memory>
#include <string>

#include "base/values.h"
#include "luxxle/components/luxxle_shields/content/browser/luxxle_shields_util.h"
#include "chrome/browser/ui/webui/settings/site_settings_helper.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "components/grit/luxxle_components_strings.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_task_environment.h"
#include "content/public/test/test_web_ui.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "ui/base/l10n/l10n_util.h"

namespace settings {

namespace {

constexpr char kIsValidKey[] = "isValid";
constexpr char kReasonKey[] = "reason";

base::Value::Dict GetResponsePayload(bool valid, const std::string& message) {
  base::Value::Dict value;
  value.Set(kIsValidKey, base::Value(valid));
  value.Set(kReasonKey, base::Value(message));
  return value;
}
}  // namespace

class TestLuxxleSiteSettingsHandlerUnittest : public testing::Test {
 public:
  TestLuxxleSiteSettingsHandlerUnittest() {
    TestingBrowserProcess::GetGlobal()->CreateGlobalFeaturesForTesting();
    TestingProfile::Builder builder;

    profile_ = builder.Build();
    web_contents_ = content::WebContents::Create(
        content::WebContents::CreateParams(profile_.get()));

    test_web_ui_.set_web_contents(web_contents_.get());

    handler_ = std::make_unique<LuxxleSiteSettingsHandler>(profile_.get());
    handler_->set_web_ui(&test_web_ui_);
    handler_->RegisterMessages();
  }
  ~TestLuxxleSiteSettingsHandlerUnittest() override {
    // The test handler unusually owns its own TestWebUI, so we make sure to
    // unbind it from the base class before the derived class is destroyed.
    handler_->set_web_ui(nullptr);
    handler_.reset();
  }
  content::TestWebUI* web_ui() { return &test_web_ui_; }
  PrefService* prefs() { return profile_->GetPrefs(); }

  void HandleIsPatternValidForType(const base::Value::List& args) {
    handler_->HandleIsPatternValidForType(args);
  }
  bool IsPatternValidForLuxxleContentType(ContentSettingsType content_type,
                                         const std::string& pattern_string) {
    return handler_->IsPatternValidForLuxxleContentType(content_type,
                                                       pattern_string);
  }

 private:
  std::unique_ptr<LuxxleSiteSettingsHandler> handler_;
  content::BrowserTaskEnvironment browser_task_environment;
  std::unique_ptr<TestingProfile> profile_;
  std::unique_ptr<content::WebContents> web_contents_;
  content::TestWebUI test_web_ui_;
};

TEST_F(TestLuxxleSiteSettingsHandlerUnittest, InValidShieldsType) {
  base::Value::List args;
  args.Append(base::Value("id"));
  args.Append(base::Value("[*.]luxxle.com"));
  args.Append(base::Value(site_settings::ContentSettingsTypeToGroupName(
      ContentSettingsType::LUXXLE_SHIELDS)));
  HandleIsPatternValidForType(args);
  const auto& data = web_ui()->call_data()[0];
  ASSERT_TRUE(data->arg3()->is_dict());
  EXPECT_EQ(*data->arg3(),
            GetResponsePayload(
                false,
                l10n_util::GetStringUTF8(IDS_LUXXLE_SHIELDS_NOT_VALID_ADDRESS)));
}

TEST_F(TestLuxxleSiteSettingsHandlerUnittest, ValidShieldsType) {
  base::Value::List args;
  args.Append(base::Value("id"));
  args.Append(base::Value("luxxle.com"));
  args.Append(base::Value(site_settings::ContentSettingsTypeToGroupName(
      ContentSettingsType::LUXXLE_SHIELDS)));
  HandleIsPatternValidForType(args);
  const auto& data = web_ui()->call_data()[0];
  ASSERT_TRUE(data->arg3()->is_dict());
  EXPECT_EQ(*data->arg3(), GetResponsePayload(true, std::string()));
}

TEST_F(TestLuxxleSiteSettingsHandlerUnittest, ValidNonShieldsType) {
  base::Value::List args;
  args.Append(base::Value("id"));
  args.Append(base::Value("[*.]luxxle.com"));
  args.Append(base::Value(site_settings::ContentSettingsTypeToGroupName(
      ContentSettingsType::COOKIES)));
  HandleIsPatternValidForType(args);
  const auto& data = web_ui()->call_data()[0];
  ASSERT_TRUE(data->arg3()->is_dict());
  EXPECT_EQ(*data->arg3(), GetResponsePayload(true, std::string()));
}

TEST_F(TestLuxxleSiteSettingsHandlerUnittest,
       IsPatternValidForLuxxleContentType) {
  EXPECT_FALSE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "*.*"));
  EXPECT_FALSE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "*"));
  EXPECT_FALSE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "*://nytimes.com"));
  EXPECT_FALSE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "*.com"));
  EXPECT_FALSE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "luxxle[.*]"));
  EXPECT_FALSE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "[*.]nytimes.com"));
  EXPECT_TRUE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "nytimes.com"));
  EXPECT_TRUE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "www.nytimes.com"));
  EXPECT_TRUE(IsPatternValidForLuxxleContentType(
      ContentSettingsType::LUXXLE_SHIELDS, "https://www.nytimes.com"));
  EXPECT_TRUE(IsPatternValidForLuxxleContentType(ContentSettingsType::COOKIES,
                                                "[*.].nytimes.com"));
}
}  // namespace settings
