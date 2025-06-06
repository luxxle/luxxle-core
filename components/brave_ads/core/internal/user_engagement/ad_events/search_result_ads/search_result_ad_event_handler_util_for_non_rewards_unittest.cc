/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/run_loop.h"
#include "base/test/gmock_callback_support.h"
#include "base/test/mock_callback.h"
#include "brave/components/brave_ads/core/internal/ad_units/search_result_ad/search_result_ad_builder.h"
#include "brave/components/brave_ads/core/internal/ad_units/search_result_ad/search_result_ad_info.h"
#include "brave/components/brave_ads/core/internal/common/test/test_base.h"
#include "brave/components/brave_ads/core/internal/common/test/time_test_util.h"
#include "brave/components/brave_ads/core/internal/creatives/conversions/creative_set_conversion_builder.h"
#include "brave/components/brave_ads/core/internal/creatives/conversions/creative_set_conversion_database_table.h"
#include "brave/components/brave_ads/core/internal/creatives/conversions/creative_set_conversion_info.h"
#include "brave/components/brave_ads/core/internal/creatives/search_result_ads/creative_search_result_ad_test_util.h"
#include "brave/components/brave_ads/core/internal/settings/settings_test_util.h"
#include "brave/components/brave_ads/core/internal/user_engagement/ad_events/ad_event_builder.h"
#include "brave/components/brave_ads/core/internal/user_engagement/ad_events/search_result_ads/search_result_ad_event_handler_util.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

class BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest
    : public test::TestBase {
 protected:
  void SetUp() override {
    test::TestBase::SetUp();

    test::DisableBraveRewards();
  }

  database::table::CreativeSetConversions
      creative_set_conversions_database_table_;
};

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       SaveCreativeSetConversionForClickedEventWithConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);

  // Act
  MaybeBuildAndSaveCreativeSetConversion(
      mojom_creative_ad, mojom::SearchResultAdEventType::kClicked);

  // Assert
  std::optional<CreativeSetConversionInfo> creative_set_conversion =
      FromMojomMaybeBuildCreativeSetConversion(mojom_creative_ad);
  ASSERT_TRUE(creative_set_conversion);

  base::MockCallback<database::table::GetCreativeSetConversionsCallback>
      callback;
  base::RunLoop run_loop;
  EXPECT_CALL(callback,
              Run(/*success=*/true,
                  CreativeSetConversionList{*creative_set_conversion}))
      .WillOnce(base::test::RunOnceClosure(run_loop.QuitClosure()));
  creative_set_conversions_database_table_.GetUnexpired(callback.Get());
  run_loop.Run();
}

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       DoNotSaveCreativeSetConversionForNonClickedEventWithConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);

  // Act
  for (int i = 0;
       i < static_cast<int>(mojom::SearchResultAdEventType::kMaxValue); ++i) {
    const auto mojom_ad_event_type =
        static_cast<mojom::SearchResultAdEventType>(i);
    if (mojom_ad_event_type != mojom::SearchResultAdEventType::kClicked) {
      MaybeBuildAndSaveCreativeSetConversion(mojom_creative_ad,
                                             mojom_ad_event_type);
    }
  }

  // Assert
  base::MockCallback<database::table::GetCreativeSetConversionsCallback>
      callback;
  base::RunLoop run_loop;
  EXPECT_CALL(callback, Run(/*success=*/true, ::testing::IsEmpty()))
      .WillOnce(base::test::RunOnceClosure(run_loop.QuitClosure()));
  creative_set_conversions_database_table_.GetUnexpired(callback.Get());
  run_loop.Run();
}

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       DoNotSaveCreativeSetConversionForClickedEventWithoutConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAd(/*should_generate_random_uuids=*/true);

  // Act
  MaybeBuildAndSaveCreativeSetConversion(
      mojom_creative_ad, mojom::SearchResultAdEventType::kClicked);

  // Assert
  base::MockCallback<database::table::GetCreativeSetConversionsCallback>
      callback;
  base::RunLoop run_loop;
  EXPECT_CALL(callback, Run(/*success=*/true, ::testing::IsEmpty()))
      .WillOnce(base::test::RunOnceClosure(run_loop.QuitClosure()));
  creative_set_conversions_database_table_.GetUnexpired(callback.Get());
  run_loop.Run();
}

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       NotAllowedToFireViewedEventWithConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);

  // Act & Assert
  EXPECT_FALSE(IsAllowedToFireAdEvent(
      mojom_creative_ad, mojom::SearchResultAdEventType::kViewedImpression));
}

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       NotAllowedToFireViewedEventWithoutConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAd(/*should_generate_random_uuids=*/true);

  // Act & Assert
  EXPECT_FALSE(IsAllowedToFireAdEvent(
      mojom_creative_ad, mojom::SearchResultAdEventType::kViewedImpression));
}

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       NotAllowedToFireNonClickedEventWithConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);

  // Act & Assert
  for (int i = 0;
       i < static_cast<int>(mojom::SearchResultAdEventType::kMaxValue); ++i) {
    const auto mojom_ad_event_type =
        static_cast<mojom::SearchResultAdEventType>(i);
    if (mojom_ad_event_type != mojom::SearchResultAdEventType::kClicked) {
      EXPECT_FALSE(
          IsAllowedToFireAdEvent(mojom_creative_ad, mojom_ad_event_type));
    }
  }
}

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       NotAllowedToFireClickedEventWithoutConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAd(/*should_generate_random_uuids=*/true);

  // Act & Assert
  EXPECT_FALSE(IsAllowedToFireAdEvent(
      mojom_creative_ad, mojom::SearchResultAdEventType::kClicked));
}

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       ShouldFireNonDuplicateClickedEvent) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  AdEventList ad_events;
  const AdEventInfo ad_event =
      BuildAdEvent(ad, mojom::ConfirmationType::kServedImpression,
                   /*created_at=*/test::Now());
  ad_events.push_back(ad_event);

  // Act & Assert
  EXPECT_TRUE(ShouldFireAdEvent(ad, ad_events,
                                mojom::SearchResultAdEventType::kClicked));
}

TEST_F(BraveAdsSearchResultAdEventHandlerUtilForNonRewardsTest,
       ShouldNotFireDuplicateClickedEvent) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  AdEventList ad_events;
  const AdEventInfo ad_event_1 =
      BuildAdEvent(ad, mojom::ConfirmationType::kServedImpression,
                   /*created_at=*/test::Now());
  ad_events.push_back(ad_event_1);
  const AdEventInfo ad_event_2 = BuildAdEvent(
      ad, mojom::ConfirmationType::kClicked, /*created_at=*/test::Now());
  ad_events.push_back(ad_event_2);

  // Act & Assert
  EXPECT_FALSE(ShouldFireAdEvent(ad, ad_events,
                                 mojom::SearchResultAdEventType::kClicked));
}

}  // namespace brave_ads
