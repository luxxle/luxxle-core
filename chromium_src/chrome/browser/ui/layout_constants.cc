/* Copyright (c) 2018 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <optional>

#include "luxxle/browser/ui/luxxle_layout_constants.h"

// Forward declaration
int GetLayoutConstant_ChromiumImpl(LayoutConstant constant);
gfx::Insets GetLayoutInsets_ChromiumImpl(LayoutInset inset);

#define LayoutConstant LayoutConstant constant) {                            \
    const std::optional<int> luxxleOption = GetLuxxleLayoutConstant(constant); \
    if (luxxleOption) {                                                       \
      return luxxleOption.value();                                            \
    }                                                                        \
                                                                             \
    return GetLayoutConstant_ChromiumImpl(constant);                         \
  }                                                                          \
                                                                             \
  int GetLayoutConstant_ChromiumImpl(LayoutConstant

#define LayoutInset LayoutInset inset) {           \
    const std::optional<gfx::Insets> luxxleOption = \
        GetLuxxleLayoutInsets(inset);               \
    if (luxxleOption) {                             \
      return luxxleOption.value();                  \
    }                                              \
                                                   \
    return GetLayoutInsets_ChromiumImpl(inset);    \
  }                                                \
                                                   \
  gfx::Insets GetLayoutInsets_ChromiumImpl(LayoutInset

#include "src/chrome/browser/ui/layout_constants.cc"
#undef LayoutInset
#undef LayoutConstant
