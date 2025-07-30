/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_CONTAINER_VIEW_H_
#define LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_CONTAINER_VIEW_H_

#include "components/infobars/core/infobar_container.h"

#define PlatformSpecificRemoveInfoBar       \
  PlatformSpecificRemoveInfoBar_UnUsed() {} \
  friend class LuxxleInfoBarContainerView;   \
  void PlatformSpecificRemoveInfoBar

#include "src/chrome/browser/ui/views/infobars/infobar_container_view.h"  // IWYU pragma: export

#undef PlatformSpecificRemoveInfoBar

#endif  // LUXXLE_CHROMIUM_SRC_CHROME_BROWSER_UI_VIEWS_INFOBARS_INFOBAR_CONTAINER_VIEW_H_
