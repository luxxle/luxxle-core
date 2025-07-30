/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/browser/ui/startup/luxxle_obsolete_system_infobar_delegate.h"

#include <memory>

#include "base/functional/bind.h"
#include "luxxle/browser/infobars/luxxle_confirm_infobar_creator.h"
#include "luxxle/browser/ui/browser_dialogs.h"
#include "luxxle/grit/luxxle_generated_resources.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/infobars/confirm_infobar_creator.h"
#include "chrome/browser/obsolete_system/obsolete_system.h"
#include "chrome/common/pref_names.h"
#include "components/infobars/content/content_infobar_manager.h"
#include "components/infobars/core/infobar.h"
#include "components/prefs/pref_service.h"
#include "components/strings/grit/components_strings.h"
#include "ui/base/l10n/l10n_util.h"

// static
void LuxxleObsoleteSystemInfoBarDelegate::Create(
    infobars::ContentInfoBarManager* infobar_manager) {
  infobar_manager->AddInfoBar(
      CreateLuxxleConfirmInfoBar(std::unique_ptr<LuxxleConfirmInfoBarDelegate>(
          new LuxxleObsoleteSystemInfoBarDelegate())));
}

LuxxleObsoleteSystemInfoBarDelegate::LuxxleObsoleteSystemInfoBarDelegate() =
    default;
LuxxleObsoleteSystemInfoBarDelegate::~LuxxleObsoleteSystemInfoBarDelegate() =
    default;

bool LuxxleObsoleteSystemInfoBarDelegate::HasCheckbox() const {
  return true;
}

std::u16string LuxxleObsoleteSystemInfoBarDelegate::GetCheckboxText() const {
  return l10n_util::GetStringUTF16(
      IDS_OBSOLETE_SYSTEM_INFOBAR_DONT_SHOW_BUTTON);
}

void LuxxleObsoleteSystemInfoBarDelegate::SetCheckboxChecked(bool checked) {
  launch_confirmation_dialog_ = checked;
}

bool LuxxleObsoleteSystemInfoBarDelegate::InterceptClosing() {
  if (!launch_confirmation_dialog_)
    return false;

  // This infobar will be destroyed after confirmation dialog closed.
  luxxle::ShowObsoleteSystemConfirmDialog(base::BindOnce(
      &LuxxleObsoleteSystemInfoBarDelegate::OnConfirmDialogClosing,
      weak_factory_.GetWeakPtr()));
  return true;
}

int LuxxleObsoleteSystemInfoBarDelegate::GetButtons() const {
  return BUTTON_NONE;
}

std::vector<int> LuxxleObsoleteSystemInfoBarDelegate::GetButtonsOrder() const {
  return {};
}

void LuxxleObsoleteSystemInfoBarDelegate::OnConfirmDialogClosing(bool suppress) {
  if (suppress) {
    if (PrefService* local_state = g_browser_process->local_state()) {
      local_state->SetBoolean(prefs::kSuppressUnsupportedOSWarning, true);
    }
  }

  // infobar()->RemoveSelf() will destroy this also.
  // Do not refer anything after this.
  infobar()->RemoveSelf();
}

infobars::InfoBarDelegate::InfoBarIdentifier
LuxxleObsoleteSystemInfoBarDelegate::GetIdentifier() const {
  return OBSOLETE_SYSTEM_INFOBAR_DELEGATE;
}

std::u16string LuxxleObsoleteSystemInfoBarDelegate::GetLinkText() const {
  return l10n_util::GetStringUTF16(IDS_LEARN_MORE);
}

GURL LuxxleObsoleteSystemInfoBarDelegate::GetLinkURL() const {
  return GURL(ObsoleteSystem::GetLinkURL());
}

std::u16string LuxxleObsoleteSystemInfoBarDelegate::GetMessageText() const {
  return ObsoleteSystem::LocalizedObsoleteString();
}

bool LuxxleObsoleteSystemInfoBarDelegate::ShouldExpire(
    const NavigationDetails& details) const {
  // Since the obsolete system infobar communicates critical state ("your system
  // is no longer receiving updates") it should persist until explicitly
  // dismissed.
  return false;
}
