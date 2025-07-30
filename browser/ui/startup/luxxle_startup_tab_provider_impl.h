/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_STARTUP_LUXXLE_STARTUP_TAB_PROVIDER_IMPL_H_
#define LUXXLE_BROWSER_UI_STARTUP_LUXXLE_STARTUP_TAB_PROVIDER_IMPL_H_

#include "chrome/browser/ui/startup/startup_tab_provider.h"

class StartupBrowserCreator;

class LuxxleStartupTabProviderImpl : public StartupTabProviderImpl {
 public:
  LuxxleStartupTabProviderImpl() = default;
  LuxxleStartupTabProviderImpl(const LuxxleStartupTabProviderImpl&) = delete;
  LuxxleStartupTabProviderImpl& operator=(const LuxxleStartupTabProviderImpl&) =
      delete;

  // StartupTabProvider:
  StartupTabs GetDistributionFirstRunTabs(
      StartupBrowserCreator* browser_creator) const override;
};

#endif  // LUXXLE_BROWSER_UI_STARTUP_LUXXLE_STARTUP_TAB_PROVIDER_IMPL_H_
