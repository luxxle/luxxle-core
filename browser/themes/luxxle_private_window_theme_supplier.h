/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_THEMES_LUXXLE_PRIVATE_WINDOW_THEME_SUPPLIER_H_
#define LUXXLE_BROWSER_THEMES_LUXXLE_PRIVATE_WINDOW_THEME_SUPPLIER_H_

#include "chrome/browser/themes/custom_theme_supplier.h"

// A theme supplier for private (or tor) window.
class LuxxlePrivateWindowThemeSupplier : public CustomThemeSupplier {
 public:
  explicit LuxxlePrivateWindowThemeSupplier(bool private_window);

  LuxxlePrivateWindowThemeSupplier(const LuxxlePrivateWindowThemeSupplier&) =
      delete;
  LuxxlePrivateWindowThemeSupplier& operator=(
      const LuxxlePrivateWindowThemeSupplier&) = delete;

 protected:
  ~LuxxlePrivateWindowThemeSupplier() override;

  void AddColorMixers(ui::ColorProvider* provider,
                      const ui::ColorProviderKey& key) const override;

  // false if this is for tor window.
  bool for_private_window_ = true;
};

#endif  // LUXXLE_BROWSER_THEMES_LUXXLE_PRIVATE_WINDOW_THEME_SUPPLIER_H_
