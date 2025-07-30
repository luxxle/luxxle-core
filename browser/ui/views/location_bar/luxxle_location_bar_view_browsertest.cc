/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/location_bar/luxxle_location_bar_view.h"

#include <memory>

#include "base/run_loop.h"
#include "base/test/bind.h"
#include "base/test/scoped_feature_list.h"
#include "luxxle/browser/ui/views/location_bar/luxxle_search_conversion/promotion_button_controller.h"
#include "luxxle/browser/ui/views/location_bar/luxxle_search_conversion/promotion_button_view.h"
#include "luxxle/components/luxxle_search_conversion/features.h"
#include "luxxle/components/luxxle_search_conversion/pref_names.h"
#include "luxxle/components/search_engines/luxxle_prepopulated_engines.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/browser/ui/view_ids.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/omnibox/omnibox_view_views.h"
#include "chrome/browser/ui/views/toolbar/toolbar_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/interactive_test_utils.h"
#include "chrome/test/base/search_test_utils.h"
#include "components/omnibox/browser/omnibox_edit_model.h"
#include "components/omnibox/browser/omnibox_view.h"
#include "components/search_engines/template_url.h"
#include "components/search_engines/template_url_data_util.h"
#include "components/search_engines/template_url_service.h"
#include "content/public/browser/web_contents.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "testing/gtest/include/gtest/gtest.h"

class LuxxleLocationBarViewBrowserTest : public InProcessBrowserTest {
 public:
  LuxxleLocationBarViewBrowserTest() {
    // Disabled by default.
    EXPECT_FALSE(base::FeatureList::IsEnabled(
        luxxle_search_conversion::features::kOmniboxPromotionButton));

    features_.InitAndEnableFeature(
        luxxle_search_conversion::features::kOmniboxPromotionButton);
  }

  LuxxleLocationBarView* location_bar() {
    auto* browser_view = BrowserView::GetBrowserViewForBrowser(browser());
    return static_cast<LuxxleLocationBarView*>(
        browser_view->toolbar()->location_bar());
  }

  OmniboxViewViews* omnibox_view() { return location_bar()->omnibox_view(); }

  views::View* promotion_button_view() {
    return location_bar()->GetSearchPromotionButton();
  }

  TemplateURLService* GetTemplateURLService() {
    return TemplateURLServiceFactory::GetForProfile(browser()->profile());
  }

  content::WebContents* web_contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  void WaitUntil(base::RepeatingCallback<bool()> condition) {
    if (condition.Run()) {
      return;
    }

    base::RepeatingTimer scheduler;
    scheduler.Start(FROM_HERE, base::Milliseconds(100),
                    base::BindLambdaForTesting([this, &condition] {
                      if (condition.Run()) {
                        run_loop_->Quit();
                      }
                    }));
    Run();
  }

  void Run() {
    run_loop_ = std::make_unique<base::RunLoop>();
    run_loop_->Run();
  }

  base::test::ScopedFeatureList features_;
  std::unique_ptr<base::RunLoop> run_loop_;
};

IN_PROC_BROWSER_TEST_F(LuxxleLocationBarViewBrowserTest,
                       SearchConversionButtonTest) {
  search_test_utils::WaitForTemplateURLServiceToLoad(GetTemplateURLService());

  auto bing_search_data = TemplateURLDataFromPrepopulatedEngine(
      TemplateURLPrepopulateData::luxxle_bing);
  TemplateURL bing_template_url(*bing_search_data);

  auto luxxle_search_data = TemplateURLDataFromPrepopulatedEngine(
      TemplateURLPrepopulateData::luxxle_search);
  TemplateURL luxxle_template_url(*luxxle_search_data);

  // Set non luxxle search as a default provider and type any input.
  // Check promotion button is launched.
  GetTemplateURLService()->SetUserSelectedDefaultSearchProvider(
      &bing_template_url);
  location_bar()->FocusLocation(true);
  omnibox_view()->SetUserText(u"a");
  WaitUntil(base::BindLambdaForTesting(
      [&]() { return omnibox_view()->model()->PopupIsOpen(); }));
  EXPECT_TRUE(promotion_button_view()->GetVisible());

  // Unfocus from the omnibox.
  // Omnibox popup is hidden and promotion button will be gone also.
  web_contents()->Focus();
  WaitUntil(base::BindLambdaForTesting(
      [&]() { return !omnibox_view()->model()->PopupIsOpen(); }));
  EXPECT_FALSE(promotion_button_view()->GetVisible());

  // Set luxxle search as a default provider and type any input.
  GetTemplateURLService()->SetUserSelectedDefaultSearchProvider(
      &luxxle_template_url);

  // Check button is not shown with luxxle search.
  location_bar()->FocusLocation(true);
  omnibox_view()->SetUserText(u"a");
  WaitUntil(base::BindLambdaForTesting(
      [&]() { return omnibox_view()->model()->PopupIsOpen(); }));
  EXPECT_FALSE(promotion_button_view()->GetVisible());

  location_bar()->Revert();
  WaitUntil(base::BindLambdaForTesting(
      [&]() { return !omnibox_view()->model()->PopupIsOpen(); }));
  EXPECT_FALSE(promotion_button_view()->GetVisible());

  GetTemplateURLService()->SetUserSelectedDefaultSearchProvider(
      &bing_template_url);

  // Set dismissed and check button is not shown anymore.
  browser()->profile()->GetPrefs()->SetBoolean(
      luxxle_search_conversion::prefs::kDismissed, true);
  omnibox_view()->SetUserText(u"a");
  WaitUntil(base::BindLambdaForTesting(
      [&]() { return omnibox_view()->model()->PopupIsOpen(); }));
  EXPECT_FALSE(promotion_button_view()->GetVisible());

  location_bar()->Revert();
  WaitUntil(base::BindLambdaForTesting(
      [&]() { return !omnibox_view()->model()->PopupIsOpen(); }));
  EXPECT_FALSE(promotion_button_view()->GetVisible());

  constexpr std::u16string search_term = u"a";

  // Unset dismissed and simulate promotion button click.
  browser()->profile()->GetPrefs()->SetBoolean(
      luxxle_search_conversion::prefs::kDismissed, false);
  location_bar()->FocusLocation(true);
  omnibox_view()->SetUserText(search_term);
  WaitUntil(base::BindLambdaForTesting(
      [&]() { return omnibox_view()->model()->PopupIsOpen(); }));
  EXPECT_TRUE(promotion_button_view()->GetVisible());

  // Check luxxle search is set as default provider and luxxle search is loaded in
  // active tab.
  location_bar()
      ->promotion_controller_->SetDefaultAndLoadLuxxleSearchWithCurrentInput();
  const TemplateURL* default_provider =
      GetTemplateURLService()->GetDefaultSearchProvider();
  const auto target_search_url = luxxle_template_url.GenerateSearchURL(
      GetTemplateURLService()->search_terms_data(), search_term);
  EXPECT_EQ(luxxle_template_url.prepopulate_id(),
            default_provider->prepopulate_id());
  content::WaitForLoadStop(web_contents());
  EXPECT_EQ(target_search_url, web_contents()->GetVisibleURL());

  // Check dismissed bit is set after user clicks button.
  EXPECT_TRUE(browser()->profile()->GetPrefs()->GetBoolean(
      luxxle_search_conversion::prefs::kDismissed));
}
