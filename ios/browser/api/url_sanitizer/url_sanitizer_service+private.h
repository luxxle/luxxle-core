// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_API_URL_SANITIZER_URL_SANITIZER_SERVICE_PRIVATE_H_
#define LUXXLE_IOS_BROWSER_API_URL_SANITIZER_URL_SANITIZER_SERVICE_PRIVATE_H_

#import <Foundation/Foundation.h>

#include "base/memory/raw_ptr.h"
#include "luxxle/components/url_sanitizer/browser/url_sanitizer_service.h"
#include "luxxle/ios/browser/api/url_sanitizer/url_sanitizer_service.h"

NS_ASSUME_NONNULL_BEGIN

@interface URLSanitizerService (Private)

- (instancetype)initWithURLSanitizerService:
    (luxxle::URLSanitizerService*)urlSanitizer;

@end

NS_ASSUME_NONNULL_END

#endif  // LUXXLE_IOS_BROWSER_API_URL_SANITIZER_URL_SANITIZER_SERVICE_PRIVATE_H_
