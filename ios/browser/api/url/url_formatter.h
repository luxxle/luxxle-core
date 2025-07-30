/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_IOS_BROWSER_API_URL_URL_FORMATTER_H_
#define LUXXLE_IOS_BROWSER_API_URL_URL_FORMATTER_H_

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSInteger LuxxleURLSchemeDisplay NS_TYPED_ENUM
    NS_SWIFT_NAME(URLFormatter.SchemeDisplay);

OBJC_EXPORT LuxxleURLSchemeDisplay const LuxxleURLSchemeDisplayShow;
OBJC_EXPORT LuxxleURLSchemeDisplay const LuxxleURLSchemeDisplayOmitHttpAndHttps;
/// Omit cryptographic (i.e. https and wss).
OBJC_EXPORT LuxxleURLSchemeDisplay const LuxxleURLSchemeDisplayOmitCryptographic;

NS_SWIFT_NAME(URLFormatter.FormatType)
typedef NS_OPTIONS(NSUInteger, LuxxleURLFormatterFormatType) {
  LuxxleURLFormatterFormatTypeOmitNothing = 0,
  LuxxleURLFormatterFormatTypeOmitUsernamePassword = 1 << 0,
  LuxxleURLFormatterFormatTypeOmitHTTP = 1 << 1,
  LuxxleURLFormatterFormatTypeOmitTrailingSlashOnBareHostname = 1 << 2,
  LuxxleURLFormatterFormatTypeOmitHTTPS = 1 << 3,
  LuxxleURLFormatterFormatTypeOmitTrivialSubdomains = 1 << 5,
  LuxxleURLFormatterFormatTypeTrimAfterHost = 1 << 6,
  LuxxleURLFormatterFormatTypeOmitFileScheme = 1 << 7,
  LuxxleURLFormatterFormatTypeOmitMailToScheme = 1 << 8,
  LuxxleURLFormatterFormatTypeOmitMobilePrefix = 1 << 9,

  /// Omits Username & Password, HTTP (not HTTPS), and Trailing Slash
  LuxxleURLFormatterFormatTypeOmitDefaults =
      LuxxleURLFormatterFormatTypeOmitUsernamePassword |
      LuxxleURLFormatterFormatTypeOmitHTTP |
      LuxxleURLFormatterFormatTypeOmitTrailingSlashOnBareHostname
};

NS_SWIFT_NAME(URLFormatter.UnescapeRule)
typedef NS_OPTIONS(NSUInteger, LuxxleURLFormatterUnescapeRule) {
  LuxxleURLFormatterUnescapeRuleNone = 0,
  LuxxleURLFormatterUnescapeRuleNormal = 1 << 0,
  LuxxleURLFormatterUnescapeRuleSpaces = 1 << 1,
  LuxxleURLFormatterUnescapeRulePathSeparators = 1 << 2,
  LuxxleURLFormatterUnescapeRuleSpecialCharsExceptPathSeparators = 1 << 3,
  LuxxleURLFormatterUnescapeRuleReplacePlusWithSpace = 1 << 4
};

OBJC_EXPORT
NS_SWIFT_NAME(URLFormatter)
@interface LuxxleURLFormatter : NSObject
- (instancetype)init NS_UNAVAILABLE;

/// Format a URL "origin/host" for Security Display
/// origin - The origin of the URL to format
/// schemeDisplay - Determines whether or not to omit the scheme
+ (NSString*)formatURLOriginForSecurityDisplay:(NSString*)origin
                                 schemeDisplay:
                                     (LuxxleURLSchemeDisplay)schemeDisplay;

/// Format a URL "origin/host" omitting the scheme, path, and trivial
/// sub-domains. origin - The origin to be formatted
+ (NSString*)formatURLOriginForDisplayOmitSchemePathAndTrivialSubdomains:
    (NSString*)origin;

/// Format a URL
/// url - The URL string to be formatted
/// formatTypes - Formatter options when formatting the URL. Typically used to
/// omit certain parts of a URL unescapeOptions - Options passed to the
/// formatter for UN-Escaping parts of a URL
+ (NSString*)formatURL:(NSString*)url
           formatTypes:(LuxxleURLFormatterFormatType)formatTypes
       unescapeOptions:(LuxxleURLFormatterUnescapeRule)unescapeOptions;
@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_URL_URL_FORMATTER_H_
