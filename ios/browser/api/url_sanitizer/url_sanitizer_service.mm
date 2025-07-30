// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/api/url_sanitizer/url_sanitizer_service+private.h"

#include <string>

#include "base/memory/raw_ptr.h"
#include "luxxle/components/url_sanitizer/browser/url_sanitizer_service.h"
#import "net/base/apple/url_conversions.h"
#include "url/gurl.h"

@interface URLSanitizerService () {
  raw_ptr<luxxle::URLSanitizerService> urlSanitizer_;  // NOT OWNED
}

@end

@implementation URLSanitizerService

- (instancetype)initWithURLSanitizerService:
    (luxxle::URLSanitizerService*)urlSanitizer {
  self = [super init];
  if (self) {
    urlSanitizer_ = urlSanitizer;
  }
  return self;
}

- (nullable NSURL*)sanitizeURL:(NSURL*)url {
  DCHECK(urlSanitizer_);
  GURL gurl = net::GURLWithNSURL(url);
  GURL cleanURL = urlSanitizer_->SanitizeURL(gurl);
  return net::NSURLWithGURL(cleanURL);
}

@end
