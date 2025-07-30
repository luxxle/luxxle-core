/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/overlay/luxxle_back_to_tab_label_button.h"
#include "luxxle/browser/ui/views/overlay/luxxle_video_overlay_window_views.h"

#define LUXXLE_UPDATE_MAX_SIZE max_size_ = work_area.size();
#define BackToTabLabelButton LuxxleBackToTabLabelButton

#include "src/chrome/browser/ui/views/overlay/video_overlay_window_views.cc"

#undef BackToTabLabelButton
#undef LUXXLE_UPDATE_MAX_SIZE
