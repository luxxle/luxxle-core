/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { CrDialogElement } from '//resources/cr_elements/cr_dialog/cr_dialog.js'
import { CrLitElement } from '//resources/lit/v3_0/lit.rollup.js'

import { getCss } from './luxxle_account_dialog.css.js'
import { getHtml } from './luxxle_account_dialog.html.js'

export interface SettingsLuxxleAccountDialogElement {
  $: {
    dialog: CrDialogElement,
  }
}

export class SettingsLuxxleAccountDialogElement extends CrLitElement {
  static get is() {
    return 'settings-luxxle-account-dialog'
  }

  static override get styles() {
    return getCss()
  }

  override render() {
    return getHtml.bind(this)()
  }

  static override get properties() {
    return {
      alertMessage: { type: String },
      dialogDescription: { type: String },
      dialogTitle: { type: String },
      horizontalButtons: { type: Boolean },
      showBackButton: { type: Boolean },
    }
  }

  protected accessor alertMessage: string = ''
  protected accessor dialogDescription: string
  protected accessor dialogTitle: string
  protected accessor horizontalButtons: boolean
  protected accessor showBackButton: boolean
}

declare global {
  interface HTMLElementTagNameMap {
    'settings-luxxle-account-dialog': SettingsLuxxleAccountDialogElement
  }
}

customElements.define(
  SettingsLuxxleAccountDialogElement.is, SettingsLuxxleAccountDialogElement)
