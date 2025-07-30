/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/components/constants/webui_url_constants.h"
#include "chrome/common/webui_url_constants.h"

#define kChromeUIAttributionInternalsHost                                     \
  kChromeUIAttributionInternalsHost, kAdblockHost, kAdblockInternalsHost,     \
      kRewardsPageHost, kRewardsInternalsHost, kWelcomeHost, kWalletPageHost, \
      kTorInternalsHost, kSkusInternalsHost, kAdsInternalsHost,               \
      kNewTabTakeoverHost

#include "src/chrome/common/webui_url_constants.cc"

#undef kChromeUIAttributionInternalsHost
