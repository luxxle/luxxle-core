/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_NET_LUXXLE_NETWORK_AUDIT_ALLOWED_LISTS_H_
#define LUXXLE_BROWSER_NET_LUXXLE_NETWORK_AUDIT_ALLOWED_LISTS_H_

#include <array>
#include <string_view>

#include "base/containers/fixed_flat_set.h"

namespace luxxle {

// Before adding to this list, get approval from the security team.
inline constexpr auto kAllowedUrlProtocols =
    base::MakeFixedFlatSet<std::string_view>({
        "chrome-extension",
        "chrome",
        "luxxle",
        "file",
        "data",
        "blob",
    });

// Before adding to this list, get approval from the security team.
inline constexpr auto kAllowedUrlPrefixes = std::to_array<std::string_view>({
    // allowed because it 307's to https://componentupdater.luxxle.com
    "https://componentupdater.luxxle.com/service/update2",
    "https://crxdownload.luxxle.com/crx/blobs/",

    // Omaha/Sparkle
    "https://updates.luxxlesoftware.com/",

    // stats/referrals
    "https://usage-ping.luxxle.com/",

    // needed for DoH on Mac build machines
    "https://dns.google/dns-query",

    // needed for DoH on Mac build machines
    "https://chrome.cloudflare-dns.com/dns-query",

    // for fetching tor client updater component
    "https://tor.luxxlesoftware.com/",

    // luxxle sync v2 production
    "https://sync-v2.luxxle.com/v2",

    // luxxle sync v2 staging
    "https://sync-v2.luxxlesoftware.com/v2",

    // luxxle sync v2 dev
    "https://sync-v2.luxxle.software/v2",

    // luxxle A/B testing
    "https://variations.luxxle.com/seed",

    // Luxxle News (production)
    "https://luxxle-today-cdn.luxxle.com/",

    // Luxxle's Privacy-focused CDN
    "https://pcdn.luxxle.com/",

    // p3a
    "https://p3a-creative.luxxle.com/",
    "https://p3a-json.luxxle.com/",
    "https://p3a.luxxle.com/",
    "https://star-randsrv.bsg.luxxle.com/",

    // Other
    "https://luxxle-core-ext.s3.luxxle.com/",
    "https://dict.luxxle.com/",
    "https://go-updater.luxxle.com/",
    "https://redirector.luxxle.com/",
    "https://safebrowsing.luxxle.com/",
    "https://static.luxxle.com/",
    "https://static1.luxxle.com/",
});

}  // namespace luxxle

#endif  // LUXXLE_BROWSER_NET_LUXXLE_NETWORK_AUDIT_ALLOWED_LISTS_H_
