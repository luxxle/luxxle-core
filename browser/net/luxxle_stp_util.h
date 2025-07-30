/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_NET_LUXXLE_STP_UTIL_H_
#define LUXXLE_BROWSER_NET_LUXXLE_STP_UTIL_H_

#include <string_view>

#include "base/containers/flat_set.h"
#include "net/http/http_response_headers.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace luxxle {

base::flat_set<std::string_view>* TrackableSecurityHeaders();

void RemoveTrackableSecurityHeadersForThirdParty(
    const GURL& request_url, const url::Origin& top_frame_origin,
    const net::HttpResponseHeaders* original_response_headers,
    scoped_refptr<net::HttpResponseHeaders>* override_response_headers);

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_NET_LUXXLE_STP_UTIL_H_
