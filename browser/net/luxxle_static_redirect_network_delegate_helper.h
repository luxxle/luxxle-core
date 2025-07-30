/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_NET_LUXXLE_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_
#define LUXXLE_BROWSER_NET_LUXXLE_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "luxxle/browser/net/url_context.h"

struct LuxxleRequestInfo;

namespace luxxle {

int OnBeforeURLRequest_StaticRedirectWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<LuxxleRequestInfo> ctx);

int OnBeforeURLRequest_StaticRedirectWorkForGURL(
    const GURL& request_url,
    GURL* new_url);

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_NET_LUXXLE_STATIC_REDIRECT_NETWORK_DELEGATE_HELPER_H_
