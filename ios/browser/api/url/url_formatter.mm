/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/url/url_formatter.h"

#include "base/functional/bind.h"
#include "base/functional/callback.h"
#include "base/notreached.h"
#include "base/strings/sys_string_conversions.h"
#include "build/build_config.h"
#include "components/url_formatter/elide_url.h"
#include "components/url_formatter/url_formatter.h"
#include "net/base/apple/url_conversions.h"
#include "url/gurl.h"
#include "url/origin.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

// MARK: - SchemeDisplay
LuxxleURLSchemeDisplay const LuxxleURLSchemeDisplayShow =
    static_cast<NSInteger>(url_formatter::SchemeDisplay::SHOW);
LuxxleURLSchemeDisplay const LuxxleURLSchemeDisplayOmitHttpAndHttps =
    static_cast<NSInteger>(url_formatter::SchemeDisplay::OMIT_HTTP_AND_HTTPS);
/// Omit cryptographic (i.e. https and wss).
LuxxleURLSchemeDisplay const LuxxleURLSchemeDisplayOmitCryptographic =
    static_cast<NSInteger>(url_formatter::SchemeDisplay::OMIT_CRYPTOGRAPHIC);

// MARK: - Implementation

namespace {
constexpr char16_t kChromeSchema16[] = u"chrome://";
constexpr char16_t kLuxxleSchema16[] = u"luxxle://";
}  // namespace

namespace luxxle_utils {

bool ReplaceChromeToLuxxleScheme(std::u16string& url_string) {
  if (base::StartsWith(url_string, kChromeSchema16,
                       base::CompareCase::INSENSITIVE_ASCII)) {
    base::ReplaceFirstSubstringAfterOffset(&url_string, 0, kChromeSchema16,
                                           kLuxxleSchema16);
    return true;
  }

  return false;
}

}  // namespace luxxle_utils

@implementation LuxxleURLFormatter
+ (NSString*)formatURLOriginForSecurityDisplay:(NSString*)origin
                                 schemeDisplay:
                                     (LuxxleURLSchemeDisplay)schemeDisplay {
  std::u16string result = url_formatter::FormatUrlForSecurityDisplay(
      GURL(base::SysNSStringToUTF8(origin)),
      static_cast<url_formatter::SchemeDisplay>(schemeDisplay));
  luxxle_utils::ReplaceChromeToLuxxleScheme(result);
  return base::SysUTF16ToNSString(result) ?: @"";
}

+ (NSString*)formatURLOriginForDisplayOmitSchemePathAndTrivialSubdomains:
    (NSString*)origin {
  std::u16string result =
      url_formatter::FormatUrlForDisplayOmitSchemePathAndTrivialSubdomains(
          GURL(base::SysNSStringToUTF8(origin)));
  luxxle_utils::ReplaceChromeToLuxxleScheme(result);
  return base::SysUTF16ToNSString(result) ?: @"";
}

+ (NSString*)formatURL:(NSString*)url
           formatTypes:(LuxxleURLFormatterFormatType)formatTypes
       unescapeOptions:(LuxxleURLFormatterUnescapeRule)unescapeOptions {
  std::u16string result = url_formatter::FormatUrl(
      GURL(base::SysNSStringToUTF8(url)),
      static_cast<url_formatter::FormatUrlType>(formatTypes),
      static_cast<base::UnescapeRule::Type>(unescapeOptions), nullptr, nullptr,
      nullptr);
  luxxle_utils::ReplaceChromeToLuxxleScheme(result);
  return base::SysUTF16ToNSString(result) ?: @"";
}
@end
