/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/net/luxxle_service_key_network_delegate_helper.h"

#include <string>
#include <vector>

#include "base/no_destructor.h"
#include "luxxle/components/constants/luxxle_services_key.h"
#include "luxxle/components/constants/network_constants.h"
#include "luxxle/components/update_client/buildflags.h"
#include "net/base/net_errors.h"
#include "net/http/http_request_headers.h"
#include "url/gurl.h"

namespace luxxle {

int OnBeforeStartTransaction_LuxxleServiceKey(
    net::HttpRequestHeaders* headers,
    const ResponseCallback& next_callback,
    std::shared_ptr<LuxxleRequestInfo> ctx) {
  static const base::NoDestructor<std::vector<std::string>> allowed_domains{
      {kExtensionUpdaterDomain, GURL(BUILDFLAG(UPDATER_DEV_ENDPOINT)).host(),
       GURL(BUILDFLAG(UPDATER_PROD_ENDPOINT)).host()}};

  const GURL& url = ctx->request_url;

  if (url.SchemeIs(url::kHttpsScheme)) {
    if (std::any_of(
            allowed_domains->begin(), allowed_domains->end(),
            [&url](const auto& domain) { return url.DomainIs(domain); })) {
      headers->SetHeader(kLuxxleServicesKeyHeader,
                         BUILDFLAG(LUXXLE_SERVICES_KEY));
    }
  }
  return net::OK;
}

}  // namespace luxxle
