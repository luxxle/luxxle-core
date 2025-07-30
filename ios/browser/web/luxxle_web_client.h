// Copyright (c) 2020 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_WEB_LUXXLE_WEB_CLIENT_H_
#define LUXXLE_IOS_BROWSER_WEB_LUXXLE_WEB_CLIENT_H_

#include <memory>
#include <string>
#include <vector>

#include "ios/chrome/browser/web/model/chrome_web_client.h"

class LuxxleWebClient : public ChromeWebClient {
 public:
  LuxxleWebClient();
  LuxxleWebClient(const LuxxleWebClient&) = delete;
  LuxxleWebClient& operator=(const LuxxleWebClient&) = delete;
  ~LuxxleWebClient() override;

  void SetLegacyUserAgent(const std::string& user_agent);

  // WebClient implementation.
  std::unique_ptr<web::WebMainParts> CreateWebMainParts() override;
  std::string GetUserAgent(web::UserAgentType type) const override;

  void AddAdditionalSchemes(Schemes* schemes) const override;
  bool IsAppSpecificURL(const GURL& url) const override;

  std::vector<web::JavaScriptFeature*> GetJavaScriptFeatures(
      web::BrowserState* browser_state) const override;

  bool EnableLongPressUIContextMenu() const override;
  bool EnableWebInspector(web::BrowserState* browser_state) const override;

  void PostBrowserURLRewriterCreation(
      web::BrowserURLRewriter* rewriter) override;

  void BuildEditMenu(web::WebState* web_state,
                     id<UIMenuBuilder>) const override;

 private:
  std::string legacy_user_agent_;
};

#endif  // LUXXLE_IOS_BROWSER_WEB_LUXXLE_WEB_CLIENT_H_
