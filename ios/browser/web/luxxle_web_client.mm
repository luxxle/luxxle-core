// Copyright (c) 2020 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#import "luxxle/ios/browser/web/luxxle_web_client.h"

#include "base/check.h"
#include "base/functional/bind.h"
#include "base/ios/ns_error_util.h"
#include "base/strings/sys_string_conversions.h"
#include "luxxle/components/constants/url_constants.h"
#include "luxxle/ios/browser/api/web_view/luxxle_web_view.h"
#include "luxxle/ios/browser/web/luxxle_web_main_parts.h"
#import "components/translate/ios/browser/translate_java_script_feature.h"
#include "ios/chrome/browser/shared/model/url/chrome_url_constants.h"
#include "ios/chrome/browser/web/model/chrome_web_client.h"
#import "ios/components/security_interstitials/ios_security_interstitial_java_script_feature.h"
#import "ios/components/security_interstitials/lookalikes/lookalike_url_error.h"
#import "ios/components/security_interstitials/safe_browsing/safe_browsing_error.h"
#include "ios/components/webui/web_ui_url_constants.h"
#import "ios/public/provider/chrome/browser/url_rewriters/url_rewriters_api.h"
#import "ios/web/public/navigation/browser_url_rewriter.h"
#import "ios/web_view/internal/cwv_ssl_error_handler_internal.h"
#import "ios/web_view/internal/cwv_web_view_internal.h"
#import "ios/web_view/public/cwv_navigation_delegate.h"
#import "net/base/apple/url_conversions.h"
#include "url/gurl.h"

LuxxleWebClient::LuxxleWebClient() {}

LuxxleWebClient::~LuxxleWebClient() {}

std::unique_ptr<web::WebMainParts> LuxxleWebClient::CreateWebMainParts() {
  return std::make_unique<LuxxleWebMainParts>(
      *base::CommandLine::ForCurrentProcess());
}

std::string LuxxleWebClient::GetUserAgent(web::UserAgentType type) const {
  if (!legacy_user_agent_.empty()) {
    return legacy_user_agent_;
  }
  return ChromeWebClient::GetUserAgent(type);
}

void LuxxleWebClient::AddAdditionalSchemes(Schemes* schemes) const {
  ChromeWebClient::AddAdditionalSchemes(schemes);

  schemes->standard_schemes.push_back(kLuxxleUIScheme);
  schemes->secure_schemes.push_back(kLuxxleUIScheme);

  schemes->standard_schemes.push_back(kChromeUIUntrustedScheme);
  schemes->secure_schemes.push_back(kChromeUIUntrustedScheme);
}

bool LuxxleWebClient::IsAppSpecificURL(const GURL& url) const {
  // temporarily add `internal://` scheme handling until those pages can be
  // ported to WebUI
  return ChromeWebClient::IsAppSpecificURL(url) ||
         url.SchemeIs(kLuxxleUIScheme) ||
         url.SchemeIs(kChromeUIUntrustedScheme) || url.SchemeIs("internal");
}

bool WillHandleLuxxleURLRedirect(GURL* url, web::BrowserState* browser_state) {
  if (url->SchemeIs(kLuxxleUIScheme)) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(kChromeUIScheme);
    *url = url->ReplaceComponents(replacements);
  }
  return false;
}

std::vector<web::JavaScriptFeature*> LuxxleWebClient::GetJavaScriptFeatures(
    web::BrowserState* browser_state) const {
  // Add any JavaScriptFeature's from Chromium or Luxxle as needed
  std::vector<web::JavaScriptFeature*> features;
  features.push_back(
      security_interstitials::IOSSecurityInterstitialJavaScriptFeature::
          GetInstance());
  return features;
}

void LuxxleWebClient::PostBrowserURLRewriterCreation(
    web::BrowserURLRewriter* rewriter) {
  rewriter->AddURLRewriter(&WillHandleLuxxleURLRedirect);
  ChromeWebClient::PostBrowserURLRewriterCreation(rewriter);
}

bool LuxxleWebClient::EnableLongPressUIContextMenu() const {
  return CWVWebView.chromeContextMenuEnabled;
}

bool LuxxleWebClient::EnableWebInspector(
    web::BrowserState* browser_state) const {
  return CWVWebView.webInspectorEnabled;
}

void LuxxleWebClient::SetLegacyUserAgent(const std::string& user_agent) {
  legacy_user_agent_ = user_agent;
}

void LuxxleWebClient::BuildEditMenu(web::WebState* web_state,
                                   id<UIMenuBuilder> builder) const {
  LuxxleWebView* webView =
      static_cast<LuxxleWebView*>([LuxxleWebView webViewForWebState:web_state]);
  if (!webView) {
    return;
  }
  id<LuxxleWebViewUIDelegate> uiDelegate = webView.UIDelegate;

  if ([uiDelegate respondsToSelector:@selector(webView:
                                         buildEditMenuWithBuilder:)]) {
    return [uiDelegate webView:webView buildEditMenuWithBuilder:builder];
  }
}
