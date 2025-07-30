/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_SYSTEM_MENU_MODEL_BUILDER_H_
#define LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_SYSTEM_MENU_MODEL_BUILDER_H_

#include "chrome/browser/ui/views/frame/system_menu_model_builder.h"

// This class can be used to update toolbar/frame context menus.
class LuxxleSystemMenuModelBuilder : public SystemMenuModelBuilder {
 public:
  using SystemMenuModelBuilder::SystemMenuModelBuilder;
  virtual ~LuxxleSystemMenuModelBuilder();

 private:
  // SystemMenuModelBuilder
  void BuildSystemMenuForBrowserWindow(ui::SimpleMenuModel* model) override;

  void InsertLuxxleSystemMenuForBrowserWindow(ui::SimpleMenuModel* model);
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_FRAME_LUXXLE_SYSTEM_MENU_MODEL_BUILDER_H_
