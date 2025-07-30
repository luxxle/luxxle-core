// Copyright (c) 2020 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {RegisterPolymerComponentBehaviors} from 'chrome://resources/luxxle/polymer_overriding.js'

import {LuxxleResetProfileDialogBehavior} from '../luxxle_reset_page/luxxle_reset_profile_dialog_behavior.js'

RegisterPolymerComponentBehaviors({
  'settings-reset-profile-dialog': [
    LuxxleResetProfileDialogBehavior
  ]
})
