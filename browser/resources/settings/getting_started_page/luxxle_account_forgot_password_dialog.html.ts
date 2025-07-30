/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { html } from '//resources/lit/v3_0/lit.rollup.js'

import './luxxle_account_dialog.js'
import { SettingsLuxxleAccountForgotPasswordDialogElement } from './luxxle_account_forgot_password_dialog.js'

export function getHtml(this: SettingsLuxxleAccountForgotPasswordDialogElement) {
  return html`<!--_html_template_start_-->
    <settings-luxxle-account-dialog
      alert-message="$i18n{luxxleAccountAlertMessage}"
      dialog-description="$i18n{luxxleAccountForgotPasswordDialogDescription}"
      dialog-title="$i18n{luxxleAccountForgotPasswordDialogTitle}"
      horizontal-buttons
      show-back-button
    >
      <div slot="inputs">
        <leo-input placeholder="$i18n{luxxleAccountEmailInputPlaceholder}"
                   @input=${this.onEmailInput}>
          <div class="label ${this.email.length !== 0 && !this.isEmailValid ?
                              'error' : ''}">
            $i18n{luxxleAccountEmailInputLabel}
          </div>
        </leo-input>
      </div>
      <div slot="buttons">
        <leo-button kind="plain-faint"
                    @click=${() => this.fire('cancel-button-clicked')}>
          $i18n{luxxleAccountCancelButtonLabel}
        </leo-button>
        <leo-button ?isDisabled=${!this.isEmailValid}>
          $i18n{luxxleAccountResetPasswordButtonLabel}
        </leo-button>
      </div>
    </settings-luxxle-account-dialog>
  <!--_html_template_end_-->`
}
