/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_SYNC_ACCOUNT_DELETED_INFOBAR_H_
#define LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_SYNC_ACCOUNT_DELETED_INFOBAR_H_

#include <memory>

#include "chrome/browser/ui/views/infobars/confirm_infobar.h"

// The customized ConfirmInfoBar:
// "Text _link_                     [ok_button]"
// cancel_button is not supported

class LuxxleSyncAccountDeletedInfoBar : public ConfirmInfoBar {
 public:
  explicit LuxxleSyncAccountDeletedInfoBar(
      std::unique_ptr<ConfirmInfoBarDelegate> delegate);

  LuxxleSyncAccountDeletedInfoBar(const LuxxleSyncAccountDeletedInfoBar&) =
      delete;
  LuxxleSyncAccountDeletedInfoBar& operator=(
      const LuxxleSyncAccountDeletedInfoBar&) = delete;

  ~LuxxleSyncAccountDeletedInfoBar() override;

  // InfoBarView:
  void Layout(PassKey) override;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_SYNC_ACCOUNT_DELETED_INFOBAR_H_
