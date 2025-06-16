/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_STARTUP_BRAVE_STARTUP_TAB_PROVIDER_IMPL_H_
#define LUXXLE_BROWSER_UI_STARTUP_BRAVE_STARTUP_TAB_PROVIDER_IMPL_H_

#include "chrome/browser/ui/startup/startup_tab_provider.h"

class StartupBrowserCreator;

class BraveStartupTabProviderImpl : public StartupTabProviderImpl {
 public:
  BraveStartupTabProviderImpl() = default;
  BraveStartupTabProviderImpl(const BraveStartupTabProviderImpl&) = delete;
  BraveStartupTabProviderImpl& operator=(const BraveStartupTabProviderImpl&) =
      delete;

  // StartupTabProvider:
  StartupTabs GetDistributionFirstRunTabs(
      StartupBrowserCreator* browser_creator) const override;
};

#endif  // LUXXLE_BROWSER_UI_STARTUP_BRAVE_STARTUP_TAB_PROVIDER_IMPL_H_
