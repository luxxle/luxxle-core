/* Copyright (c) 2018 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/common/channel_info.h"

#include <string_view>

#include "base/environment.h"
#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "luxxle/common/luxxle_channel_info_posix.h"
#include "build/build_config.h"
#include "components/version_info/version_info.h"

namespace chrome {

std::string GetChannelName(WithExtendedStable with_extended_stable) {
  std::string modifier;
  luxxle::GetChannelImpl(&modifier, nullptr);
  return modifier;
}

std::string GetChannelSuffixForDataDir() {
  return std::string();
}

#if BUILDFLAG(IS_LINUX)
std::string GetChannelSuffixForExtraFlagsEnvVarName() {
#if defined(OFFICIAL_BUILD)
  version_info::Channel product_channel(chrome::GetChannel());
  switch (product_channel) {
    case version_info::Channel::DEV:
      return "_DEV";
    case version_info::Channel::BETA:
      return "_BETA";
    case version_info::Channel::CANARY:
      return "_NIGHTLY";
    case version_info::Channel::STABLE:
      return "_STABLE";
    default:
      return std::string();
  }
#else   // defined(OFFICIAL_BUILD)
  const char* const channel_name = getenv("CHROME_VERSION_EXTRA");
  return channel_name
             ? base::StrCat(
                   {"_", base::ToUpperASCII(std::string_view(channel_name))})
             : std::string();
#endif  // defined(OFFICIAL_BUILD)
}
#endif  // BUILDFLAG(IS_LINUX)

#if BUILDFLAG(IS_LINUX)
std::string GetDesktopName(base::Environment* env) {
  if (auto luxxle_snap = env->GetVar("LUXXLE_SNAP");
      luxxle_snap && *luxxle_snap == "1") {
    return "luxxle.desktop";
  }
#if defined(OFFICIAL_BUILD)
  version_info::Channel product_channel(chrome::GetChannel());
  switch (product_channel) {
    case version_info::Channel::DEV:
      return "luxxle-browser-dev.desktop";
    case version_info::Channel::BETA:
      return "luxxle-browser-beta.desktop";
    case version_info::Channel::CANARY:
      return "luxxle-browser-nightly.desktop";
    default:
      return "luxxle-browser.desktop";
  }
#else  // defined(OFFICIAL_BUILD)
  // Allow $CHROME_DESKTOP to override the built-in value, so that development
  // versions can set themselves as the default without interfering with
  // non-official, packaged versions using the built-in value.
  if (std::string name = env->GetVar("CHROME_DESKTOP").value_or(std::string());
      !name.empty()) {
    return name;
  }
  return "luxxle-browser.desktop";
#endif
}
#endif  // BUILDFLAG(IS_LINUX)

version_info::Channel GetChannel() {
  return luxxle::GetChannelImpl(nullptr, nullptr);
}

bool IsExtendedStableChannel() {
  // No extended stable channel for Luxxle.
  return false;
}

}  // namespace chrome
