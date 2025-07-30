// Copyright (c) 2023 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import {PolymerElement} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'
import {WebUiListenerMixin, WebUiListenerMixinInterface} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js'
import {BaseMixin} from '../base_mixin.js'
import {PerformanceBrowserProxy, PerformanceBrowserProxyImpl} from '../performance_page/performance_browser_proxy.js'
import {getTemplate} from './luxxle_performance_page.html.js'
import {pageVisibility} from '../page_visibility.js'
import {loadTimeData} from '../i18n_setup.js'
import '../performance_page/battery_page.js'
import '../performance_page/memory_page.js'
import '../performance_page/tab_discard/exception_list.js';

/**
 * 'settings-luxxle-performance-page' is the settings page containing
 * luxxle's performance features.
 */

const SettingsLuxxlePerformancePageElementBase =
  WebUiListenerMixin(BaseMixin(PolymerElement)) as {
    new(): PolymerElement & WebUiListenerMixinInterface
  }

export class SettingsLuxxlePerformancePageElement
    extends SettingsLuxxlePerformancePageElementBase {
  static get is() {
    return 'settings-luxxle-performance-page'
  }

  static get template() {
    return getTemplate()
  }

  static get properties() {
    return {
      /**
       * Used to hide battery settings section if the device has no battery
       */
      showBatterySettings_: {
        type: Boolean,
        value: false,
      },
    }
  }

  private declare showBatterySettings_: boolean;
  private performanceBrowserProxy_: PerformanceBrowserProxy =
      PerformanceBrowserProxyImpl.getInstance();

  override connectedCallback() {
    super.connectedCallback()

    this.addWebUiListener(
      'device-has-battery-changed',
      this.onDeviceHasBatteryChanged_.bind(this))
    this.performanceBrowserProxy_.getDeviceHasBattery().then(
      this.onDeviceHasBatteryChanged_.bind(this))
  }

  private showPerformancePage_(): boolean {
    return pageVisibility?.performance !== false
  }

  private showBatteryPage_(): boolean {
    return pageVisibility?.performance !== false
  }

  private onDeviceHasBatteryChanged_(deviceHasBattery: boolean) {
    this.showBatterySettings_ = deviceHasBattery
  }
}

declare global {
  interface HTMLElementTagNameMap {
    'settings-luxxle-performance-page': SettingsLuxxlePerformancePageElement
  }
}

customElements.define(
    SettingsLuxxlePerformancePageElement.is, SettingsLuxxlePerformancePageElement)
