/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_BRAVE_BROWSER_ACTIONS_H_
#define LUXXLE_BROWSER_UI_BRAVE_BROWSER_ACTIONS_H_

#include "chrome/browser/ui/browser_actions.h"

// Add more side panel actions for our playlist/chat panels.
class BraveBrowserActions : public BrowserActions {
 public:
  explicit BraveBrowserActions(Browser& browser);
  ~BraveBrowserActions() override;

 private:
  void InitializeBrowserActions() override;
};

#endif  // LUXXLE_BROWSER_UI_BRAVE_BROWSER_ACTIONS_H_
