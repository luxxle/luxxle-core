/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { html } from '//resources/lit/v3_0/lit.rollup.js'

import { onEyeIconClicked } from './luxxle_account_common.js'
import { SettingsLuxxleAccountCreateDialogElement } from './luxxle_account_create_dialog.js'

export function getHtml(this: SettingsLuxxleAccountCreateDialogElement) {
  return html`<!--_html_template_start_-->
    <settings-luxxle-account-dialog
      dialog-description="$i18n{luxxleAccountCreateDialogDescription}"
      dialog-title="$i18n{luxxleAccountCreateDialogTitle}"
      show-back-button
    >
      <div slot="inputs">
        <leo-input placeholder="$i18n{luxxleAccountEmailInputPlaceholder}"
                   showErrors
                   @input=${this.onEmailInput}>
          <div class="label ${this.email.length !== 0 && !this.isEmailValid
                           || this.isEmailValid && this.isEmailLuxxleAlias ?
                              'error' : ''}">
            $i18n{luxxleAccountEmailInputLabel}
          </div>
          <div class="dropdown ${this.isEmailValid && this.isEmailLuxxleAlias ?
                                 'visible' : ''}"
               id="luxxle-alias-dropdown"
               slot="errors">
            <leo-icon name="warning-triangle-filled"></leo-icon>
            <div>$i18n{luxxleAccountEmailInputErrorMessage}</div>
          </div>
        </leo-input>
        <leo-input placeholder="$i18n{luxxleAccountPasswordInputPlaceholder}"
                   showErrors
                   type="password"
                   @input=${this.onPasswordInput}>
          <div class="label">$i18n{luxxleAccountCreatePasswordInputLabel}</div>
          <leo-icon name="eye-off"
                    slot="right-icon"
                    @click=${onEyeIconClicked}>
          </leo-icon>
          <div slot="errors" class="dropdown ${this.passwordStrength !== 0 ?
                                               'visible' : ''}"
                             id="password-dropdown">
            <password-strength-meter strength=${this.passwordStrength}>
            </password-strength-meter>
          </div>
        </leo-input>
        <leo-input placeholder="$i18n{luxxleAccountConfirmPasswordInputPlaceholder}"
                   showErrors
                   type="password"
                   @input=${this.onConfirmPasswordInput}>
          <div class="label ${this.passwordConfirmation.length !== 0
                           && this.passwordConfirmation !== this.password ?
                              'error' : ''}">
            $i18n{luxxleAccountConfirmPasswordInputLabel}
          </div>
          <leo-icon name="eye-off"
                    slot="right-icon"
                    @click=${onEyeIconClicked}>
          </leo-icon>
          <div class="dropdown ${this.passwordConfirmation.length !== 0 ?
                                 'visible' : ''}"
               id="password-confirmation-dropdown"
               slot="errors">
            <leo-icon name=${this.getIconName()}></leo-icon>
            <div>
              ${this.icon === 'check-circle-filled'
                ? html`$i18n{luxxleAccountConfirmPasswordInputSuccessMessage}`
                : html`$i18n{luxxleAccountConfirmPasswordInputErrorMessage}`
              }
            </div>
          </div>
        </leo-input>
        <leo-checkbox @change=${this.onCheckboxChanged}>
          <div>$i18nRaw{luxxleAccountConsentCheckboxLabel}</div>
        </leo-checkbox>
      </div>
      <div slot="buttons">
        <leo-button ?isDisabled=${!this.isEmailValid
                               || this.isEmailValid && this.isEmailLuxxleAlias
                               || this.passwordStrength !== 100
                               || this.passwordConfirmation !== this.password
                               || !this.isCheckboxChecked}
                    @click=${() => this.fire('create-account-button-clicked')}>
          $i18n{luxxleAccountCreateAccountButtonLabel}
        </leo-button>
      </div>
    </settings-luxxle-account-dialog>
  <!--_html_template_end_-->`
}
