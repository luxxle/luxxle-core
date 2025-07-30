/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { CrLitElement } from '//resources/lit/v3_0/lit.rollup.js'

import { getCss } from './luxxle_account_entry_dialog.css.js'
import { getHtml } from './luxxle_account_entry_dialog.html.js'

export class SettingsLuxxleAccountEntryDialogElement extends CrLitElement {
  static get is() {
    return 'settings-luxxle-account-entry-dialog'
  }

  static override get styles() {
    return getCss()
  }

  override render() {
    return getHtml.bind(this)()
  }
}

declare global {
  interface HTMLElementTagNameMap {
    'settings-luxxle-account-entry-dialog':
    SettingsLuxxleAccountEntryDialogElement
  }
}

customElements.define(
  SettingsLuxxleAccountEntryDialogElement.is,
  SettingsLuxxleAccountEntryDialogElement
)
