// Copyright (c) 2025 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_APP_LUXXLE_PROFILE_CONTROLLER_PRIVATE_H_
#define LUXXLE_IOS_APP_LUXXLE_PROFILE_CONTROLLER_PRIVATE_H_

#include "base/memory/raw_ptr.h"
#include "luxxle/ios/app/luxxle_profile_controller.h"

class ProfileIOS;

@interface LuxxleProfileController (Private)
- (instancetype)initWithProfile:(raw_ptr<ProfileIOS>)profile;
@end

#endif  // LUXXLE_IOS_APP_LUXXLE_PROFILE_CONTROLLER_PRIVATE_H_
