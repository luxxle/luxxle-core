/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */
static const char* kLuxxleCTExcludedHosts[] = {
    // Critical endpoints that shouldn't require SCTs so they always work
    "updates.luxxlesoftware.com",
    "updates-cdn.luxxlesoftware.com",
    "usage-ping.luxxle.com",
    // Test host for manual testing
    "sct-exempted.luxxlesoftware.com",
};

#define LUXXLE_PROFILE_NETWORK_CONTEXT_SERVICE_GET_CT_POLICY \
  for (const auto* host : kLuxxleCTExcludedHosts) {          \
    excluded.push_back(host);                               \
  }

#include "src/chrome/browser/net/profile_network_context_service.cc"
#undef LUXXLE_PROFILE_NETWORK_CONTEXT_SERVICE_GET_CT_POLICY
