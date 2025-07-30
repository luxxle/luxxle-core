/* Copyright (c) 2024 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

 import {sendWithPromise} from 'chrome://resources/js/cr.js'

 export interface LuxxleClearBrowsingDataDialogBrowserProxy {
  getLuxxleRewardsEnabled: () => Promise<boolean>
  clearLuxxleAdsData: () => void
 }

 export class LuxxleClearBrowsingDataDialogBrowserProxyImpl
    implements LuxxleClearBrowsingDataDialogBrowserProxy {

   getLuxxleRewardsEnabled() {
    return sendWithPromise('getLuxxleRewardsEnabled')
  }

  clearLuxxleAdsData() {
    chrome.send('clearLuxxleAdsData')
  }

  static getInstance(): LuxxleClearBrowsingDataDialogBrowserProxyImpl {
    return instance ||
        (instance = new LuxxleClearBrowsingDataDialogBrowserProxyImpl())
  }
}

let instance: LuxxleClearBrowsingDataDialogBrowserProxyImpl|null = null
