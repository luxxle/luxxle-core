/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_NET_LUXXLE_AD_BLOCK_CSP_NETWORK_DELEGATE_HELPER_H_
#define LUXXLE_BROWSER_NET_LUXXLE_AD_BLOCK_CSP_NETWORK_DELEGATE_HELPER_H_

#include <memory>

#include "base/memory/scoped_refptr.h"
#include "luxxle/browser/net/url_context.h"

namespace net {
class HttpResponseHeaders;
}  // namespace net

class GURL;

namespace luxxle {

int OnHeadersReceived_AdBlockCspWork(
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers,
    GURL* allowed_unsafe_redirect_url,
    const luxxle::ResponseCallback& next_callback,
    std::shared_ptr<luxxle::LuxxleRequestInfo> ctx);

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_NET_LUXXLE_AD_BLOCK_CSP_NETWORK_DELEGATE_HELPER_H_
