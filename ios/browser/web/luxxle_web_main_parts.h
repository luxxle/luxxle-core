// Copyright (c) 2019 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_IOS_BROWSER_WEB_LUXXLE_WEB_MAIN_PARTS_H_
#define LUXXLE_IOS_BROWSER_WEB_LUXXLE_WEB_MAIN_PARTS_H_

#include "ios/chrome/browser/web/model/chrome_main_parts.h"

class LuxxleWebMainParts : public IOSChromeMainParts {
 public:
  explicit LuxxleWebMainParts(const base::CommandLine& parsed_command_line);
  LuxxleWebMainParts(const LuxxleWebMainParts&) = delete;
  LuxxleWebMainParts& operator=(const LuxxleWebMainParts&) = delete;
  ~LuxxleWebMainParts() override;

 private:
  // web::WebMainParts implementation.
  void PreCreateMainMessageLoop() override;
  void PreMainMessageLoopRun() override;
};

#endif  // LUXXLE_IOS_BROWSER_WEB_LUXXLE_WEB_MAIN_PARTS_H_
