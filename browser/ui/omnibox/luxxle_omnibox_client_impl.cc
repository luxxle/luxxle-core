/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/omnibox/luxxle_omnibox_client_impl.h"

#include <string>

#include "base/check_is_test.h"
#include "luxxle/browser/autocomplete/luxxle_autocomplete_scheme_classifier.h"
#include "luxxle/browser/misc_metrics/profile_misc_metrics_service.h"
#include "luxxle/browser/misc_metrics/profile_misc_metrics_service_factory.h"
#include "luxxle/browser/search_engines/search_engine_tracker.h"
#include "luxxle/components/ai_chat/core/browser/ai_chat_metrics.h"
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/luxxle_search_conversion/p3a.h"
#include "luxxle/components/luxxle_search_conversion/utils.h"
#include "luxxle/components/omnibox/browser/luxxle_omnibox_prefs.h"
#include "luxxle/components/omnibox/browser/promotion_utils.h"
#include "luxxle/components/p3a_utils/bucket.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/omnibox/chrome_omnibox_client.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/omnibox/browser/autocomplete_result.h"
#include "components/omnibox/browser/omnibox_log.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"

namespace {

using luxxle_search_conversion::ConversionType;
using luxxle_search_conversion::GetConversionType;

constexpr char kSearchCountPrefName[] = "luxxle.weekly_storage.search_count";
constexpr char kSearchCountNonRewardsHistogramName[] =
    "Luxxle.Omnibox.SearchCount.NonRewards";
constexpr char kSearchCountRewardsHistogramName[] =
    "Luxxle.Omnibox.SearchCount.Rewards";
constexpr char kSearchCountRewardsWalletHistogramName[] =
    "Luxxle.Omnibox.SearchCount.RewardsWallet";
constexpr const char* kAllSearchCountHistogramNames[] = {
    kSearchCountNonRewardsHistogramName,
    kSearchCountRewardsHistogramName,
    kSearchCountRewardsWalletHistogramName,
};
constexpr int kSearchCountBuckets[] = {0, 5, 10, 20, 50, 100, 500};

bool IsSearchEvent(const AutocompleteMatch& match) {
  switch (match.type) {
    case AutocompleteMatchType::SEARCH_WHAT_YOU_TYPED:
    case AutocompleteMatchType::SEARCH_HISTORY:
    case AutocompleteMatchType::SEARCH_SUGGEST:
    case AutocompleteMatchType::SEARCH_SUGGEST_ENTITY:
    case AutocompleteMatchType::SEARCH_SUGGEST_TAIL:
    case AutocompleteMatchType::SEARCH_SUGGEST_PERSONALIZED:
    case AutocompleteMatchType::SEARCH_SUGGEST_PROFILE:
    case AutocompleteMatchType::SEARCH_OTHER_ENGINE:
      return true;
    default:
      return false;
  }
}

}  // namespace

LuxxleOmniboxClientImpl::LuxxleOmniboxClientImpl(LocationBar* location_bar,
                                               Browser* browser,
                                               Profile* profile)
    : ChromeOmniboxClient(location_bar, browser, profile),
      profile_(profile),
      search_engine_tracker_(
          SearchEngineTrackerFactory::GetForBrowserContext(profile)),
      scheme_classifier_(profile),
      search_storage_(profile_->GetPrefs(), kSearchCountPrefName) {
  // Record initial search count p3a value.
  RecordSearchEventP3A();

  auto* profile_metrics =
      misc_metrics::ProfileMiscMetricsServiceFactory::GetServiceForContext(
          profile);
  if (profile_metrics) {
    ai_chat_metrics_ = profile_metrics->GetAIChatMetrics();
    CHECK(ai_chat_metrics_);
  }

  pref_change_registrar_.Init(profile_->GetPrefs());
  pref_change_registrar_.Add(
      luxxle_rewards::prefs::kEnabled,
      base::BindRepeating(&LuxxleOmniboxClientImpl::RecordSearchEventP3A,
                          base::Unretained(this)));
  pref_change_registrar_.Add(
      luxxle_rewards::prefs::kExternalWalletType,
      base::BindRepeating(&LuxxleOmniboxClientImpl::RecordSearchEventP3A,
                          base::Unretained(this)));
}

LuxxleOmniboxClientImpl::~LuxxleOmniboxClientImpl() = default;

void LuxxleOmniboxClientImpl::RegisterProfilePrefs(
    PrefRegistrySimple* registry) {
  registry->RegisterListPref(kSearchCountPrefName);
}

const AutocompleteSchemeClassifier&
LuxxleOmniboxClientImpl::GetSchemeClassifier() const {
  return scheme_classifier_;
}

void LuxxleOmniboxClientImpl::OnURLOpenedFromOmnibox(OmniboxLog* log) {
  if (log->selection.line <= 0) {
    return;
  }
  const auto match = log->result->match_at(log->selection.line);
  if (IsLuxxleSearchPromotionMatch(match)) {
    luxxle_search_conversion::p3a::RecordPromoTrigger(
        g_browser_process->local_state(), GetConversionTypeFromMatch(match));
  }
}

void LuxxleOmniboxClientImpl::OnAutocompleteAccept(
    const GURL& destination_url,
    TemplateURLRef::PostContent* post_content,
    WindowOpenDisposition disposition,
    ui::PageTransition transition,
    AutocompleteMatchType::Type match_type,
    base::TimeTicks match_selection_timestamp,
    bool destination_url_entered_without_scheme,
    bool destination_url_entered_with_http_scheme,
    const std::u16string& text,
    const AutocompleteMatch& match,
    const AutocompleteMatch& alternative_nav_match) {
  if (IsSearchEvent(match)) {
    // TODO(iefremov): Optimize this.
    search_storage_.AddDelta(1);
    RecordSearchEventP3A();
    if (search_engine_tracker_ != nullptr) {
      search_engine_tracker_->RecordLocationBarQuery();
    }
    if (ai_chat_metrics_) {
      ai_chat_metrics_->RecordOmniboxSearchQuery();
    }
  }
  ChromeOmniboxClient::OnAutocompleteAccept(
      destination_url, post_content, disposition, transition, match_type,
      match_selection_timestamp, destination_url_entered_without_scheme,
      destination_url_entered_with_http_scheme, text, match,
      alternative_nav_match);
}

void LuxxleOmniboxClientImpl::RecordSearchEventP3A() {
  const char* report_histogram_name = nullptr;
  auto number_of_searches = search_storage_.GetWeeklySum();

  if (profile_->GetPrefs()->GetBoolean(luxxle_rewards::prefs::kEnabled)) {
    const std::string wallet_type = profile_->GetPrefs()->GetString(
        luxxle_rewards::prefs::kExternalWalletType);
    if (wallet_type.empty()) {
      report_histogram_name = kSearchCountRewardsHistogramName;
    } else {
      report_histogram_name = kSearchCountRewardsWalletHistogramName;
    }
  } else {
    report_histogram_name = kSearchCountNonRewardsHistogramName;
  }

  for (const auto* histogram_name : kAllSearchCountHistogramNames) {
    if (report_histogram_name == histogram_name) {
      p3a_utils::RecordToHistogramBucket(histogram_name, kSearchCountBuckets,
                                         number_of_searches);
    } else {
      base::UmaHistogramExactLinear(histogram_name, INT_MAX - 1, 8);
    }
  }
}
