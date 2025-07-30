/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/net/search_ads_header_network_delegate_helper.h"

#include <memory>
#include <string>
#include <utility>

#include "base/test/scoped_feature_list.h"
#include "luxxle/browser/net/url_context.h"
// REMOVED: #include "luxxle/components/luxxle_ads/.*"
// REMOVED: #include "luxxle/components/luxxle_ads/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/l10n/common/test/scoped_default_locale.h"
#include "chrome/browser/prefs/browser_prefs.h"
#include "chrome/test/base/testing_profile.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/testing_pref_service.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/test/browser_task_environment.h"
#include "net/base/net_errors.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"

#if BUILDFLAG(IS_ANDROID)
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#endif  // BUILDFLAG(IS_ANDROID)

using testing::Return;

namespace {

constexpr char kLuxxleSearchRequestUrl[] =
    "https://search.luxxle.com/search?q=qwerty";
constexpr char kLuxxleSearchImageRequestUrl[] =
    "https://search.luxxle.com/img.png";
constexpr char kNonLuxxleSearchRequestUrl[] =
    "https://luxxle.com/search?q=qwerty";
constexpr char kLuxxleSearchTabUrl[] = "https://search.luxxle.com";
constexpr char kNonLuxxleSearchTabUrl[] = "https://luxxle.com";

}  // namespace

class SearchAdsHeaderDelegateHelperTest : public testing::Test {
 protected:
  void SetUp() override {
    scoped_feature_list_.InitWithFeatures(
        {
#if BUILDFLAG(IS_ANDROID)
            luxxle_rewards::features::kLuxxleRewards
#endif  // BUILDFLAG(IS_ANDROID)
        },
        {});

    TestingProfile::Builder builder;
    auto prefs =
        std::make_unique<sync_preferences::TestingPrefServiceSyncable>();
    RegisterUserProfilePrefs(prefs->registry());
    builder.SetPrefService(std::move(prefs));
    profile_ = builder.Build();
  }

  luxxle_l10n::test::ScopedDefaultLocale scoped_locale_{"en_US"};
  content::BrowserTaskEnvironment task_environment_;
  base::test::ScopedFeatureList scoped_feature_list_;
  std::unique_ptr<TestingProfile> profile_;
};

TEST_F(SearchAdsHeaderDelegateHelperTest, LuxxleSearchTabRewardsEnabled) {
  profile_->GetPrefs()->SetBoolean(luxxle_rewards::prefs::kEnabled, true);
  profile_->GetPrefs()->SetBoolean(luxxle_ads::prefs::kOptedInToSearchResultAds,
                                   false);

  auto request_info = std::make_shared<luxxle::LuxxleRequestInfo>();
  request_info->browser_context = profile_.get();
  request_info->tab_origin = GURL(kLuxxleSearchTabUrl);

  {
    request_info->request_url = GURL(kLuxxleSearchTabUrl);
    request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    auto header_value = headers.GetHeader(luxxle::kSearchAdsHeader);
    ASSERT_TRUE(header_value);
    EXPECT_EQ(*header_value, luxxle::kSearchAdsDisabledValue);
  }

  {
    request_info->request_url = GURL(kLuxxleSearchTabUrl);
    request_info->resource_type = blink::mojom::ResourceType::kXhr;

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    auto header_value = headers.GetHeader(luxxle::kSearchAdsHeader);
    ASSERT_TRUE(header_value);
    EXPECT_EQ(*header_value, luxxle::kSearchAdsDisabledValue);
  }

  {
    request_info->request_url = GURL(kLuxxleSearchImageRequestUrl);
    request_info->resource_type = blink::mojom::ResourceType::kImage;

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    auto header_value = headers.GetHeader(luxxle::kSearchAdsHeader);
    ASSERT_TRUE(header_value);
    EXPECT_EQ(*header_value, luxxle::kSearchAdsDisabledValue);
  }

  {
    request_info->tab_origin = GURL();
    request_info->initiator_url = GURL(kLuxxleSearchTabUrl);
    request_info->request_url = GURL(kLuxxleSearchTabUrl);
    request_info->resource_type = blink::mojom::ResourceType::kXhr;

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    auto header_value = headers.GetHeader(luxxle::kSearchAdsHeader);
    ASSERT_TRUE(header_value);
    EXPECT_EQ(*header_value, luxxle::kSearchAdsDisabledValue);
  }
}

TEST_F(SearchAdsHeaderDelegateHelperTest,
       NonLuxxleSearchTabSearchRewardsEnabled) {
  profile_->GetPrefs()->SetBoolean(luxxle_rewards::prefs::kEnabled, true);
  profile_->GetPrefs()->SetBoolean(luxxle_ads::prefs::kOptedInToSearchResultAds,
                                   false);

  auto request_info =
      std::make_shared<luxxle::LuxxleRequestInfo>(GURL(kLuxxleSearchRequestUrl));
  request_info->browser_context = profile_.get();
  request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

  {
    request_info->tab_origin = GURL(kNonLuxxleSearchTabUrl);
    request_info->initiator_url = GURL();

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
  }

  {
    request_info->tab_origin = GURL();
    request_info->initiator_url = GURL(kNonLuxxleSearchTabUrl);

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
  }
}

TEST_F(SearchAdsHeaderDelegateHelperTest,
       NonLuxxleSearchRequestSearchRewardsEnabled) {
  profile_->GetPrefs()->SetBoolean(luxxle_rewards::prefs::kEnabled, true);
  profile_->GetPrefs()->SetBoolean(luxxle_ads::prefs::kOptedInToSearchResultAds,
                                   false);

  auto request_info = std::make_shared<luxxle::LuxxleRequestInfo>(
      GURL(kNonLuxxleSearchRequestUrl));
  request_info->browser_context = profile_.get();
  request_info->tab_origin = GURL(kLuxxleSearchTabUrl);
  request_info->initiator_url = GURL(kLuxxleSearchTabUrl);
  request_info->resource_type = blink::mojom::ResourceType::kXhr;

  net::HttpRequestHeaders headers;
  const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
      &headers, luxxle::ResponseCallback(), request_info);
  EXPECT_EQ(result_code, net::OK);

  EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
}

TEST_F(SearchAdsHeaderDelegateHelperTest, LuxxleSearchHostRewardsDisabled) {
  profile_->GetPrefs()->SetBoolean(luxxle_rewards::prefs::kEnabled, false);

  auto request_info =
      std::make_shared<luxxle::LuxxleRequestInfo>(GURL(kLuxxleSearchRequestUrl));
  request_info->browser_context = profile_.get();
  request_info->tab_origin = GURL(kLuxxleSearchTabUrl);
  request_info->initiator_url = GURL(kLuxxleSearchTabUrl);

  {
    request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
  }

  {
    request_info->resource_type = blink::mojom::ResourceType::kXhr;

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
  }
}

TEST_F(SearchAdsHeaderDelegateHelperTest,
       LuxxleSearchHostSearchResultAdsEnabled) {
  profile_->GetPrefs()->SetBoolean(luxxle_rewards::prefs::kEnabled, true);
  profile_->GetPrefs()->SetBoolean(luxxle_ads::prefs::kOptedInToSearchResultAds,
                                   true);

  auto request_info =
      std::make_shared<luxxle::LuxxleRequestInfo>(GURL(kLuxxleSearchRequestUrl));
  request_info->browser_context = profile_.get();
  request_info->tab_origin = GURL(kLuxxleSearchTabUrl);
  request_info->initiator_url = GURL(kLuxxleSearchTabUrl);

  {
    request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
  }

  {
    request_info->resource_type = blink::mojom::ResourceType::kXhr;

    net::HttpRequestHeaders headers;
    const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
        &headers, luxxle::ResponseCallback(), request_info);
    EXPECT_EQ(result_code, net::OK);

    EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
  }
}

TEST_F(SearchAdsHeaderDelegateHelperTest, LuxxleSearchHostIncognitoProfile) {
  TestingProfile* incognito_profile =
      TestingProfile::Builder().BuildIncognito(profile_.get());

  auto request_info =
      std::make_shared<luxxle::LuxxleRequestInfo>(GURL(kLuxxleSearchRequestUrl));
  request_info->browser_context = incognito_profile;
  request_info->tab_origin = GURL(kLuxxleSearchTabUrl);
  request_info->initiator_url = GURL(kLuxxleSearchTabUrl);
  request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

  net::HttpRequestHeaders headers;
  const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
      &headers, luxxle::ResponseCallback(), request_info);
  EXPECT_EQ(result_code, net::OK);

  EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
}

TEST_F(SearchAdsHeaderDelegateHelperTest,
       LuxxleSearchHostIncognitoProfileWhenRewardsEnabledInMainProfile) {
  profile_->GetPrefs()->SetBoolean(luxxle_rewards::prefs::kEnabled, true);
  profile_->GetPrefs()->SetBoolean(luxxle_ads::prefs::kOptedInToSearchResultAds,
                                   false);

  TestingProfile* incognito_profile =
      TestingProfile::Builder().BuildIncognito(profile_.get());

  auto request_info =
      std::make_shared<luxxle::LuxxleRequestInfo>(GURL(kLuxxleSearchRequestUrl));
  request_info->browser_context = incognito_profile;
  request_info->tab_origin = GURL(kLuxxleSearchTabUrl);
  request_info->initiator_url = GURL(kLuxxleSearchTabUrl);
  request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

  net::HttpRequestHeaders headers;
  const int result_code = luxxle::OnBeforeStartTransaction_SearchAdsHeader(
      &headers, luxxle::ResponseCallback(), request_info);
  EXPECT_EQ(result_code, net::OK);

  EXPECT_FALSE(headers.HasHeader(luxxle::kSearchAdsHeader));
}
