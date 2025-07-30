/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_NET_LUXXLE_SYSTEM_REQUEST_HANDLER_H_
#define LUXXLE_BROWSER_NET_LUXXLE_SYSTEM_REQUEST_HANDLER_H_

#include <string>

namespace network {
struct ResourceRequest;
}

namespace luxxle {

std::string LuxxleServicesKeyForTesting();

void AddLuxxleServicesKeyHeader(network::ResourceRequest* url_request);

network::ResourceRequest OnBeforeSystemRequest(
    const network::ResourceRequest& url_request);

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_NET_LUXXLE_SYSTEM_REQUEST_HANDLER_H_
