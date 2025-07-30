/* Copyright (c) 2022 The Luxxle Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { sendWithPromise, addWebUiListener } from 'chrome://resources/js/cr.js'

export class Scriptlet {
  name: string

  kind: object = {
    mime: 'application/javascript'
  }

  content: string
}

export interface FilterList {
  uuid: string
  url: string
  title: string
  desc: string
  support_url: string
  component_id: string
  base64_public_key: string
  enabled: boolean
}

export interface SubscriptionInfo {
  subscription_url: string
  last_update_attempt: number
  last_successful_update_attempt: number
  enabled: boolean
  title?: string
  homepage?: string
}

export enum ErrorCode {
  kOK = 0,
  kInvalidName,
  kAlreadyExists,
  kNotFound,
}

export interface LuxxleAdblockBrowserProxy {
  getRegionalLists: () => Promise<FilterList[]>
  enableFilterList: (uuid: string, enabled: boolean) => void
  updateFilterLists: () => Promise<boolean>
  getCustomFilters: () => Promise<string>
  updateCustomFilters: (value: string) => void
  getListSubscriptions: () => Promise<any>
  setSubscriptionEnabled: (url: string, enabled: boolean) => void
  addSubscription: (url: string) => void
  updateSubscription: (url: string) => void
  deleteSubscription: (url: string) => void
  viewSubscription: (url: string) => void
  getCustomScriptlets: () => Promise<Scriptlet[]>
  addCustomScriptlet: (scriptlet: Scriptlet) => Promise<ErrorCode>
  updateCustomScriptlet: (name: string, scriptlet: Scriptlet) =>
    Promise<ErrorCode>
  removeCustomScriptlet: (name: string) => Promise<ErrorCode>
  addWebUiListener: (eventName: string, callback: Function) => void
}

export class LuxxleAdblockBrowserProxyImpl implements LuxxleAdblockBrowserProxy {
  static getInstance() {
    return instance || (instance = new LuxxleAdblockBrowserProxyImpl())
  }

  getRegionalLists () {
    return sendWithPromise('luxxle_adblock.getRegionalLists')
  }

  enableFilterList (uuid: string, enabled: boolean) {
    chrome.send('luxxle_adblock.enableFilterList', [uuid, enabled])
  }

  updateFilterLists () {
    return sendWithPromise('luxxle_adblock.updateFilterLists')
  }

  getCustomFilters () {
    return sendWithPromise('luxxle_adblock.getCustomFilters')
  }

  updateCustomFilters (value: string) {
    chrome.send('luxxle_adblock.updateCustomFilters', [value])
  }

  getListSubscriptions () {
    return sendWithPromise('luxxle_adblock.getListSubscriptions')
  }

  setSubscriptionEnabled (url: string, enabled: boolean) {
    chrome.send('luxxle_adblock.setSubscriptionEnabled', [url, enabled])
  }

  addSubscription (url: string) {
    chrome.send('luxxle_adblock.addSubscription', [url])
  }

  updateSubscription (url: string) {
    chrome.send('luxxle_adblock.updateSubscription', [url])
  }

  deleteSubscription (url: string) {
    chrome.send('luxxle_adblock.deleteSubscription', [url])
  }

  viewSubscription (url: string) {
    chrome.send('luxxle_adblock.viewSubscription', [url])
  }

  getCustomScriptlets () {
    return sendWithPromise('luxxle_adblock.getCustomScriptlets')
      .then((scriptlets) => {
        for (const scriptlet of scriptlets) {
          scriptlet.content = this.base64ToUtf8_(scriptlet.content)
        }
        return scriptlets
      })
      .catch((error) => {
        throw error
      })
  }

  addCustomScriptlet (scriptlet: Scriptlet) {
    scriptlet.content = this.utf8ToBase64_(scriptlet.content)
    return sendWithPromise('luxxle_adblock.addCustomScriptlet', scriptlet)
  }

  updateCustomScriptlet (name: string, scriptlet: Scriptlet) {
    scriptlet.content = this.utf8ToBase64_(scriptlet.content)
    return sendWithPromise(
      'luxxle_adblock.updateCustomScriptlet', name, scriptlet
    )
  }

  removeCustomScriptlet (name: string) {
    return sendWithPromise('luxxle_adblock.removeCustomScriptlet', name)
  }

  addWebUiListener (eventName: string, callback: Function) {
    addWebUiListener(eventName, callback)
  }

  private utf8ToBase64_ (str: string) {
    const uint8Array = new TextEncoder().encode(str)
    const base64String = btoa(String.fromCharCode(...uint8Array))
    return base64String
  }

  private base64ToUtf8_ (base64: string) {
    const binaryString = atob(base64)
    const bytes = new Uint8Array(binaryString.length)
    for (let i = 0; i < binaryString.length; i++) {
      bytes[i] = binaryString.charCodeAt(i)
    }
    return new TextDecoder().decode(bytes)
  }
}

let instance: LuxxleAdblockBrowserProxy|null = null
