/* Copyright (c) 2025 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <string>
#include <string_view>
#include <utility>

#include "base/memory/weak_ptr.h"
#include "luxxle/components/luxxle_education/education_urls.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/constants/webui_url_constants.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/webui_url_constants.h"
#include "chrome/test/base/chrome_test_utils.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/browser/web_ui_data_source.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"

namespace luxxle_education {

class LuxxleEducationPageUIBrowserTest : public InProcessBrowserTest {
 protected:
  void SetUpOnMainThread() override { CreateAndAddWebUITestDataSource(); }

  void NavigateToEducationPage(EducationPageType page_type) {
    auto webui_url = GetEducationPageBrowserURL(page_type);
    ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), webui_url));
    auto* web_contents = chrome_test_utils::GetActiveWebContents(this);
    ASSERT_TRUE(WaitForLoadStop(web_contents));
  }

  void PostMessageFromIFrame(std::string_view message_data) {
    message_data_ = message_data;
    auto* web_contents = chrome_test_utils::GetActiveWebContents(this);
    ASSERT_TRUE(content::ExecJs(web_contents, R"(
      const app = document.querySelector('luxxle-education-app')
      app.url_ = 'chrome://webui-test/'
      app.shadowRoot.querySelector('#content').src = app.url_
    )"));
  }

 private:
  void CreateAndAddWebUITestDataSource() {
    auto* web_contents = chrome_test_utils::GetActiveWebContents(this);

    content::WebUIDataSource* source = content::WebUIDataSource::CreateAndAdd(
        web_contents->GetBrowserContext(), chrome::kChromeUIWebUITestHost);

    source->OverrideContentSecurityPolicy(
        network::mojom::CSPDirectiveName::FrameAncestors,
        "frame-ancestors chrome://* 'self';");

    source->SetRequestFilter(
        base::BindRepeating([](const std::string&) { return true; }),
        base::BindRepeating(
            &LuxxleEducationPageUIBrowserTest::HandleWebUITestRequest,
            weak_factory_.GetWeakPtr()));
  }

  void HandleWebUITestRequest(
      const std::string& path,
      content::WebUIDataSource::GotDataCallback callback) {
    if (path == "post-message.js") {
      std::string js = "window.parent.postMessage(" + message_data_ + ", '*')";
      std::move(callback).Run(
          base::MakeRefCounted<base::RefCountedString>(std::move(js)));
      return;
    }

    static const char kHTML[] = R"(
      <!doctype html>
      <html>
      <script src='/post-message.js'></script>
      <body>
        Hello world!
      </body>
      </html>
    )";

    std::move(callback).Run(
        base::MakeRefCounted<base::RefCountedString>(std::move(kHTML)));
  }

  std::string message_data_;
  base::WeakPtrFactory<LuxxleEducationPageUIBrowserTest> weak_factory_{this};
};

IN_PROC_BROWSER_TEST_F(LuxxleEducationPageUIBrowserTest, OpenWalletOnboarding) {
  NavigateToEducationPage(EducationPageType::kGettingStarted);

  content::WebContentsAddedObserver added_observer;

  PostMessageFromIFrame(R"(
      {messageType: 'browser-command',
       command: 'open-wallet-onboarding'})");

  auto* new_web_contents = added_observer.GetWebContents();
  EXPECT_EQ(new_web_contents->GetVisibleURL(), GURL(kLuxxleUIWalletPageURL));
}

IN_PROC_BROWSER_TEST_F(LuxxleEducationPageUIBrowserTest, OpenRewardsOnboarding) {
  NavigateToEducationPage(EducationPageType::kGettingStarted);

  content::WebContentsAddedObserver added_observer;

  PostMessageFromIFrame(R"(
      {messageType: 'browser-command',
       command: 'open-rewards-onboarding'})");

  auto* new_web_contents = added_observer.GetWebContents();
  EXPECT_EQ(new_web_contents->GetVisibleURL(), GURL(kRewardsPageTopURL));
}

#if BUILDFLAG(ENABLE_LUXXLE_VPN)

IN_PROC_BROWSER_TEST_F(LuxxleEducationPageUIBrowserTest, OpenVPNOnboarding) {
  NavigateToEducationPage(EducationPageType::kGettingStarted);

  content::WebContentsAddedObserver added_observer;

  PostMessageFromIFrame(R"(
      {messageType: 'browser-command',
       command: 'open-vpn-onboarding'})");

  auto* new_web_contents = added_observer.GetWebContents();
  EXPECT_EQ(new_web_contents->GetVisibleURL(), GURL(kVPNPanelURL));
}

#endif  // BUILDFLAG(ENABLE_LUXXLE_VPN)

IN_PROC_BROWSER_TEST_F(LuxxleEducationPageUIBrowserTest, OpenAPIChat) {
  NavigateToEducationPage(EducationPageType::kGettingStarted);

  content::WebContentsAddedObserver added_observer;

  PostMessageFromIFrame(R"(
      {messageType: 'browser-command',
       command: 'open-ai-chat'})");

  auto* new_web_contents = added_observer.GetWebContents();
  EXPECT_TRUE(
      new_web_contents->GetVisibleURL().spec().starts_with(kAIChatUIURL));
}

}  // namespace luxxle_education
