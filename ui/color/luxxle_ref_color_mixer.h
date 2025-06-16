// Copyright (c) 2024 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef LUXXLE_UI_COLOR_LUXXLE_REF_COLOR_MIXER_H_
#define LUXXLE_UI_COLOR_LUXXLE_REF_COLOR_MIXER_H_

#include "ui/color/color_provider.h"
#include "ui/color/color_provider_key.h"

namespace ui {

// This adds colors which are part of the Material Design Palette that Chromium
// doesn't define (as it doesn't need them) and redefines the baseline color
// palette to be slightly cooler.
void AddLuxxleRefColorMixer(ColorProvider* provider,
                           const ColorProviderKey& key);

}  // namespace ui

#endif  // LUXXLE_UI_COLOR_LUXXLE_REF_COLOR_MIXER_H_ 