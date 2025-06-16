// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/luxxle_domains/service_domains.h"

#include <string>

#include "base/command_line.h"
#include "base/containers/contains.h"
#include "base/containers/fixed_flat_set.h"
#include "base/containers/flat_map.h"
#include "base/logging.h"
#include "base/notreached.h"
#include "base/strings/strcat.h"
#include "luxxle/luxxle_domains/buildflags.h"

namespace luxxle_domains {

namespace {

constexpr char kLuxxleServicesSwitchValueDev[] = "dev";
constexpr char kLuxxleServicesSwitchValueStaging[] = "staging";
constexpr char kLuxxleServicesSwitchValueProduction[] = "prod";
constexpr char kLuxxleServicesEnvironmentSwitch[] = "luxxle-services-env";

std::string GetServicesDomainForSwitchValue(std::string env_from_switch) {
  if (env_from_switch == kLuxxleServicesSwitchValueStaging) {
    return BUILDFLAG(LUXXLE_SERVICES_STAGING_DOMAIN);
  }
  if (env_from_switch == kLuxxleServicesSwitchValueDev) {
    return BUILDFLAG(LUXXLE_SERVICES_DEV_DOMAIN);
  }
  // Default to production
  return BUILDFLAG(LUXXLE_SERVICES_PRODUCTION_DOMAIN);
}

bool IsValidSwitchValue(std::string value) {
  static constexpr auto kAllowedSwitchValues =
      base::MakeFixedFlatSet<std::string_view>(
          {kLuxxleServicesSwitchValueDev, kLuxxleServicesSwitchValueStaging,
           kLuxxleServicesSwitchValueProduction});
  return kAllowedSwitchValues.contains(value);
}

void MaybeWarnSwitchValue(std::string key, std::string value) {
  if (!value.empty()) {
    if (!IsValidSwitchValue(value)) {
      LOG(ERROR) << "The switch value for --" << key << " is \"" << value
                 << "\" which is not a valid value, please provide"
                 << " either \"" << kLuxxleServicesSwitchValueDev << "\", \""
                 << kLuxxleServicesSwitchValueStaging << "\", or \""
                 << kLuxxleServicesSwitchValueProduction << "\" (default).";
    } else {
      // It's useful to have this in the logs. This should be a temporary
      // dev or debug tool and not a permanent situation for a user.
      LOG(WARNING) << "Services domain(s) was overriden with the parameter: --"
                   << key << " and value \"" << value << "\"";
    }
  }
}

#if !defined(OFFICIAL_BUILD)
std::string ConvertEnvironmentToString(luxxle_domains::ServicesEnvironment env) {
  static const base::flat_map<ServicesEnvironment, std::string> envMap = {
      {luxxle_domains::ServicesEnvironment::DEV, kLuxxleServicesSwitchValueDev},
      {luxxle_domains::ServicesEnvironment::STAGING,
       kLuxxleServicesSwitchValueStaging},
      {luxxle_domains::ServicesEnvironment::PROD,
       kLuxxleServicesSwitchValueProduction}};

  auto it = envMap.find(env);
  if (it != envMap.end()) {
    return it->second;
  }

  NOTREACHED();
}
#endif

}  // namespace

std::string GetServicesDomain(
    std::string prefix,
    ServicesEnvironment env_value_default_override,
    base::CommandLine*
        command_line /* = base::CommandLine::ForCurrentProcess() */) {
  // Default to production
  std::string env_value = kLuxxleServicesSwitchValueProduction;

  // If a default parameter was supplied, use that instead, but only
  // for unofficial builds.
#if !defined(OFFICIAL_BUILD)
  env_value = ConvertEnvironmentToString(env_value_default_override);
#endif

  // If a global value was supplied via CLI, use that instead.
  std::string env_from_switch =
      command_line->GetSwitchValueASCII(kLuxxleServicesEnvironmentSwitch);
  MaybeWarnSwitchValue(kLuxxleServicesEnvironmentSwitch, env_from_switch);
  if (IsValidSwitchValue(env_from_switch)) {
    env_value = env_from_switch;
  }

  // If a value was supplied for this specific prefix via CLI, use that instead.
  if (!prefix.empty()) {
    std::string env_key = base::StrCat({"env-", prefix});
    env_from_switch = command_line->GetSwitchValueASCII(env_key);
    MaybeWarnSwitchValue(env_key, env_from_switch);
    if (IsValidSwitchValue(env_from_switch)) {
      env_value = env_from_switch;
    }
  }

  // Build hostname
  if (prefix.empty()) {
    return GetServicesDomainForSwitchValue(env_value);
  }

  return base::StrCat(
      {prefix, ".", GetServicesDomainForSwitchValue(env_value)});
}

}  // namespace luxxle_domains
