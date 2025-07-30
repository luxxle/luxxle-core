/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { html } from '//resources/lit/v3_0/lit.rollup.js'

import './luxxle_account_dialog.js'
import { SettingsLuxxleAccountEntryDialogElement } from './luxxle_account_entry_dialog.js'

export function getHtml(this: SettingsLuxxleAccountEntryDialogElement) {
  return html`<!--_html_template_start_-->
    <settings-luxxle-account-dialog
      dialog-description="$i18n{luxxleAccountEntryDialogDescription}"
      dialog-title="$i18n{luxxleAccountEntryDialogTitle}"
    >
      <div slot="buttons">
        <leo-button @click=${() => this.fire('create-button-clicked')}>
          $i18n{luxxleAccountCreateLuxxleAccountButtonLabel}
        </leo-button>
        <leo-button kind="outline"
                    @click=${() => this.fire('sign-in-button-clicked')}>
          $i18n{luxxleAccountAlreadyHaveAccountSignInButtonLabel}
        </leo-button>
      </div>
      <div slot="footer">
        <div class="footer-text">
          $i18nRaw{luxxleAccountSelfCustodyDescription}
        </div>
        <leo-button kind="plain-faint"
                    @click=${() => this.fire('self-custody-button-clicked')}>
          $i18n{luxxleAccountSelfCustodyButtonLabel}
        </leo-button>
      </div>
    </settings-luxxle-account-dialog>
  <!--_html_template_end_-->`
}
