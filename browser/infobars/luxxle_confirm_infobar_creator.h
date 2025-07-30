/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef LUXXLE_BROWSER_INFOBARS_LUXXLE_CONFIRM_INFOBAR_CREATOR_H_
#define LUXXLE_BROWSER_INFOBARS_LUXXLE_CONFIRM_INFOBAR_CREATOR_H_

#include <memory>

class LuxxleConfirmInfoBarDelegate;

namespace infobars {
class InfoBar;
}  // namespace infobars

std::unique_ptr<infobars::InfoBar> CreateLuxxleConfirmInfoBar(
    std::unique_ptr<LuxxleConfirmInfoBarDelegate> delegate);

#endif  // LUXXLE_BROWSER_INFOBARS_LUXXLE_CONFIRM_INFOBAR_CREATOR_H_
