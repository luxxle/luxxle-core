/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/luxxle_player/luxxle_player_action_icon_view.h"

#include <memory>
#include <string>

#include "base/strings/strcat.h"
#include "luxxle/browser/ui/color/luxxle_color_id.h"
#include "luxxle/components/luxxle_player/common/buildflags/buildflags.h"
#include "luxxle/components/luxxle_player/common/url_constants.h"
#include "luxxle/components/vector_icons/vector_icons.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "luxxle/grit/luxxle_theme_resources.h"
#include "chrome/browser/ui/browser_tabstrip.h"
#include "content/public/browser/web_contents.h"
#include "net/base/url_util.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/gfx/vector_icon_types.h"
#include "url/url_canon.h"
#include "url/url_util.h"

namespace {

GURL GetPlayerURL(content::WebContents* web_contents) {
  if (!web_contents) {
    return {};
  }

  const GURL& url = web_contents->GetLastCommittedURL();
  if (url.DomainIs("youtube.com") && url.path_piece() == "/watch" &&
      url.has_query()) {
    if (std::string video_id; net::GetValueForKeyInQuery(url, "v", &video_id)) {
      url::RawCanonOutputT<char> encoded_video_id;
      url::EncodeURIComponent(video_id, &encoded_video_id);
      return GURL(base::StrCat({luxxle_player::kLuxxlePlayerURL, "youtube/",
                                encoded_video_id.view()}));
    }
  }
  return {};
}

}  // namespace

LuxxlePlayerActionIconView::LuxxlePlayerActionIconView(
    CommandUpdater* command_updater,
    Browser& browser,
    IconLabelBubbleView::Delegate* icon_label_bubble_delegate,
    PageActionIconView::Delegate* page_action_icon_delegate)
    : PageActionIconView(command_updater,
                         0,
                         icon_label_bubble_delegate,
                         page_action_icon_delegate,
                         "LuxxlePlayerActionIconView",
                         /*ephemeral=*/false),
      browser_(browser) {
  SetVisible(false);

  SetLabel(l10n_util::GetStringUTF16(IDS_LUXXLE_PLAYER_ACTION_VIEW));
}

LuxxlePlayerActionIconView::~LuxxlePlayerActionIconView() = default;

void LuxxlePlayerActionIconView::OnExecuting(ExecuteSource execute_source) {
  CHECK(player_url_.is_valid());
  chrome::AddTabAt(base::to_address(browser_), player_url_, /*index*/ -1,
                   /*foreground=*/true);
}

views::BubbleDialogDelegate* LuxxlePlayerActionIconView::GetBubble() const {
  return nullptr;
}

void LuxxlePlayerActionIconView::UpdateIconImage() {
  SetImageModel(
      ui::ImageModel::FromResourceId(/* REMOVED: IDR_LUXXLE_PLAYER_ACTION_VIEW_ICON */ 0));
}

const gfx::VectorIcon& LuxxlePlayerActionIconView::GetVectorIcon() const {
  // We don't use vector icon because we need gradation effect.
  // TODO(sko) When Nala icon updates, try use vector icon and blending effect
  // to generate gradation effect.
  NOTREACHED();
}

void LuxxlePlayerActionIconView::UpdateImpl() {
  player_url_ = GetPlayerURL(GetWebContents());
  SetVisible(player_url_.is_valid());
}

void LuxxlePlayerActionIconView::UpdateBorder() {
  // Update insets using base class implementation.
  PageActionIconView::UpdateBorder();

  SetBorder(views::CreatePaddedBorder(
      views::CreateRoundedRectBorder(
          /*thickness*/ 1,
          /*corner_radius=*/8, kColorLuxxlePlayerActionViewBorder),
      GetInsets()));
}

BEGIN_METADATA(LuxxlePlayerActionIconView);
END_METADATA
