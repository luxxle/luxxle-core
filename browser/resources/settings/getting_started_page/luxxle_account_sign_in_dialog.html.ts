/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { html } from '//resources/lit/v3_0/lit.rollup.js'

import './luxxle_account_dialog.js'
import { onEyeIconClicked } from './luxxle_account_common.js'
import { SettingsLuxxleAccountSignInDialogElement } from './luxxle_account_sign_in_dialog.js'

export function getHtml(this: SettingsLuxxleAccountSignInDialogElement) {
  return html`<!--_html_template_start_-->
    <settings-luxxle-account-dialog
      dialog-description="$i18n{luxxleAccountSignInDialogDescription}"
      dialog-title="$i18n{luxxleAccountSignInDialogTitle}"
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
        <leo-input placeholder="$i18n{luxxleAccountPasswordInputPlaceholder}"
                   type="password"
                   @input=${this.onPasswordInput}>
          <div class="password">
            <div class="label">$i18n{luxxleAccountPasswordInputLabel}</div>
            <div class="forgot-password"
                 @click=${() => this.fire('forgot-password-button-clicked')}>
              $i18n{luxxleAccountForgotPasswordButtonLabel}
            </div>
          </div>
          <leo-icon name="eye-off"
                    slot="right-icon"
                    @click=${onEyeIconClicked}>
          </leo-icon>
        </leo-input>
      </div>
      <div slot="buttons">
        <leo-button ?isDisabled=${!this.isEmailValid || !this.isPasswordValid}
                    @click=${() => this.fire('sign-in-button-clicked')}>
          $i18n{luxxleAccountSignInButtonLabel}
        </leo-button>
      </div>
    </settings-luxxle-account-dialog>
  <!--_html_template_end_-->`
}
