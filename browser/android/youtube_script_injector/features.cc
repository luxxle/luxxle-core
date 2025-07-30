/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/android/youtube_script_injector/features.h"

#include "base/feature_list.h"
#include "build/build_config.h"

namespace preferences {
namespace features {

BASE_FEATURE(kLuxxleBackgroundVideoPlayback,
             "LuxxleBackgroundVideoPlayback",
             base::FEATURE_ENABLED_BY_DEFAULT);

BASE_FEATURE(kLuxxlePictureInPictureForYouTubeVideos,
             "LuxxlePictureInPictureForYouTubeVideos",
             base::FEATURE_ENABLED_BY_DEFAULT);

}  // namespace features
}  // namespace preferences
