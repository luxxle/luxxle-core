/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_PROFILES_LUXXLE_PROFILE_MANAGER_H_
#define LUXXLE_BROWSER_PROFILES_LUXXLE_PROFILE_MANAGER_H_

#include <string>

#include "chrome/browser/profiles/profile_manager.h"

class LuxxleProfileManager : public ProfileManager {
 public:
  explicit LuxxleProfileManager(const base::FilePath& user_data_dir);
  LuxxleProfileManager(const LuxxleProfileManager&) = delete;
  LuxxleProfileManager& operator=(const LuxxleProfileManager&) = delete;

  void InitProfileUserPrefs(Profile* profile) override;
  void SetNonPersonalProfilePrefs(Profile* profile) override;
  bool IsAllowedProfilePath(const base::FilePath& path) const override;
  bool LoadProfileByPath(const base::FilePath& profile_path,
                         bool incognito,
                         ProfileLoadedCallback callback) override;

 protected:
  void DoFinalInitForServices(Profile* profile,
                              bool go_off_the_record) override;

 private:
  void MigrateProfileNames();
};

class LuxxleProfileManagerWithoutInit : public LuxxleProfileManager {
 public:
  LuxxleProfileManagerWithoutInit(const LuxxleProfileManagerWithoutInit&) =
      delete;
  LuxxleProfileManagerWithoutInit& operator=(
      const LuxxleProfileManagerWithoutInit&) = delete;
  explicit LuxxleProfileManagerWithoutInit(const base::FilePath& user_data_dir);
};

#endif  // LUXXLE_BROWSER_PROFILES_LUXXLE_PROFILE_MANAGER_H_
