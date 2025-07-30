// Copyright (c) 2020 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js';
import {WebUiListenerMixin, WebUiListenerMixinInterface} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js';
import {BaseMixin} from '../base_mixin.js'
import {NewTabOption, LuxxleNewTabBrowserProxy, LuxxleNewTabBrowserProxyImpl} from './luxxle_new_tab_browser_proxy.js'
import {getTemplate} from './luxxle_new_tab_page.html.js'

/**
 * 'settings-luxxle-new-tab-page' is the settings page containing
 * luxxle's new tab features.
 */

const SettingsLuxxleNewTabPageElementBase =
  WebUiListenerMixin(BaseMixin(PolymerElement)) as {
    new(): PolymerElement & WebUiListenerMixinInterface
  }

export class SettingsLuxxleNewTabPageElement extends SettingsLuxxleNewTabPageElementBase {
  static get is() {
    return 'settings-luxxle-new-tab-page'
  }

  static get template() {
    return getTemplate()
  }

  static get properties() {
    return {
      newTabShowOptions_: Array,
      showNewTabDashboardSettings_: Boolean,
    };
  }

  private declare newTabShowOptions_: NewTabOption[];
  private declare showNewTabDashboardSettings_: boolean;

  browserProxy_: LuxxleNewTabBrowserProxy = LuxxleNewTabBrowserProxyImpl.getInstance();

  override ready() {
    super.ready()
    this.openNewTabPage_ = this.openNewTabPage_.bind(this)

    this.browserProxy_.getNewTabShowsOptionsList().then((list: NewTabOption[]) => {
      this.newTabShowOptions_ = list
    })
    this.browserProxy_.shouldShowNewTabDashboardSettings().then((show: boolean) => {
      this.showNewTabDashboardSettings_ = show
    })
    this.addWebUiListener(
      'show-new-tab-dashboard-settings-changed', (show: boolean) => {
        this.showNewTabDashboardSettings_ = show
    })
  }

  openNewTabPage_() {
    window.open("chrome://newTab?openSettings=1", "_self")
  }
}

customElements.define(
  SettingsLuxxleNewTabPageElement.is, SettingsLuxxleNewTabPageElement)
