/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/color/playlist/playlist_color_mixer.h"

#include "luxxle/browser/ui/color/luxxle_color_id.h"
#include "luxxle/ui/color/nala/nala_color_id.h"
#include "ui/color/color_mixer.h"
#include "ui/color/color_provider.h"
#include "ui/color/color_recipe.h"

namespace playlist {

void AddThemeColorMixer(ui::ColorProvider* provider,
                        const ui::ColorProviderKey& key) {
  ui::ColorMixer& mixer = provider->AddMixer();
  mixer[kColorLuxxlePlaylistAddedIcon] = {nala::kColorSystemfeedbackSuccessIcon};
  mixer[kColorLuxxlePlaylistCheckedIcon] = {nala::kColorIconInteractive};
  mixer[kColorLuxxlePlaylistSelectedBackground] = {
      nala::kColorContainerInteractive};
  mixer[kColorLuxxlePlaylistListBorder] = {nala::kColorDividerSubtle};
  mixer[kColorLuxxlePlaylistMoveDialogDescription] = {nala::kColorTextSecondary};
  mixer[kColorLuxxlePlaylistMoveDialogCreatePlaylistAndMoveTitle] = {
      nala::kColorTextPrimary};
  mixer[kColorLuxxlePlaylistNewPlaylistDialogNameLabel] = {
      nala::kColorTextPrimary};
  mixer[kColorLuxxlePlaylistNewPlaylistDialogItemsLabel] = {
      nala::kColorTextSecondary};
  mixer[kColorLuxxlePlaylistTextInteractive] = {nala::kColorTextInteractive};
}

}  // namespace playlist
