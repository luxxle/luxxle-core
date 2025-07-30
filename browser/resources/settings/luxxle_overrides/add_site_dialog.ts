// Copyright (c) 2022 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {RegisterPolymerComponentReplacement, RegisterPolymerTemplateModifications} from 'chrome://resources/luxxle/polymer_overriding.js'

import {LuxxleAddSiteDialogElement} from '../luxxle_add_site_dialog/luxxle_add_site_dialog.js'

RegisterPolymerComponentReplacement(
  'add-site-dialog',
  LuxxleAddSiteDialogElement
)


RegisterPolymerTemplateModifications({
  'add-site-dialog': (templateContent) => {
    const site = templateContent.querySelector('#site')
    if (!site) {
      console.error(
        `[Luxxle Settings Overrides] cannot find #site in add-site-dialog`)
      return
    }
    site.setAttribute('placeholder', '[[sitePlaceholder]]')
  }
})
