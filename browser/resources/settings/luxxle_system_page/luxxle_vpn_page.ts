// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import '/shared/settings/prefs/prefs.js';
import 'chrome://resources/cr_elements/cr_button/cr_button.js';
import 'chrome://resources/cr_elements/cr_icon_button/cr_icon_button.js';

import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'
import {PrefsMixin, PrefsMixinInterface} from '/shared/settings/prefs/prefs_mixin.js';
import {BaseMixin} from '../base_mixin.js'
import {getTemplate} from './luxxle_vpn_page.html.js'
import {WebUiListenerMixin, WebUiListenerMixinInterface} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js';
import {LuxxleVPNBrowserProxy, LuxxleVPNBrowserProxyImpl} from './luxxle_vpn_browser_proxy.js';
import {I18nMixin, I18nMixinInterface} from 'chrome://resources/cr_elements/i18n_mixin.js'
/**
 * 'settings-luxxle-vpn-page' is the settings page containing
 * luxxle's vpn features.
 */
const SettingsLuxxleVpnPageElementBase =
  PrefsMixin(BaseMixin(I18nMixin(WebUiListenerMixin(
    PolymerElement)))) as {
    new(): PolymerElement &
           PrefsMixinInterface &
           WebUiListenerMixinInterface &
           I18nMixinInterface
  }

export class SettingsLuxxleVpnPageElement
    extends SettingsLuxxleVpnPageElementBase {

  static get is() {
    return 'settings-luxxle-vpn-page'
  }

  static get template() {
    return getTemplate()
  }

  private toggleWireguardSubLabel_: String;
  private luxxleVpnConnected_: Boolean = false;

  private vpnBrowserProxy_: LuxxleVPNBrowserProxy =
    LuxxleVPNBrowserProxyImpl.getInstance();

  override ready() {
    super.ready();
    this.addWebUiListener('luxxle-vpn-state-change', this.onVpnStateChange.bind(this))
    // <if expr="is_win">
    this.vpnBrowserProxy_.isLuxxleVpnConnected().then(this.onVpnStateChange.bind(this))
    // </if>
  }

  private onVpnStateChange(connected: boolean) {
    this.luxxleVpnConnected_ = connected
    this.updateState()
  }

  private updateState() {
    this.toggleWireguardSubLabel_ = this.luxxleVpnConnected_ ?
      this.i18n('sublabelVpnConnected') : ''
  }
}

declare global {
  interface HTMLElementTagNameMap {
    'settings-luxxle-vpn-page': SettingsLuxxleVpnPageElement
  }
}

customElements.define(
  SettingsLuxxleVpnPageElement.is, SettingsLuxxleVpnPageElement)
