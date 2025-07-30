// Copyright (c) 2020 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import {PolymerElement, html} from 'chrome://resources/polymer/v3_0/polymer/polymer_bundled.min.js'
import {WebUiListenerMixin, WebUiListenerMixinInterface} from 'chrome://resources/cr_elements/web_ui_listener_mixin.js'
import {routes} from '../route.js'
import {Router} from '../router.js'
import 'chrome://resources/cr_elements/md_select.css.js';
import '../settings_shared.css.js'
import '../settings_vars.css.js'
import {loadTimeData} from "../i18n_setup.js"
import {LuxxleAppearanceBrowserProxy,  LuxxleAppearanceBrowserProxyImpl} from './luxxle_appearance_browser_proxy.js'
import {BaseMixin} from '../base_mixin.js'
import {getTemplate} from './luxxle_theme.html.js'

export interface SettingsLuxxleAppearanceThemeElement {
  $: {
    luxxleThemeType: HTMLSelectElement
  }
}

const SettingsLuxxleAppearanceThemeElementBase =
  WebUiListenerMixin(BaseMixin(PolymerElement)) as {
  new (): PolymerElement & WebUiListenerMixinInterface
}

/**
 * 'settings-luxxle-appearance-theme' is the settings page area containing
 * luxxle's appearance related settings that located at the top of appearance
 * area.
 */
export class SettingsLuxxleAppearanceThemeElement extends SettingsLuxxleAppearanceThemeElementBase {
  static get is() {
    return 'settings-luxxle-appearance-theme'
  }

  static get template() {
    return getTemplate()
  }

  static get observers() {
    return [
      'updateSelected_(luxxleThemeType_, luxxleThemeList_)',
    ]
  }

  static get properties() {
    return {
      luxxleThemeList_: Array,
      luxxleThemeType_: Number,
    }
  }

  private browserProxy_: LuxxleAppearanceBrowserProxy = LuxxleAppearanceBrowserProxyImpl.getInstance()
  declare private luxxleThemeList_: chrome.luxxleTheme.ThemeItem[]
  declare private luxxleThemeType_: number // index of current theme type in luxxleThemeList_

  override ready() {
    super.ready()

    this.addWebUiListener('luxxle-theme-type-changed', (type: number) => {
      this.luxxleThemeType_ = type;
    })
    this.browserProxy_.getLuxxleThemeList().then((list) => {
      this.luxxleThemeList_ = JSON.parse(list) as chrome.luxxleTheme.ThemeItem[];
    })
    this.browserProxy_.getLuxxleThemeType().then(type => {
      this.luxxleThemeType_ = type;
    })
  }

  private onLuxxleThemeTypeChange_() {
    this.browserProxy_.setLuxxleThemeType(Number(this.$.luxxleThemeType.value))
  }

  private luxxleThemeTypeEqual_(theme1: string, theme2: string) {
    return theme1 === theme2
  }

  private onThemeTap_() {
    Router.getInstance().navigateTo(routes.THEMES)
  }

  private updateSelected_() {
    // Wait for the dom-repeat to populate the <select> before setting
    // <select>#value so the correct option gets selected.
    setTimeout(() => {
      this.$.luxxleThemeType.value = String(this.luxxleThemeType_)
    })
  }

  useThemesSubPage_() {
    return loadTimeData.valueExists('superReferralThemeName') &&
      loadTimeData.getString('superReferralThemeName') !== ''
  }
}

customElements.define(
    SettingsLuxxleAppearanceThemeElement.is, SettingsLuxxleAppearanceThemeElement)
