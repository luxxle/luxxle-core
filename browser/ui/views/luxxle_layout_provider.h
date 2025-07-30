/* Copyright (c) 2019 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_LUXXLE_LAYOUT_PROVIDER_H_
#define LUXXLE_BROWSER_UI_VIEWS_LUXXLE_LAYOUT_PROVIDER_H_

#include "chrome/browser/ui/views/chrome_layout_provider.h"

class LuxxleLayoutProvider : public ChromeLayoutProvider {
 public:
  LuxxleLayoutProvider() = default;
  LuxxleLayoutProvider(const LuxxleLayoutProvider&) = delete;
  LuxxleLayoutProvider& operator=(const LuxxleLayoutProvider&) = delete;
  ~LuxxleLayoutProvider() override = default;

  int GetCornerRadiusMetric(views::Emphasis emphasis,
                            const gfx::Size& size = gfx::Size()) const override;
  int GetDistanceMetric(int metric) const override;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_LUXXLE_LAYOUT_PROVIDER_H_
