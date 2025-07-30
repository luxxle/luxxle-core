/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_INFOBAR_CONTAINER_VIEW_H_
#define LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_INFOBAR_CONTAINER_VIEW_H_

#include "chrome/browser/ui/views/infobars/infobar_container_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

class LuxxleInfoBarContainerView : public InfoBarContainerView {
  METADATA_HEADER(LuxxleInfoBarContainerView, InfoBarContainerView)
 public:
  explicit LuxxleInfoBarContainerView(
      infobars::InfoBarContainer::Delegate* delegate);
  LuxxleInfoBarContainerView(const LuxxleInfoBarContainerView&) = delete;
  LuxxleInfoBarContainerView& operator=(const LuxxleInfoBarContainerView&) =
      delete;
  ~LuxxleInfoBarContainerView() override;
};

#endif  // LUXXLE_BROWSER_UI_VIEWS_INFOBARS_LUXXLE_INFOBAR_CONTAINER_VIEW_H_
