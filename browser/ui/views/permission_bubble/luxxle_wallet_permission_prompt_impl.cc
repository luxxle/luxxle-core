/* Copyright (c) 2021 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/views/permission_bubble/luxxle_wallet_permission_prompt_impl.h"

#include <optional>

// REMOVED: #include "luxxle/browser/luxxle_wallet/.*"
#include "components/permissions/permission_uma_util.h"

LuxxleWalletPermissionPromptImpl::LuxxleWalletPermissionPromptImpl(
    Browser* browser,
    content::WebContents* web_contents,
    Delegate& delegate)
    : web_contents_(web_contents),
      delegate_(delegate),
      permission_requested_time_(base::TimeTicks::Now()) {
  DCHECK(web_contents_);
  ShowBubble();
}

LuxxleWalletPermissionPromptImpl::~LuxxleWalletPermissionPromptImpl() {
  luxxle_wallet::LuxxleWalletTabHelper::FromWebContents(web_contents_)
      ->CloseBubble();
}

void LuxxleWalletPermissionPromptImpl::ShowBubble() {
  luxxle_wallet::LuxxleWalletTabHelper::FromWebContents(web_contents_)
      ->ShowBubble();
}

bool LuxxleWalletPermissionPromptImpl::UpdateAnchor() {
  // Don't recreate the view for every BrowserView::Layout() which would cause
  // LuxxleWalletPermissionPromptImpl being destoryed which leads to bubble
  // dismissed unintentionally.
  return true;
}

permissions::PermissionPrompt::TabSwitchingBehavior
LuxxleWalletPermissionPromptImpl::GetTabSwitchingBehavior() {
  return permissions::PermissionPrompt::TabSwitchingBehavior::
      kDestroyPromptButKeepRequestPending;
}

permissions::PermissionPromptDisposition
LuxxleWalletPermissionPromptImpl::GetPromptDisposition() const {
  return permissions::PermissionPromptDisposition::ANCHORED_BUBBLE;
}

bool LuxxleWalletPermissionPromptImpl::IsAskPrompt() const {
  return true;
}

std::optional<gfx::Rect>
LuxxleWalletPermissionPromptImpl::GetViewBoundsInScreen() const {
  return std::nullopt;
}

bool LuxxleWalletPermissionPromptImpl::ShouldFinalizeRequestAfterDecided()
    const {
  return true;
}

std::vector<permissions::ElementAnchoredBubbleVariant>
LuxxleWalletPermissionPromptImpl::GetPromptVariants() const {
  return {};
}

std::optional<permissions::feature_params::PermissionElementPromptPosition>
LuxxleWalletPermissionPromptImpl::GetPromptPosition() const {
  return std::nullopt;
}
