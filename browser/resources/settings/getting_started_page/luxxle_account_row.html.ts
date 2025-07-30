/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { html, nothing } from '//resources/lit/v3_0/lit.rollup.js'

import './luxxle_account_create_dialog.js'
import './luxxle_account_entry_dialog.js'
import './luxxle_account_forgot_password_dialog.js'
import './luxxle_account_sign_in_dialog.js'
import { Dialog, SettingsLuxxleAccountRow } from './luxxle_account_row.js'

export function getHtml(this: SettingsLuxxleAccountRow) {
  return html`<!--_html_template_start_-->
    <div class="row">
      <div class="circle">
        <leo-icon name="social-luxxle-release-favicon-fullheight-color">
        </leo-icon>
      </div>
      <div class="title-and-description">
        <div class="title">
          ${this.signedIn
            ? 'John Doe'
            : '$i18n{luxxleAccountRowTitle}'}
        </div>
        <div class="description">
          ${this.signedIn
            ? 'johndoe@gmail.com'
            : '$i18n{luxxleAccountRowDescription}'}
        </div>
      </div>
      <leo-button kind=${this.signedIn ? 'outline' : 'filled'}
                  size="small"
                  @click=${this.signedIn
                    ? () => this.signedIn = false
                    : () => this.dialog = Dialog.ENTRY}>
        ${this.signedIn
          ? '$i18n{luxxleAccountManageAccountButtonLabel}'
          : '$i18n{luxxleAccountGetStartedButtonLabel}'
        }
      </leo-button>
    </div>
    ${(() => {
      switch(this.dialog) {
        case Dialog.NONE:
          return nothing
        case Dialog.ENTRY:
          return html`
            <settings-luxxle-account-entry-dialog
              @close=${() => this.dialog = Dialog.NONE}
              @create-button-clicked=${() => this.dialog = Dialog.CREATE}
              @self-custody-button-clicked=${() => this.dialog = Dialog.NONE}
              @sign-in-button-clicked=${() => this.dialog = Dialog.SIGN_IN}>
            </settings-luxxle-account-entry-dialog>
          `
        case Dialog.CREATE:
          return html`
            <settings-luxxle-account-create-dialog
              @back-button-clicked=${this.onBackButtonClicked}
              @close=${() => this.dialog = Dialog.NONE}
              @create-account-button-clicked=${() => this.dialog = Dialog.NONE}>
            </settings-luxxle-account-create-dialog>
          `
        case Dialog.SIGN_IN:
          return html`
            <settings-luxxle-account-sign-in-dialog
              @back-button-clicked=${this.onBackButtonClicked}
              @close=${() => this.dialog = Dialog.NONE}
              @forgot-password-button-clicked=${() => this.dialog = Dialog.FORGOT_PASSWORD}
              @sign-in-button-clicked=${() => {this.dialog = Dialog.NONE; this.signedIn = true}}>
            </settings-luxxle-account-sign-in-dialog>
          `
        case Dialog.FORGOT_PASSWORD:
          return html`
            <settings-luxxle-account-forgot-password-dialog
              @back-button-clicked=${this.onBackButtonClicked}
              @cancel-button-clicked=${this.onBackButtonClicked}
              @close=${() => this.dialog = Dialog.NONE}>
            </settings-luxxle-account-forgot-password-dialog>
          `
      }
    })()}
  <!--_html_template_end_-->`
}
