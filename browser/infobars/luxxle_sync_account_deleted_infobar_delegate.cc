/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/infobars/luxxle_sync_account_deleted_infobar_delegate.h"

#include <memory>
#include <utility>

#include "base/memory/ptr_util.h"
#include "luxxle/browser/ui/views/infobars/luxxle_sync_account_deleted_infobar.h"
#include "luxxle/components/luxxle_sync/luxxle_sync_prefs.h"
#include "luxxle/components/constants/webui_url_constants.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/infobars/confirm_infobar_creator.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/chrome_pages.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/infobar.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/views/vector_icons.h"

// static
void LuxxleSyncAccountDeletedInfoBarDelegate::Create(
    content::WebContents* active_web_contents,
    Profile* profile,
    Browser* browser) {
  luxxle_sync::Prefs luxxle_sync_prefs(profile->GetPrefs());
  const bool notification_pending =
      luxxle_sync_prefs.IsSyncAccountDeletedNoticePending();
  if (!notification_pending) {
    return;
  }

  // If we already are on luxxle://settings/luxxleSync/setup page, don't show
  // informer
  if (!active_web_contents || active_web_contents->GetURL() ==
                                  chrome::GetSettingsUrl(kLuxxleSyncSetupPath)) {
    return;
  }

  infobars::ContentInfoBarManager* infobar_manager =
      infobars::ContentInfoBarManager::FromWebContents(active_web_contents);

  if (!infobar_manager) {
    return;
  }

  // Create custom confirm infobar
  std::unique_ptr<infobars::InfoBar> infobar(
      std::make_unique<LuxxleSyncAccountDeletedInfoBar>(
          base::WrapUnique<ConfirmInfoBarDelegate>(
              new LuxxleSyncAccountDeletedInfoBarDelegate(browser, profile))));

  // Show infobar
  infobar_manager->AddInfoBar(std::move(infobar));
}

// Start class impl
LuxxleSyncAccountDeletedInfoBarDelegate::LuxxleSyncAccountDeletedInfoBarDelegate(
    Browser* browser,
    Profile* profile)
    : ConfirmInfoBarDelegate(), profile_(profile), browser_(browser) {}

LuxxleSyncAccountDeletedInfoBarDelegate::
    ~LuxxleSyncAccountDeletedInfoBarDelegate() {}

infobars::InfoBarDelegate::InfoBarIdentifier
LuxxleSyncAccountDeletedInfoBarDelegate::GetIdentifier() const {
  return LUXXLE_SYNC_ACCOUNT_DELETED_INFOBAR;
}

const gfx::VectorIcon& LuxxleSyncAccountDeletedInfoBarDelegate::GetVectorIcon()
    const {
  return views::kInfoIcon;
}

bool LuxxleSyncAccountDeletedInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  return false;
}

void LuxxleSyncAccountDeletedInfoBarDelegate::InfoBarDismissed() {
  luxxle_sync::Prefs luxxle_sync_prefs(profile_->GetPrefs());
  luxxle_sync_prefs.SetSyncAccountDeletedNoticePending(false);
}

std::u16string LuxxleSyncAccountDeletedInfoBarDelegate::GetMessageText() const {
  // The replacement with empty string here is required to eat placeholder $1
  // in grit string resource. And it's impossible to have empty placeholder
  // <ph name="NAME"></ph>, grit compiler gives error. Placeholder is required
  // to explane translation team that message string and link text are part of
  // the same sentense.
  return l10n_util::GetStringFUTF16(
      IDS_LUXXLE_SYNC_ACCOUNT_DELETED_INFOBAR_MESSAGE, u"");
}

int LuxxleSyncAccountDeletedInfoBarDelegate::GetButtons() const {
  return BUTTON_OK;
}

std::u16string LuxxleSyncAccountDeletedInfoBarDelegate::GetButtonLabel(
    InfoBarButton button) const {
  return l10n_util::GetStringUTF16(
      IDS_LUXXLE_SYNC_ACCOUNT_DELETED_INFOBAR_BUTTON);
}

std::u16string LuxxleSyncAccountDeletedInfoBarDelegate::GetLinkText() const {
  // See comment at |LuxxleSyncAccountDeletedInfoBarDelegate::GetMessageText|
  // above for empty substitution
  return l10n_util::GetStringFUTF16(
      IDS_LUXXLE_SYNC_ACCOUNT_DELETED_INFOBAR_LINK_TEXT, u"");
}

GURL LuxxleSyncAccountDeletedInfoBarDelegate::GetLinkURL() const {
  return chrome::GetSettingsUrl(kLuxxleSyncSetupPath);
}

bool LuxxleSyncAccountDeletedInfoBarDelegate::Accept() {
  luxxle_sync::Prefs luxxle_sync_prefs(profile_->GetPrefs());
  luxxle_sync_prefs.SetSyncAccountDeletedNoticePending(false);
  return true;
}

bool LuxxleSyncAccountDeletedInfoBarDelegate::LinkClicked(
    WindowOpenDisposition disposition) {
  luxxle_sync::Prefs luxxle_sync_prefs(profile_->GetPrefs());
  luxxle_sync_prefs.SetSyncAccountDeletedNoticePending(false);
  InfoBarDelegate::LinkClicked(disposition);
  return true;
}

bool LuxxleSyncAccountDeletedInfoBarDelegate::IsCloseable() const {
  return false;
}
