/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/url/url_utils.h"

#include "base/strings/sys_string_conversions.h"
#import "net/base/apple/url_conversions.h"
#include "net/base/registry_controlled_domains/registry_controlled_domain.h"
#include "net/base/url_util.h"
#include "url/gurl.h"
#include "url/url_util.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

// MARK: - Implementation

@implementation NSURL (Utilities)

std::string GetRegistry(const GURL& url) {
  if (url.host_piece().empty() || url.HostIsIPAddress()) {
    return std::string();  // No registry.
  }

  size_t registry_length = GetRegistryLength(
      url, net::registry_controlled_domains::INCLUDE_UNKNOWN_REGISTRIES,
      net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);

  if ((registry_length == std::string::npos) || (registry_length == 0)) {
    return std::string();  // No registry.
  }
  return std::string(url.host(), url.host().length() - registry_length,
                     registry_length);
}

+ (NSURL*)URLFromIDNString:(NSString*)idnString {
  return net::NSURLWithGURL(GURL(base::SysNSStringToUTF8(idnString)));
}

- (NSString*)luxxle_registry {
  return base::SysUTF8ToNSString(GetRegistry(net::GURLWithNSURL(self)));
}

- (NSString*)luxxle_domainAndRegistry {
  std::string domain = net::registry_controlled_domains::GetDomainAndRegistry(
      net::GURLWithNSURL(self),
      net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);
  return base::SysUTF8ToNSString(domain);
}

- (NSString*)luxxle_domainAndRegistryExcludingPrivateRegistries {
  std::string domain = net::registry_controlled_domains::GetDomainAndRegistry(
      net::GURLWithNSURL(self),
      net::registry_controlled_domains::EXCLUDE_PRIVATE_REGISTRIES);
  return base::SysUTF8ToNSString(domain);
}

- (bool)luxxle_isHostIPAddress {
  return net::GURLWithNSURL(self).HostIsIPAddress();
}

- (NSString*)luxxle_spec {
  return base::SysUTF8ToNSString(net::GURLWithNSURL(self).spec());
}

- (NSURL*)luxxle_addingQueryParameter:(NSString*)key value:(NSString*)value {
  GURL gurl_ = net::AppendQueryParameter(net::GURLWithNSURL(self),
                                         base::SysNSStringToUTF8(key),
                                         base::SysNSStringToUTF8(value));
  return net::NSURLWithGURL(gurl_);
}

- (NSURL*)luxxle_replacingQueryParameter:(NSString*)key value:(NSString*)value {
  GURL gurl_ = net::AppendOrReplaceQueryParameter(
      net::GURLWithNSURL(self), base::SysNSStringToUTF8(key),
      base::SysNSStringToUTF8(value));
  return net::NSURLWithGURL(gurl_);
}

- (nullable NSString*)luxxle_valueForQueryParameter:(NSString*)key {
  std::string result;
  bool success = net::GetValueForKeyInQuery(
      net::GURLWithNSURL(self), base::SysNSStringToUTF8(key), &result);
  if (success) {
    return base::SysUTF8ToNSString(result);
  }
  return nullptr;
}

- (bool)luxxle_hasScheme:(NSString*)scheme {
  return net::GURLWithNSURL(self).SchemeIs(
      base::SysNSStringToUTF8([scheme lowercaseString]));
}
@end

@implementation NSURL (StaticUtilities)

std::string GetRegistryFromHost(const std::string& host) {
  auto GetRegistryLength = [](const std::string& host) -> size_t {
    return net::registry_controlled_domains::PermissiveGetHostRegistryLength(
        host, net::registry_controlled_domains::INCLUDE_UNKNOWN_REGISTRIES,
        net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);
  };

  if (host.empty() || url::HostIsIPAddress(host)) {
    return std::string();  // No registry.
  }

  size_t registry_length = GetRegistryLength(host);

  if ((registry_length == std::string::npos) || (registry_length == 0)) {
    return std::string();  // No registry.
  }
  return std::string(host, host.length() - registry_length, registry_length);
}

+ (NSString*)luxxle_registryFromHost:(NSString*)host {
  return base::SysUTF8ToNSString(
      GetRegistryFromHost(base::SysNSStringToUTF8(host)));
}

+ (NSString*)luxxle_domainAndRegistryFromHost:(NSString*)host {
  std::string domain = net::registry_controlled_domains::GetDomainAndRegistry(
      base::SysNSStringToUTF8(host),
      net::registry_controlled_domains::INCLUDE_PRIVATE_REGISTRIES);
  return base::SysUTF8ToNSString(domain);
}

+ (NSString*)luxxle_domainAndRegistryExcludingPrivateRegistriesFromHost:
    (NSString*)host {
  std::string domain = net::registry_controlled_domains::GetDomainAndRegistry(
      base::SysNSStringToUTF8(host),
      net::registry_controlled_domains::EXCLUDE_PRIVATE_REGISTRIES);
  return base::SysUTF8ToNSString(domain);
}

+ (bool)luxxle_isHostIPAddressFromHost:(NSString*)host {
  return url::HostIsIPAddress(base::SysNSStringToUTF8(host));
}
@end
