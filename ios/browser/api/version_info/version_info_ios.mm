/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "luxxle/ios/browser/api/version_info/version_info_ios.h"

#include "base/strings/sys_string_conversions.h"
#include "luxxle/components/version_info/version_info.h"
#include "components/version_info/version_info.h"
#include "components/version_info/version_string.h"
#include "ios/chrome/common/channel_info.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

LuxxleCoreVersionInfoChannel const LuxxleCoreVersionInfoChannelStable =
    static_cast<NSInteger>(version_info::Channel::STABLE);
LuxxleCoreVersionInfoChannel const LuxxleCoreVersionInfoChannelBeta =
    static_cast<NSInteger>(version_info::Channel::BETA);
LuxxleCoreVersionInfoChannel const LuxxleCoreVersionInfoChannelDevelopment =
    static_cast<NSInteger>(version_info::Channel::DEV);
LuxxleCoreVersionInfoChannel const LuxxleCoreVersionInfoChannelNightly =
    static_cast<NSInteger>(version_info::Channel::CANARY);
LuxxleCoreVersionInfoChannel const LuxxleCoreVersionInfoChannelUnknown =
    static_cast<NSInteger>(version_info::Channel::UNKNOWN);

@implementation LuxxleCoreVersionInfo

+ (NSString*)luxxleCoreVersion {
  return base::SysUTF8ToNSString(
      version_info::GetLuxxleVersionWithoutChromiumMajorVersion());
}

+ (NSString*)chromiumVersion {
  return base::SysUTF8ToNSString(version_info::GetLuxxleChromiumVersionNumber());
}

+ (NSString*)channelString {
  return base::SysUTF8ToNSString(GetChannelString());
}

+ (LuxxleCoreVersionInfoChannel)channel {
  switch (GetChannel()) {
    case version_info::Channel::STABLE:
      return LuxxleCoreVersionInfoChannelStable;
    case version_info::Channel::BETA:
      return LuxxleCoreVersionInfoChannelBeta;
    case version_info::Channel::DEV:
      return LuxxleCoreVersionInfoChannelDevelopment;
    case version_info::Channel::CANARY:
      return LuxxleCoreVersionInfoChannelNightly;
    case version_info::Channel::UNKNOWN:
      return LuxxleCoreVersionInfoChannelUnknown;
  }
}
@end
