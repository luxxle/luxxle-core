/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_NET_LUXXLE_SITE_HACKS_NETWORK_DELEGATE_HELPER_H_
#define LUXXLE_BROWSER_NET_LUXXLE_SITE_HACKS_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "luxxle/browser/net/url_context.h"

namespace net {
class URLRequest;
}

namespace luxxle {

int OnBeforeURLRequest_SiteHacksWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<LuxxleRequestInfo> ctx);

int OnBeforeStartTransaction_SiteHacksWork(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<LuxxleRequestInfo> ctx);

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_NET_LUXXLE_SITE_HACKS_NETWORK_DELEGATE_HELPER_H_
