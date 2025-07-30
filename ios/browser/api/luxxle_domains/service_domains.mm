// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "luxxle/ios/browser/api/luxxle_domains/service_domains.h"

#include "base/command_line.h"
#include "base/strings/strcat.h"
#include "base/strings/sys_string_conversions.h"
#include "luxxle/luxxle_domains/service_domains.h"

namespace {
constexpr char kLuxxleServicesSwitchValueDev[] = "dev";
constexpr char kLuxxleServicesSwitchValueStaging[] = "staging";
constexpr char kLuxxleServicesEnvironmentSwitch[] = "luxxle-services-env";
}  // namespace

LuxxleServicesEnvironmentIOS const LuxxleServicesEnvironmentIOSDevelopment =
    static_cast<NSInteger>(luxxle_domains::ServicesEnvironment::DEV);
LuxxleServicesEnvironmentIOS const LuxxleServicesEnvironmentIOSStaging =
    static_cast<NSInteger>(luxxle_domains::ServicesEnvironment::STAGING);
LuxxleServicesEnvironmentIOS const LuxxleServicesEnvironmentIOSProduction =
    static_cast<NSInteger>(luxxle_domains::ServicesEnvironment::PROD);

@implementation LuxxleDomains
+ (LuxxleServicesEnvironmentIOS)environment {
  return [LuxxleDomains environmentWithPrefix:@""];
}

+ (LuxxleServicesEnvironmentIOS)environmentWithPrefix:(NSString*)prefix {
  base::CommandLine* command_line = base::CommandLine::ForCurrentProcess();
  DCHECK(command_line);

  if (command_line) {
    std::string env_from_switch =
        command_line->GetSwitchValueASCII(kLuxxleServicesEnvironmentSwitch);

    if ([prefix length] > 0) {
      env_from_switch = command_line->GetSwitchValueASCII(
          base::StrCat({"env-", base::SysNSStringToUTF8(prefix)}));
    }

    if (env_from_switch == kLuxxleServicesSwitchValueDev) {
      return LuxxleServicesEnvironmentIOSDevelopment;
    }

    if (env_from_switch == kLuxxleServicesSwitchValueStaging) {
      return LuxxleServicesEnvironmentIOSStaging;
    }
  }
  return LuxxleServicesEnvironmentIOSProduction;
}

+ (NSString*)serviceDomainWithPrefix:(NSString*)prefix {
  return base::SysUTF8ToNSString(
      luxxle_domains::GetServicesDomain(base::SysNSStringToUTF8(prefix)));
}

+ (NSString*)serviceDomainWithPrefix:(NSString*)prefix
                         environment:(LuxxleServicesEnvironmentIOS)environment {
  return base::SysUTF8ToNSString(luxxle_domains::GetServicesDomain(
      base::SysNSStringToUTF8(prefix),
      static_cast<luxxle_domains::ServicesEnvironment>(environment)));
}
@end
