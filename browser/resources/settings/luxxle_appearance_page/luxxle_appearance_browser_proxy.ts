/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import {sendWithPromise} from 'chrome://resources/js/cr.js';

/** @interface */
export interface LuxxleAppearanceBrowserProxy {
  /**
   * Returns JSON string with shape `chrome.luxxleTheme.ThemeItem[]`
   */
  getLuxxleThemeList(): Promise<string>
  /**
   * Index of current ThemeItem
   */
  getLuxxleThemeType(): Promise<number>
  /**
   * 
   * @param value index of ThemeItem
   */
  setLuxxleThemeType(value: number): void
}

/**
 * @implements {LuxxleAppearanceBrowserProxy}
 */
export class LuxxleAppearanceBrowserProxyImpl implements
    LuxxleAppearanceBrowserProxy {
  getLuxxleThemeList() {
    return new Promise<string>(resolve => chrome.luxxleTheme.getLuxxleThemeList(resolve))
  }

  getLuxxleThemeType() {
    return sendWithPromise('getLuxxleThemeType');
  }

  setLuxxleThemeType(value: number) {
    chrome.send('setLuxxleThemeType', [value]);
  }

  static getInstance(): LuxxleAppearanceBrowserProxyImpl {
    return instance || (instance = new LuxxleAppearanceBrowserProxyImpl())
  }
}

let instance: LuxxleAppearanceBrowserProxy|null = null
