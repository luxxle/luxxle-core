/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_CONFIRM_INFOBAR_H_
#define LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_CONFIRM_INFOBAR_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "luxxle/components/infobars/core/luxxle_confirm_infobar_delegate.h"
#include "chrome/browser/ui/views/infobars/infobar_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

namespace views {
class Checkbox;
class Label;
class MdTextButton;
}  // namespace views

// An infobar that shows a message, up to two optional buttons, and an optional,
// right-aligned link.  This is commonly used to do things like:
// "Would you like to do X?  [Yes]  [No]  [<custom button>]    _Learn More_ [x]"
class LuxxleConfirmInfoBar : public InfoBarView {
  METADATA_HEADER(LuxxleConfirmInfoBar, InfoBarView)
 public:
  explicit LuxxleConfirmInfoBar(
      std::unique_ptr<LuxxleConfirmInfoBarDelegate> delegate);

  LuxxleConfirmInfoBar(const LuxxleConfirmInfoBar&) = delete;
  LuxxleConfirmInfoBar& operator=(const LuxxleConfirmInfoBar&) = delete;

  ~LuxxleConfirmInfoBar() override;

  // InfoBarView:
  void Layout(PassKey) override;

  LuxxleConfirmInfoBarDelegate* GetDelegate() const;

 private:
  // InfoBarView:
  int GetContentMinimumWidth() const override;

  void OkButtonPressed();
  void CancelButtonPressed();
  void ExtraButtonPressed();
  void CloseButtonPressed() override;
  void CheckboxPressed();

  views::MdTextButton* GetButtonById(int id);

  // Returns the width of all content other than the label and link.  Layout()
  // uses this to determine how much space the label and link can take.
  int NonLabelWidth() const;

  raw_ptr<views::Label> label_ = nullptr;
  raw_ptr<views::MdTextButton> ok_button_ = nullptr;
  raw_ptr<views::MdTextButton> cancel_button_ = nullptr;
  raw_ptr<views::MdTextButton> extra_button_ = nullptr;
  raw_ptr<views::Link> link_ = nullptr;
  raw_ptr<views::Checkbox> checkbox_ = nullptr;

  base::WeakPtrFactory<LuxxleConfirmInfoBar> weak_ptr_factory_{this};
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_CONFIRM_INFOBAR_H_
