/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_POLICY_LUXXLE_SIMPLE_POLICY_MAP_H_
#define LUXXLE_BROWSER_POLICY_LUXXLE_SIMPLE_POLICY_MAP_H_

#include "luxxle/components/ai_chat/core/common/pref_names.h"
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#include "luxxle/components/constants/pref_names.h"
#include "luxxle/components/ipfs/buildflags/buildflags.h"
#include "luxxle/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "components/policy/core/browser/configuration_policy_handler.h"
#include "components/policy/policy_constants.h"

#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
// REMOVED: #include "luxxle/components/luxxle_rewards/.*"
#include "luxxle/components/luxxle_sync/luxxle_sync_prefs.h"
// REMOVED: #include "luxxle/components/luxxle_wallet/.*"
#endif

#if BUILDFLAG(ENABLE_TOR)
#include "luxxle/components/tor/pref_names.h"
#endif

#if BUILDFLAG(ENABLE_LUXXLE_VPN)
// REMOVED: #include "luxxle/components/luxxle_vpn/.*"
#endif

#if BUILDFLAG(DEPRECATE_IPFS)
#include "luxxle/components/ipfs/ipfs_prefs.h"
#endif  // BUILDFLAG(DEPRECATE_IPFS)

namespace policy {

inline constexpr PolicyToPreferenceMapEntry kLuxxleSimplePolicyMap[] = {
#if BUILDFLAG(IS_WIN) || BUILDFLAG(IS_MAC) || BUILDFLAG(IS_LINUX)
    {policy::key::kLuxxleRewardsDisabled,
     luxxle_rewards::prefs::kDisabledByPolicy, base::Value::Type::BOOLEAN},
    {policy::key::kLuxxleWalletDisabled, luxxle_wallet::prefs::kDisabledByPolicy,
     base::Value::Type::BOOLEAN},
    {policy::key::kLuxxleShieldsDisabledForUrls,
     kManagedLuxxleShieldsDisabledForUrls, base::Value::Type::LIST},
    {policy::key::kLuxxleShieldsEnabledForUrls,
     kManagedLuxxleShieldsEnabledForUrls, base::Value::Type::LIST},
    {policy::key::kLuxxleSyncUrl, luxxle_sync::kCustomSyncServiceUrl,
     base::Value::Type::STRING},
#endif

#if BUILDFLAG(ENABLE_TOR)
    {policy::key::kTorDisabled, tor::prefs::kTorDisabled,
     base::Value::Type::BOOLEAN},
#endif
#if BUILDFLAG(ENABLE_LUXXLE_VPN)
    {policy::key::kLuxxleVPNDisabled, luxxle_vpn::prefs::kManagedLuxxleVPNDisabled,
     base::Value::Type::BOOLEAN},
#endif
    {policy::key::kLuxxleAIChatEnabled, ai_chat::prefs::kEnabledByPolicy,
     base::Value::Type::BOOLEAN},

#if BUILDFLAG(DEPRECATE_IPFS)
    {policy::key::kIPFSEnabled, ipfs::prefs::kIPFSEnabledByPolicy,
     base::Value::Type::BOOLEAN},
#endif  // BUILDFLAG(DEPRECATE_IPFS)

};

}  // namespace policy

#endif  // LUXXLE_BROWSER_POLICY_LUXXLE_SIMPLE_POLICY_MAP_H_
