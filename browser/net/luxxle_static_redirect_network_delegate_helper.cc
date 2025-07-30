/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/net/luxxle_static_redirect_network_delegate_helper.h"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "luxxle/components/static_redirect_helper/static_redirect_helper.h"
#include "net/base/net_errors.h"

namespace luxxle {

int OnBeforeURLRequest_StaticRedirectWork(
    const ResponseCallback& next_callback,
    std::shared_ptr<LuxxleRequestInfo> ctx) {
  GURL new_url;
  int rc = OnBeforeURLRequest_StaticRedirectWorkForGURL(ctx->request_url,
                                                        &new_url);
  if (!new_url.is_empty()) {
    ctx->new_url_spec = new_url.spec();
  }
  return rc;
}

int OnBeforeURLRequest_StaticRedirectWorkForGURL(
    const GURL& request_url,
    GURL* new_url) {
  luxxle::StaticRedirectHelper(request_url, new_url);
  return net::OK;
}

}  // namespace luxxle
