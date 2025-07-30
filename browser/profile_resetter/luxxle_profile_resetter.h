/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_PROFILE_RESETTER_LUXXLE_PROFILE_RESETTER_H_
#define LUXXLE_BROWSER_PROFILE_RESETTER_LUXXLE_PROFILE_RESETTER_H_

#include "chrome/browser/profile_resetter/profile_resetter.h"

// Reset luxxle specific prefs.
class LuxxleProfileResetter : public ProfileResetter {
 public:
  using ProfileResetter::ProfileResetter;
  LuxxleProfileResetter(const LuxxleProfileResetter&) = delete;
  LuxxleProfileResetter& operator=(const LuxxleProfileResetter&) = delete;
  ~LuxxleProfileResetter() override;

  // ProfileResetter overrides:
  void ResetDefaultSearchEngine() override;
};

#endif  // LUXXLE_BROWSER_PROFILE_RESETTER_LUXXLE_PROFILE_RESETTER_H_
