// Copyright (c) 2022 The Luxxle Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { sendWithPromise } from 'chrome://resources/js/cr.js';

// TODO(petemill): Define the expected types instead of using any
export interface LuxxleTorBrowserProxy {
  getBridgesConfig: () => Promise<any>
  setBridgesConfig: (config: any) => void
  requestBridgesCaptcha: () => Promise<any>
  resolveBridgesCaptcha: (captcha: any) => Promise<any>
  setTorEnabled: (value: boolean) => void
  isTorEnabled: () => Promise<boolean>
  isTorManaged: () => Promise<boolean>
  isSnowflakeExtensionAllowed: () => Promise<boolean>
  isSnowflakeExtensionEnabled: () => Promise<boolean>
  enableSnowflakeExtension: (enable: boolean) => Promise<boolean>
}

export class LuxxleTorBrowserProxyImpl implements LuxxleTorBrowserProxy {
  static getInstance() {
    return instance || (instance = new LuxxleTorBrowserProxyImpl());
  }

  getBridgesConfig() {
    return sendWithPromise('luxxle_tor.getBridgesConfig')
  }

  setBridgesConfig(config: any) {
    chrome.send('luxxle_tor.setBridgesConfig', [config])
  }

  requestBridgesCaptcha() {
    return sendWithPromise('luxxle_tor.requestBridgesCaptcha')
  }

  resolveBridgesCaptcha(captcha: any) {
    return sendWithPromise('luxxle_tor.resolveBridgesCaptcha', captcha)
  }

  setTorEnabled(value: boolean) {
    chrome.send('luxxle_tor.setTorEnabled', [value])
  }

  isTorEnabled() {
    return sendWithPromise('luxxle_tor.isTorEnabled')
  }

  isTorManaged() {
    return sendWithPromise('luxxle_tor.isTorManaged')
  }

  isSnowflakeExtensionAllowed() {
    return sendWithPromise('luxxle_tor.isSnowflakeExtensionAllowed')
  }

  isSnowflakeExtensionEnabled() {
    return sendWithPromise('luxxle_tor.isSnowflakeExtensionEnabled')
  }

  enableSnowflakeExtension(enable: boolean) {
    return sendWithPromise('luxxle_tor.enableSnowflakeExtension', enable)
  }
}

let instance: LuxxleTorBrowserProxy|null = null
