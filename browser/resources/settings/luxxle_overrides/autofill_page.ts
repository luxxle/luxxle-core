// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {html, RegisterPolymerTemplateModifications, RegisterPolymerComponentReplacement} from 'chrome://resources/luxxle/polymer_overriding.js'
import {LuxxleSettingsAutofillPageElement} from '../luxxle_autofill_page/luxxle_autofill_page.js'
import {loadTimeData} from '../i18n_setup.js'

RegisterPolymerComponentReplacement(
  'settings-autofill-page', LuxxleSettingsAutofillPageElement
)

RegisterPolymerTemplateModifications({
  'settings-autofill-page': (templateContent) => {
    templateContent.appendChild(html`
        <settings-toggle-button
          class="hr"
          label="${loadTimeData.getString('autofillInPrivateSettingLabel')}"
          sub-label="${loadTimeData.getString('autofillInPrivateSettingDesc')}"
          id="autofill-private-profies"
          pref="{{prefs.luxxle.autofill_private_windows}}"
          hidden=[[!isAutofillPage_]]
        </settings-toggle-button>
      `)
    }
  }
)
