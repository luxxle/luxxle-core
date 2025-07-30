/* Copyright (c) 2023 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "luxxle/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"

#define IsGuestSession                                                   \
  IsGuestSession() ||                                                    \
      (!profile->GetPrefs()->GetBoolean(kLuxxleAutofillPrivateWindows) && \
       (IsOffTheRecord() || profile->IsTor())) ||                        \
      profile->IsGuestSession
#include "src/chrome/browser/password_manager/chrome_password_manager_client.cc"
#undef IsGuestSession
