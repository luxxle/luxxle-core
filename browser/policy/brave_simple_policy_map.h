/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_POLICY_BRAVE_SIMPLE_POLICY_MAP_H_
#define LUXXLE_BROWSER_POLICY_BRAVE_SIMPLE_POLICY_MAP_H_

#include "luxxle/components/ai_chat/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/brave_vpn/.*"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/ipfs/buildflags/buildflags.h"
#include "luxxle/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "components/policy/core/browser/configuration_policy_handler.h"
#include "components/policy/policy_constants.h"

#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
// REMOVED: #include "luxxle/components/brave_rewards/.*"
#include "luxxle/components/brave_sync/brave_sync_prefs.h"
// REMOVED: #include "luxxle/components/brave_wallet/.*"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "luxxle/components/tor/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_BRAVE_VPN)
// REMOVED: #include "luxxle/components/brave_vpn/.*"
#endif

#if BUILDFLAG(DEPRECATE_IPFS)
#include "luxxle/components/ipfs/ipfs_prefs.h"
#endif  // BUILDFLAG(DEPRECATE_IPFS)

namespace policy {

inline constexpr PolicyToPreferenceMapEntry kBraveSimplePolicyMap[] = {
#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
    {policy::key::kBraveRewardsDisabled,
     brave_rewards::prefs::kDisabledByPolicy, base::Value::Type::BOOLEAN},
    {policy::key::kBraveWalletDisabled, brave_wallet::prefs::kDisabledByPolicy,
     base::Value::Type::BOOLEAN},
    {policy::key::kBraveShieldsDisabledForUrls,
     kManagedBraveShieldsDisabledForUrls, base::Value::Type::LIST},
    {policy::key::kBraveShieldsEnabledForUrls,
     kManagedBraveShieldsEnabledForUrls, base::Value::Type::LIST},
    {policy::key::kBraveSyncUrl, brave_sync::kCustomSyncServiceUrl,
     base::Value::Type::STRING},
#endif

#if BUILDFLAG(ENABLE_TOR)
    {policy::key::kTorDisabled, tor::prefs::kTorDisabled,
     base::Value::Type::BOOLEAN},
#endif
#if BUILDFLAG(ENABLE_BRAVE_VPN)
    {policy::key::kBraveVPNDisabled, brave_vpn::prefs::kManagedBraveVPNDisabled,
     base::Value::Type::BOOLEAN},
#endif
    {policy::key::kBraveAIChatEnabled, ai_chat::prefs::kEnabledByPolicy,
     base::Value::Type::BOOLEAN},

#if BUILDFLAG(DEPRECATE_IPFS)
    {policy::key::kIPFSEnabled, ipfs::prefs::kIPFSEnabledByPolicy,
     base::Value::Type::BOOLEAN},
#endif  // BUILDFLAG(DEPRECATE_IPFS)

};

}  // namespace policy

#endif  // LUXXLE_BROWSER_POLICY_BRAVE_SIMPLE_POLICY_MAP_H_
