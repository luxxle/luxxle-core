/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { CrLitElement } from '//resources/lit/v3_0/lit.rollup.js'

import { getCss } from './luxxle_account_row.css.js'
import { getHtml } from './luxxle_account_row.html.js'

export enum Dialog {
  NONE,
  CREATE,
  ENTRY,
  FORGOT_PASSWORD,
  SIGN_IN
}

export class SettingsLuxxleAccountRow extends CrLitElement {
  static get is() {
    return 'settings-luxxle-account-row'
  }

  static override get styles() {
    return getCss()
  }

  override render() {
    return getHtml.bind(this)()
  }

  static override get properties() {
    return {
      dialog: { type: Dialog },
      signedIn: { type: Boolean, reflect: true },
    }
  }

  protected onBackButtonClicked() {
    switch (this.dialog) {
      case Dialog.CREATE:
        this.dialog = Dialog.ENTRY
        break
      case Dialog.FORGOT_PASSWORD:
        this.dialog = Dialog.SIGN_IN
        break
      case Dialog.SIGN_IN:
        this.dialog = Dialog.ENTRY
        break
    }
  }

  protected accessor dialog: Dialog = Dialog.NONE
  protected accessor signedIn: boolean = false
}

declare global {
  interface HTMLElementTagNameMap {
    'settings-luxxle-account-row': SettingsLuxxleAccountRow
  }
}

customElements.define(SettingsLuxxleAccountRow.is, SettingsLuxxleAccountRow)
