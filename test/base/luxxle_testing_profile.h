/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_TEST_BASE_LUXXLE_TESTING_PROFILE_H_
#define LUXXLE_TEST_BASE_LUXXLE_TESTING_PROFILE_H_

#include "chrome/test/base/testing_profile.h"

class LuxxleTestingProfile : public TestingProfile {
 public:
  LuxxleTestingProfile();
  LuxxleTestingProfile(const base::FilePath& path,
                      Delegate* delegate,
                      CreateMode create_mode);
  ~LuxxleTestingProfile() override = default;
};

#endif  // LUXXLE_TEST_BASE_LUXXLE_TESTING_PROFILE_H_
